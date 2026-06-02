import logging
import os
import signal
from threading import Thread
from time import sleep


log = logging.getLogger(__name__)

_IN_TOPLEVEL_PROCESS = True


def in_toplevel_process() -> bool:
    global _IN_TOPLEVEL_PROCESS
    return _IN_TOPLEVEL_PROCESS


# If this process dies abnormally (e.g. segfault)
# it will not shut down the workers. Instead,
# the workers will have their parent reassigned to the
# init process. This launches a separate thread to
# watch for the worker getting reassigned,
# and cleans it up in this case.
#
# This function cannot be an inner function since otherwise mp_context="spawn" would
# not work for ProcessPoolExecutor since inner functions cannot be pickled.
def _async_compile_initializer(orig_ppid: int) -> None:
    import torch._C

    def run() -> None:
        while True:
            sleep(60)
            if orig_ppid != os.getppid():
                os.kill(os.getpid(), signal.SIGKILL)

    global _watchdog_thread, _original_parent
    _original_parent = orig_ppid
    _watchdog_thread = Thread(target=run, daemon=True)
    _watchdog_thread.start()
    # Ignore Ctrl-C (i.e. SIGINT) sent to pool workers to avoid meaningless log spam.
    signal.signal(signal.SIGINT, signal.SIG_IGN)

    # Install a crash handler to print out the stacktrace for SEGV
    torch._C._initCrashHandler()

    # A worker forked after CUDA init breaks Triton's cuInit-based is_active()
    # (triton#9578); pin the nvidia driver so .active won't raise "0 active
    # drivers". Guard before importing Triton so CPU-only/AMD workers skip it.
    if torch.version.hip is None and torch.cuda.is_available():
        try:
            import triton

            if triton.runtime.driver._active is None:
                backend = triton.backends.backends.get("nvidia")
                if backend is not None:
                    triton.runtime.driver.set_active(backend.driver())
        except Exception:
            log.debug(
                "Failed to pin Triton nvidia driver in compile worker", exc_info=True
            )

    # Set a bit to distinguish async_compile subprocesses from the toplevel process.
    global _IN_TOPLEVEL_PROCESS
    _IN_TOPLEVEL_PROCESS = False


_watchdog_thread: Thread | None = None
_original_parent: int | None = None


def has_parent_changed() -> bool:
    return _original_parent != os.getppid()

#pragma once

#include <c10/macros/Macros.h>
#include <torch/csrc/utils/python_compat.h>

#if defined(_MSC_VER) && defined(_DEBUG) && !defined(Py_DEBUG)
#  pragma push_macro("_DEBUG")
#  undef _DEBUG
#  include <Python.h>
#  pragma pop_macro("_DEBUG")
#else
#  include <Python.h>
#endif

inline PyCFunction castPyCFunctionWithKeywords(PyCFunctionWithKeywords func) {
  C10_DIAGNOSTIC_PUSH_AND_IGNORED_IF_DEFINED("-Wcast-function-type")
  C10_DIAGNOSTIC_PUSH_AND_IGNORED_IF_DEFINED("-Wcast-function-type-strict")
  return reinterpret_cast<PyCFunction>(func);
  C10_DIAGNOSTIC_POP()
  C10_DIAGNOSTIC_POP()
}

#if !IS_PYTHON_3_13_PLUS
using PyCFunctionFast = _PyCFunctionFast;
#endif

inline PyCFunction castPyCFunctionFast(PyCFunctionFast func) {
  C10_DIAGNOSTIC_PUSH_AND_IGNORED_IF_DEFINED("-Wcast-function-type")
  C10_DIAGNOSTIC_PUSH_AND_IGNORED_IF_DEFINED("-Wcast-function-type-strict")
  return reinterpret_cast<PyCFunction>(func);
  C10_DIAGNOSTIC_POP()
  C10_DIAGNOSTIC_POP()
}

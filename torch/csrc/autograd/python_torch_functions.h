#if defined(_MSC_VER) && defined(_DEBUG) && !defined(Py_DEBUG)
#  pragma push_macro("_DEBUG")
#  undef _DEBUG
#  include <Python.h>
#  pragma pop_macro("_DEBUG")
#else
#  include <Python.h>
#endif

namespace torch::autograd {

extern PyObject* THPVariableFunctionsModule;

// Wrapper converts a raised TypeError into returning NotImplemented
// Used to implement binary arithmetic operators
template <PyObject* (*Func)(PyObject*, PyObject*, PyObject*)>
inline PyObject* TypeError_to_NotImplemented_(
    PyObject* self,
    PyObject* args,
    PyObject* kwargs) {
  PyObject* ret = Func(self, args, kwargs);
  if (!ret && PyErr_ExceptionMatches(PyExc_TypeError)) {
    PyErr_Clear();
    ret = Py_NewRef(Py_NotImplemented);
  }
  return ret;
}

void initTorchFunctions(PyObject* module);

} // namespace torch::autograd

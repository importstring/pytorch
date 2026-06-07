#ifndef THP_STORAGE_METHODS_INC
#define THP_STORAGE_METHODS_INC

#if defined(_MSC_VER) && defined(_DEBUG) && !defined(Py_DEBUG)
#  pragma push_macro("_DEBUG")
#  undef _DEBUG
#  include <Python.h>
#  pragma pop_macro("_DEBUG")
#else
#  include <Python.h>
#endif

PyMethodDef* THPStorage_getMethods();

#endif

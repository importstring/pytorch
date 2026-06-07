#pragma once

// C2039 MSVC
#include <pybind11/complex.h>
#include <torch/csrc/utils/pybind.h>

#if defined(_MSC_VER) && defined(_DEBUG) && !defined(Py_DEBUG)
#  pragma push_macro("_DEBUG")
#  undef _DEBUG
#  include <Python.h>
#  pragma pop_macro("_DEBUG")
#else
#  include <Python.h>
#endif

namespace torch::dynamo {
void initDynamoBindings(PyObject* torch);
}

#pragma once

#if defined(_MSC_VER) && defined(_DEBUG) && !defined(Py_DEBUG)
#  pragma push_macro("_DEBUG")
#  undef _DEBUG
#  include <Python.h>
#  pragma pop_macro("_DEBUG")
#else
#  include <Python.h>
#endif

#include <torch/csrc/profiler/collection.h>
#include <torch/csrc/profiler/python/pybind.h>

namespace pybind11::detail {
using torch::profiler::impl::TensorID;

#define STRONG_POINTER_TYPE_CASTER(T) \
  template <>                         \
  struct type_caster<T> : public strong_pointer_type_caster<T> {};

STRONG_POINTER_TYPE_CASTER(torch::profiler::impl::StorageImplData)
STRONG_POINTER_TYPE_CASTER(torch::profiler::impl::AllocationID)
STRONG_POINTER_TYPE_CASTER(torch::profiler::impl::TensorImplAddress)
STRONG_POINTER_TYPE_CASTER(torch::profiler::impl::PyModuleSelf)
STRONG_POINTER_TYPE_CASTER(torch::profiler::impl::PyModuleCls)
STRONG_POINTER_TYPE_CASTER(torch::profiler::impl::PyOptimizerSelf)
#undef STRONG_POINTER_TYPE_CASTER

template <>
struct type_caster<TensorID> : public strong_uint_type_caster<TensorID> {};
} // namespace pybind11::detail

namespace torch::profiler {

void initPythonBindings(PyObject* module);

} // namespace torch::profiler

/**
 * memory_model_stubs.cpp
 *
 * OCaml C FFI stubs for the MemoryModel module.
 * Mirrors SVF-Python/pybind/MemoryModel.cpp.
 *
 * PointsTo bindings live in wpa_stubs.cpp.
 * This file provides only the module-level placeholder.
 */

#include "svf_ocaml_types.h"

extern "C" CAMLprim value caml_memory_model_version(value v_unit) {
    CAMLparam1(v_unit);
    CAMLreturn(caml_copy_string("SVF-OCaml MemoryModel"));
}

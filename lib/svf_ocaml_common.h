/**
 * svf_ocaml_common.h
 *
 * Common macros and definitions for SVF-OCaml C++ stubs.
 * Each C++ stub file includes this header to get the shared
 * custom-block wrappers for all SVF pointer types.
 *
 * The approach:
 *   - Each C++ class is wrapped in an OCaml custom block containing a raw pointer.
 *   - Lifetime is managed by SVF itself (singleton pattern mirrors PySVF in SVF-Python).
 *   - No finalizer deletes the objects (SVF manages memory via SVFIR/LLVMModuleSet).
 */

#pragma once

extern "C" {
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/memory.h>
#include <caml/fail.h>
}

#include <stdexcept>
#include <string>
#include <vector>

/* ---------------------------------------------------------------------------
 * DEFINE_SVF_TYPE(name, CppType)
 *
 * Generates:
 *   - A custom_operations struct: name_ops
 *   - A wrap function: value wrap_name(CppType* ptr)   (fails on nullptr)
 *   - An optional wrap: value wrap_name_opt(CppType*)  (returns None on nullptr)
 *   - An unwrap function: CppType* unwrap_name(value v)
 * --------------------------------------------------------------------------- */
#define DEFINE_SVF_TYPE(name, CppType)                                        \
  static void name##_finalize(value v) {                                      \
    /* Do not delete: lifetime is managed by SVF/LLVMModuleSet */             \
    (void)v;                                                                  \
  }                                                                           \
  static struct custom_operations name##_ops = {                              \
      "svf_ocaml." #name,                                                     \
      name##_finalize,                                                        \
      custom_compare_default,                                                 \
      custom_hash_default,                                                    \
      custom_serialize_default,                                               \
      custom_deserialize_default,                                             \
      custom_compare_ext_default,                                             \
      custom_fixed_length_default                                             \
  };                                                                          \
  static value wrap_##name(CppType* ptr) {                                   \
    if (!ptr) caml_failwith("svf_ocaml: unexpected null " #name " pointer"); \
    CAMLparam0(); CAMLlocal1(_v);                                             \
    _v = caml_alloc_custom(&name##_ops, sizeof(CppType*), 0, 1);             \
    *((CppType**)Data_custom_val(_v)) = ptr;                                 \
    CAMLreturn(_v);                                                           \
  }                                                                           \
  static value wrap_##name##_opt(CppType* ptr) {                             \
    CAMLparam0(); CAMLlocal2(_some, _v);                                      \
    if (!ptr) CAMLreturn(Val_int(0)); /* None */                              \
    _v = caml_alloc_custom(&name##_ops, sizeof(CppType*), 0, 1);             \
    *((CppType**)Data_custom_val(_v)) = ptr;                                 \
    _some = caml_alloc(1, 0); /* Some _ */                                   \
    Store_field(_some, 0, _v);                                               \
    CAMLreturn(_some);                                                        \
  }                                                                           \
  static inline CppType* unwrap_##name(value v) {                           \
    return *((CppType**)Data_custom_val(v));                                  \
  }

/* ---------------------------------------------------------------------------
 * Build an OCaml list from a std::vector<ElemType*> using a wrap function.
 *
 * Usage inside a CAMLprim function that already has CAMLlocal declarations:
 *   CAMLlocal1(result);
 *   VEC_TO_CAML_LIST(result, my_vec, wrap_icfg_node);
 * --------------------------------------------------------------------------- */
#define VEC_TO_CAML_LIST(list_var, vec, wrap_fn)                             \
  do {                                                                        \
    CAMLlocal2(_cons, _head);                                                 \
    list_var = Val_int(0); /* [] */                                           \
    for (int _i = (int)(vec).size() - 1; _i >= 0; --_i) {                   \
      _head = wrap_fn((vec)[_i]);                                             \
      _cons = caml_alloc(2, 0);                                               \
      Store_field(_cons, 0, _head);                                           \
      Store_field(_cons, 1, list_var);                                        \
      list_var = _cons;                                                       \
    }                                                                         \
  } while (0)

/* Build a list of node_id (int) from std::vector<NodeID> */
#define VEC_INT_TO_CAML_LIST(list_var, vec)                                  \
  do {                                                                        \
    CAMLlocal1(_cons);                                                        \
    list_var = Val_int(0); /* [] */                                           \
    for (int _i = (int)(vec).size() - 1; _i >= 0; --_i) {                   \
      _cons = caml_alloc(2, 0);                                               \
      Store_field(_cons, 0, Val_int((int)(vec)[_i]));                        \
      Store_field(_cons, 1, list_var);                                        \
      list_var = _cons;                                                       \
    }                                                                         \
  } while (0)

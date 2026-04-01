/**
 * svf_types_stubs.cpp
 *
 * Defines all custom_operations structs and wrap/unwrap functions for
 * SVF C++ pointer types.  All other stub files declare these via
 * svf_ocaml_types.h (DECL_SVF_WRAP / extern declarations).
 *
 * Having a single definition file ensures:
 *   - Each custom_operations struct has exactly one address in the binary.
 *   - wrap/unwrap calls in different translation units use the same ops pointer.
 *   - OCaml GC sees consistent type identifiers.
 */

/* Include OCaml C runtime headers */
extern "C" {
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/memory.h>
#include <caml/fail.h>
}

/* Pull in all SVF headers so we can refer to the concrete types */
#include "SVF-LLVM/SVFIRBuilder.h"
#include "Graphs/ICFG.h"
#include "Graphs/CallGraph.h"
#include "Graphs/ConsG.h"
#include "Graphs/VFGNode.h"
#include "Graphs/VFGEdge.h"
#include "SVFIR/SVFType.h"
#include "SVFIR/SVFStatements.h"
#include "SVFIR/SVFVariables.h"
#include "MemoryModel/PointerAnalysis.h"
#include "WPA/Andersen.h"
#include "WPA/Steensgaard.h"
#include "MTA/MTA.h"
#include "MTA/MHP.h"
#include "MTA/LockAnalysis.h"
#include "MTA/TCT.h"
#include "AE/Core/AbstractState.h"
#include "AE/Core/IntervalValue.h"
#include "AE/Core/AddressValue.h"

using namespace SVF;

/* =========================================================================
 * Helper macro — generates one custom_operations struct + wrap/unwrap
 * functions with EXTERNAL linkage (no static keyword).
 * ========================================================================= */
#define IMPL_SVF_TYPE(name, CppType)                                         \
  static void name##_finalize(value v) { (void)v; }                         \
  struct custom_operations name##_ops = {                                    \
      "svf_ocaml." #name,                                                    \
      name##_finalize,                                                       \
      custom_compare_default,                                                \
      custom_hash_default,                                                   \
      custom_serialize_default,                                              \
      custom_deserialize_default,                                            \
      custom_compare_ext_default,                                            \
      custom_fixed_length_default                                            \
  };                                                                         \
  value wrap_##name(CppType* ptr) {                                          \
    if (!ptr) caml_failwith("svf_ocaml: unexpected null " #name);            \
    CAMLparam0(); CAMLlocal1(_v);                                             \
    _v = caml_alloc_custom(&name##_ops, sizeof(CppType*), 0, 1);             \
    *((CppType**)Data_custom_val(_v)) = ptr;                                 \
    CAMLreturn(_v);                                                           \
  }                                                                           \
  value wrap_##name##_opt(CppType* ptr) {                                    \
    CAMLparam0(); CAMLlocal2(_some, _v);                                      \
    if (!ptr) CAMLreturn(Val_int(0));                                         \
    _v = caml_alloc_custom(&name##_ops, sizeof(CppType*), 0, 1);             \
    *((CppType**)Data_custom_val(_v)) = ptr;                                 \
    _some = caml_alloc(1, 0);                                                 \
    Store_field(_some, 0, _v);                                               \
    CAMLreturn(_some);                                                        \
  }                                                                           \
  CppType* unwrap_##name(value v) {                                          \
    return *((CppType**)Data_custom_val(v));                                  \
  }

/* =========================================================================
 * Type definitions — one per SVF class
 * ========================================================================= */

IMPL_SVF_TYPE(svfir,           SVFIR)
IMPL_SVF_TYPE(icfg,            ICFG)
IMPL_SVF_TYPE(icfg_node,       ICFGNode)
IMPL_SVF_TYPE(icfg_edge,       ICFGEdge)
IMPL_SVF_TYPE(fun_entry_node,  FunEntryICFGNode)
IMPL_SVF_TYPE(fun_exit_node,   FunExitICFGNode)
IMPL_SVF_TYPE(call_icfg_node,  CallICFGNode)
IMPL_SVF_TYPE(ret_icfg_node,   RetICFGNode)
IMPL_SVF_TYPE(svf_stmt,        SVFStmt)
IMPL_SVF_TYPE(svf_var,         SVFVar)
IMPL_SVF_TYPE(svf_type,        SVFType)
IMPL_SVF_TYPE(svf_basic_block, SVFBasicBlock)
IMPL_SVF_TYPE(call_graph,      CallGraph)
IMPL_SVF_TYPE(call_graph_node, CallGraphNode)
IMPL_SVF_TYPE(call_graph_edge, CallGraphEdge)
IMPL_SVF_TYPE(svfg,            SVFG)
IMPL_SVF_TYPE(vfg_node,        VFGNode)
IMPL_SVF_TYPE(vfg_edge,        VFGEdge)
IMPL_SVF_TYPE(andersen_base,   AndersenBase)
IMPL_SVF_TYPE(andersen_wave_diff, AndersenWaveDiff)
IMPL_SVF_TYPE(steensgaard,     Steensgaard)
IMPL_SVF_TYPE(constraint_graph, ConstraintGraph)
IMPL_SVF_TYPE(constraint_node,  ConstraintNode)
IMPL_SVF_TYPE(constraint_edge,  ConstraintEdge)
IMPL_SVF_TYPE(abstract_state,  AbstractState)
IMPL_SVF_TYPE(fun_obj_var,     FunObjVar)

/* MTA-related types are user-owned — finalizer deletes the C++ object */
#define IMPL_SVF_OWNED_TYPE(name, CppType)                                   \
  static void name##_finalize(value v) {                                     \
    CppType** pp = (CppType**)Data_custom_val(v);                            \
    delete *pp;                                                               \
    *pp = nullptr;                                                            \
  }                                                                           \
  struct custom_operations name##_ops = {                                    \
      "svf_ocaml." #name,                                                    \
      name##_finalize,                                                       \
      custom_compare_default, custom_hash_default,                           \
      custom_serialize_default, custom_deserialize_default,                  \
      custom_compare_ext_default, custom_fixed_length_default                \
  };                                                                         \
  value wrap_##name(CppType* ptr) {                                          \
    if (!ptr) caml_failwith("svf_ocaml: unexpected null " #name);            \
    CAMLparam0(); CAMLlocal1(_v);                                             \
    _v = caml_alloc_custom(&name##_ops, sizeof(CppType*), 0, 1);             \
    *((CppType**)Data_custom_val(_v)) = ptr;                                 \
    CAMLreturn(_v);                                                           \
  }                                                                           \
  value wrap_##name##_opt(CppType* ptr) {                                    \
    CAMLparam0(); CAMLlocal2(_some, _v);                                      \
    if (!ptr) CAMLreturn(Val_int(0));                                         \
    _v = caml_alloc_custom(&name##_ops, sizeof(CppType*), 0, 1);             \
    *((CppType**)Data_custom_val(_v)) = ptr;                                 \
    _some = caml_alloc(1, 0);                                                 \
    Store_field(_some, 0, _v);                                               \
    CAMLreturn(_some);                                                        \
  }                                                                           \
  CppType* unwrap_##name(value v) {                                          \
    return *((CppType**)Data_custom_val(v));                                  \
  }

IMPL_SVF_OWNED_TYPE(mta,           MTA)
IMPL_SVF_OWNED_TYPE(mhp,           MHP)
IMPL_SVF_OWNED_TYPE(lock_analysis, LockAnalysis)
IMPL_SVF_OWNED_TYPE(tct,           TCT)

/* =========================================================================
 * PointsTo — user-owned (heap-allocated, has a real finalizer)
 * ========================================================================= */
static void points_to_finalize(value v) {
    PointsTo** pp = (PointsTo**)Data_custom_val(v);
    delete *pp;
    *pp = nullptr;
}
struct custom_operations points_to_ops = {
    "svf_ocaml.points_to",
    points_to_finalize,
    custom_compare_default, custom_hash_default,
    custom_serialize_default, custom_deserialize_default,
    custom_compare_ext_default, custom_fixed_length_default
};
value wrap_points_to_owned(PointsTo* ptr) {
    CAMLparam0(); CAMLlocal1(v);
    v = caml_alloc_custom(&points_to_ops, sizeof(PointsTo*), 0, 1);
    *((PointsTo**)Data_custom_val(v)) = ptr;
    CAMLreturn(v);
}
PointsTo* unwrap_points_to(value v) {
    return *((PointsTo**)Data_custom_val(v));
}

/* =========================================================================
 * IntervalValue — user-owned
 * ========================================================================= */
static void interval_value_finalize(value v) {
    IntervalValue** pp = (IntervalValue**)Data_custom_val(v);
    delete *pp;
    *pp = nullptr;
}
struct custom_operations interval_value_ops = {
    "svf_ocaml.interval_value",
    interval_value_finalize,
    custom_compare_default, custom_hash_default,
    custom_serialize_default, custom_deserialize_default,
    custom_compare_ext_default, custom_fixed_length_default
};
value wrap_interval_value(IntervalValue* ptr) {
    CAMLparam0(); CAMLlocal1(v);
    v = caml_alloc_custom(&interval_value_ops, sizeof(IntervalValue*), 0, 1);
    *((IntervalValue**)Data_custom_val(v)) = ptr;
    CAMLreturn(v);
}
IntervalValue* unwrap_interval_value(value v) {
    return *((IntervalValue**)Data_custom_val(v));
}

/* =========================================================================
 * AbstractValue — user-owned
 * ========================================================================= */
static void abstract_value_finalize(value v) {
    AbstractValue** pp = (AbstractValue**)Data_custom_val(v);
    delete *pp;
    *pp = nullptr;
}
struct custom_operations abstract_value_ops = {
    "svf_ocaml.abstract_value",
    abstract_value_finalize,
    custom_compare_default, custom_hash_default,
    custom_serialize_default, custom_deserialize_default,
    custom_compare_ext_default, custom_fixed_length_default
};
value wrap_abstract_value(AbstractValue* ptr) {
    CAMLparam0(); CAMLlocal1(v);
    v = caml_alloc_custom(&abstract_value_ops, sizeof(AbstractValue*), 0, 1);
    *((AbstractValue**)Data_custom_val(v)) = ptr;
    CAMLreturn(v);
}
AbstractValue* unwrap_abstract_value(value v) {
    return *((AbstractValue**)Data_custom_val(v));
}

/* =========================================================================
 * AddressValue — user-owned
 * ========================================================================= */
static void address_value_finalize(value v) {
    AddressValue** pp = (AddressValue**)Data_custom_val(v);
    delete *pp;
    *pp = nullptr;
}
struct custom_operations address_value_ops = {
    "svf_ocaml.address_value",
    address_value_finalize,
    custom_compare_default, custom_hash_default,
    custom_serialize_default, custom_deserialize_default,
    custom_compare_ext_default, custom_fixed_length_default
};
value wrap_address_value(AddressValue* ptr) {
    CAMLparam0(); CAMLlocal1(v);
    v = caml_alloc_custom(&address_value_ops, sizeof(AddressValue*), 0, 1);
    *((AddressValue**)Data_custom_val(v)) = ptr;
    CAMLreturn(v);
}
AddressValue* unwrap_address_value(value v) {
    return *((AddressValue**)Data_custom_val(v));
}

/* =========================================================================
 * BoundedInt — user-owned
 * ========================================================================= */
static void bounded_int_finalize(value v) {
    BoundedInt** pp = (BoundedInt**)Data_custom_val(v);
    delete *pp;
    *pp = nullptr;
}
struct custom_operations bounded_int_ops = {
    "svf_ocaml.bounded_int",
    bounded_int_finalize,
    custom_compare_default, custom_hash_default,
    custom_serialize_default, custom_deserialize_default,
    custom_compare_ext_default, custom_fixed_length_default
};
value wrap_bounded_int(BoundedInt* ptr) {
    CAMLparam0(); CAMLlocal1(v);
    v = caml_alloc_custom(&bounded_int_ops, sizeof(BoundedInt*), 0, 1);
    *((BoundedInt**)Data_custom_val(v)) = ptr;
    CAMLreturn(v);
}
BoundedInt* unwrap_bounded_int(value v) {
    return *((BoundedInt**)Data_custom_val(v));
}

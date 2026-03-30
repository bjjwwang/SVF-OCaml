/**
 * svf_ocaml_types.h
 *
 * Declarations of all SVF-OCaml custom block operations and
 * wrap/unwrap functions for SVF C++ pointer types.
 *
 * All definitions live in svf_types_stubs.cpp (one definition per symbol).
 * Every other stub file includes this header instead of using DEFINE_SVF_TYPE.
 */

#pragma once

extern "C" {
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/memory.h>
#include <caml/fail.h>
}

/* caml/compatibility.h (included by the above) defines macros that rename
 * common identifiers: initialize → caml_initialize, flush → caml_flush.
 * These clash with SVF C++ method names (AndersenBase::initialize, etc.).
 * Undefine them before any SVF headers are pulled in. */
#ifdef initialize
#  undef initialize
#endif
#ifdef flush
#  undef flush
#endif

/* Forward-declare SVF types so this header can be included without SVF headers */
namespace SVF {
  class SVFIR;
  class ICFG;
  class ICFGNode;
  class ICFGEdge;
  class IntraCFGEdge;
  class CallCFGEdge;
  class RetCFGEdge;
  class FunEntryICFGNode;
  class FunExitICFGNode;
  class CallICFGNode;
  class RetICFGNode;
  class SVFStmt;
  class SVFVar;
  class SVFType;
  class SVFBasicBlock;
  class CallGraph;
  class CallGraphNode;
  class CallGraphEdge;
  class SVFG;
  class VFG;
  class AndersenBase;
  class AndersenWaveDiff;
  class Steensgaard;
  class PointsTo;
  class ConstraintGraph;
  class MTA;
  class MHP;
  class LockAnalysis;
  class TCT;
  class AbstractState;
  class FunObjVar;
  class FunValVar;
  class BaseObjVar;
  class RetValPN;
  class IntervalValue;
}

/* =========================================================================
 * Declare wrap / unwrap functions for each type.
 * All have external linkage (defined in svf_types_stubs.cpp).
 * ========================================================================= */

#define DECL_SVF_WRAP(name, CppType) \
  value wrap_##name(CppType* ptr);         \
  value wrap_##name##_opt(CppType* ptr);   \
  CppType* unwrap_##name(value v)

DECL_SVF_WRAP(svfir,          SVF::SVFIR);
DECL_SVF_WRAP(icfg,           SVF::ICFG);
DECL_SVF_WRAP(icfg_node,      SVF::ICFGNode);
DECL_SVF_WRAP(icfg_edge,      SVF::ICFGEdge);
DECL_SVF_WRAP(fun_entry_node, SVF::FunEntryICFGNode);
DECL_SVF_WRAP(fun_exit_node,  SVF::FunExitICFGNode);
DECL_SVF_WRAP(call_icfg_node, SVF::CallICFGNode);
DECL_SVF_WRAP(ret_icfg_node,  SVF::RetICFGNode);
DECL_SVF_WRAP(svf_stmt,       SVF::SVFStmt);
DECL_SVF_WRAP(svf_var,        SVF::SVFVar);
DECL_SVF_WRAP(svf_type,       SVF::SVFType);
DECL_SVF_WRAP(svf_basic_block,SVF::SVFBasicBlock);
DECL_SVF_WRAP(call_graph,     SVF::CallGraph);
DECL_SVF_WRAP(call_graph_node,SVF::CallGraphNode);
DECL_SVF_WRAP(call_graph_edge,SVF::CallGraphEdge);
DECL_SVF_WRAP(svfg,           SVF::SVFG);
DECL_SVF_WRAP(andersen_base,  SVF::AndersenBase);
DECL_SVF_WRAP(andersen_wave_diff, SVF::AndersenWaveDiff);
DECL_SVF_WRAP(steensgaard,    SVF::Steensgaard);
DECL_SVF_WRAP(constraint_graph, SVF::ConstraintGraph);
DECL_SVF_WRAP(abstract_state, SVF::AbstractState);
DECL_SVF_WRAP(fun_obj_var,    SVF::FunObjVar);

/* PointsTo: owned by OCaml (has finalizer that deletes the heap-allocated PointsTo) */
value     wrap_points_to_owned(SVF::PointsTo* ptr);
SVF::PointsTo* unwrap_points_to(value v);

/* MTA-related (owned by OCaml) */
DECL_SVF_WRAP(mta,           SVF::MTA);
DECL_SVF_WRAP(mhp,           SVF::MHP);
DECL_SVF_WRAP(lock_analysis, SVF::LockAnalysis);
DECL_SVF_WRAP(tct,           SVF::TCT);

/* IntervalValue (owned by OCaml) */
value              wrap_interval_value(SVF::IntervalValue* ptr);
SVF::IntervalValue* unwrap_interval_value(value v);

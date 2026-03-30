/**
 * graphs_stubs.cpp
 *
 * OCaml C FFI stubs for graph structures:
 *   ICFG, ICFGNode, ICFGEdge, CallGraph, CallGraphNode, CallGraphEdge,
 *   SVFBasicBlock.
 * Mirrors SVF-Python/pybind/Graphs.cpp.
 */

#include "svf_ocaml_types.h"

#include "SVF-LLVM/SVFIRBuilder.h"
#include "Graphs/ICFG.h"
#include "Graphs/CallGraph.h"
#include "Graphs/ThreadCallGraph.h"
#include "SVFIR/SVFType.h"
#include "SVFIR/SVFStatements.h"
#include "SVFIR/SVFVariables.h"
#include "MemoryModel/PointerAnalysis.h"
#include "Util/SVFUtil.h"
#include "Util/Casting.h"

#include <sstream>
#include <vector>

using namespace SVF;

/* =========================================================================
 * ICFG bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_icfg_get_nodes(value v_icfg) {
    CAMLparam1(v_icfg);
    CAMLlocal1(result);
    ICFG* icfg = unwrap_icfg(v_icfg);
    std::vector<ICFGNode*> nodes;
    for (auto it = icfg->begin(); it != icfg->end(); ++it)
        nodes.push_back(it->second);
    result = Val_int(0);
    for (int i = (int)nodes.size() - 1; i >= 0; --i) {
        CAMLlocal2(head, cons);
        head = wrap_icfg_node(nodes[i]);
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, head);
        Store_field(cons, 1, result);
        result = cons;
    }
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_icfg_get_gnode(value v_icfg, value v_id) {
    CAMLparam2(v_icfg, v_id);
    ICFG* icfg = unwrap_icfg(v_icfg);
    ICFGNode* node = icfg->getICFGNode((NodeID)Int_val(v_id));
    if (!node) caml_failwith("icfg_get_gnode: node not found");
    CAMLreturn(wrap_icfg_node(node));
}

extern "C" CAMLprim value caml_icfg_get_node_num(value v_icfg) {
    CAMLparam1(v_icfg);
    CAMLreturn(Val_int((int)unwrap_icfg(v_icfg)->getTotalNodeNum()));
}

/* =========================================================================
 * ICFGNode bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_icfg_node_to_string(value v_node) {
    CAMLparam1(v_node);
    ICFGNode* node = unwrap_icfg_node(v_node);
    if (!node) CAMLreturn(caml_copy_string("nullptr"));
    std::ostringstream oss;
    oss << node->toString();
    CAMLreturn(caml_copy_string(oss.str().c_str()));
}

extern "C" CAMLprim value caml_icfg_node_get_id(value v_node) {
    CAMLparam1(v_node);
    CAMLreturn(Val_int((int)unwrap_icfg_node(v_node)->getId()));
}

extern "C" CAMLprim value caml_icfg_node_get_svf_stmts(value v_node) {
    CAMLparam1(v_node);
    CAMLlocal1(result);
    ICFGNode* node = unwrap_icfg_node(v_node);
    std::vector<SVFStmt*> stmts;
    for (const SVFStmt* s : node->getSVFStmts())
        stmts.push_back(const_cast<SVFStmt*>(s));
    result = Val_int(0);
    for (int i = (int)stmts.size() - 1; i >= 0; --i) {
        CAMLlocal2(head, cons);
        head = wrap_svf_stmt(stmts[i]);
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, head);
        Store_field(cons, 1, result);
        result = cons;
    }
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_icfg_node_get_out_edges(value v_node) {
    CAMLparam1(v_node);
    CAMLlocal1(result);
    ICFGNode* node = unwrap_icfg_node(v_node);
    std::vector<ICFGEdge*> edges;
    for (auto& e : node->getOutEdges()) edges.push_back(e);
    result = Val_int(0);
    for (int i = (int)edges.size() - 1; i >= 0; --i) {
        CAMLlocal2(head, cons);
        head = wrap_icfg_edge(edges[i]);
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, head);
        Store_field(cons, 1, result);
        result = cons;
    }
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_icfg_node_get_in_edges(value v_node) {
    CAMLparam1(v_node);
    CAMLlocal1(result);
    ICFGNode* node = unwrap_icfg_node(v_node);
    std::vector<ICFGEdge*> edges;
    for (auto& e : node->getInEdges()) edges.push_back(e);
    result = Val_int(0);
    for (int i = (int)edges.size() - 1; i >= 0; --i) {
        CAMLlocal2(head, cons);
        head = wrap_icfg_edge(edges[i]);
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, head);
        Store_field(cons, 1, result);
        result = cons;
    }
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_icfg_node_is_fun_entry(value v_node) {
    CAMLparam1(v_node);
    CAMLreturn(Val_bool(unwrap_icfg_node(v_node)->getNodeKind() == ICFGNode::FunEntryBlock));
}

extern "C" CAMLprim value caml_icfg_node_is_fun_exit(value v_node) {
    CAMLparam1(v_node);
    CAMLreturn(Val_bool(unwrap_icfg_node(v_node)->getNodeKind() == ICFGNode::FunExitBlock));
}

extern "C" CAMLprim value caml_icfg_node_is_call(value v_node) {
    CAMLparam1(v_node);
    CAMLreturn(Val_bool(unwrap_icfg_node(v_node)->getNodeKind() == ICFGNode::FunCallBlock));
}

extern "C" CAMLprim value caml_icfg_node_is_ret(value v_node) {
    CAMLparam1(v_node);
    CAMLreturn(Val_bool(unwrap_icfg_node(v_node)->getNodeKind() == ICFGNode::FunRetBlock));
}

extern "C" CAMLprim value caml_icfg_node_get_fun_name(value v_node) {
    CAMLparam1(v_node);
    const FunObjVar* fun = unwrap_icfg_node(v_node)->getFun();
    if (!fun) CAMLreturn(caml_copy_string(""));
    CAMLreturn(caml_copy_string(fun->getName().c_str()));
}

extern "C" CAMLprim value caml_icfg_node_as_fun_entry(value v_node) {
    CAMLparam1(v_node);
    FunEntryICFGNode* e = SVFUtil::dyn_cast<FunEntryICFGNode>(unwrap_icfg_node(v_node));
    CAMLreturn(wrap_fun_entry_node_opt(e));
}

extern "C" CAMLprim value caml_icfg_node_as_call(value v_node) {
    CAMLparam1(v_node);
    CallICFGNode* c = SVFUtil::dyn_cast<CallICFGNode>(unwrap_icfg_node(v_node));
    CAMLreturn(wrap_call_icfg_node_opt(c));
}

/* =========================================================================
 * ICFGEdge bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_icfg_edge_to_string(value v_edge) {
    CAMLparam1(v_edge);
    ICFGEdge* e = unwrap_icfg_edge(v_edge);
    if (!e) CAMLreturn(caml_copy_string("nullptr"));
    std::ostringstream oss;
    if (e->getSrcNode()) oss << e->getSrcNode()->toString(); else oss << "nullptr";
    oss << " -> ";
    if (e->getDstNode()) oss << e->getDstNode()->toString(); else oss << "nullptr";
    CAMLreturn(caml_copy_string(oss.str().c_str()));
}

extern "C" CAMLprim value caml_icfg_edge_is_cfg_edge(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_icfg_edge(v_e)->isCFGEdge()));
}

extern "C" CAMLprim value caml_icfg_edge_is_call_cfg_edge(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_icfg_edge(v_e)->isCallCFGEdge()));
}

extern "C" CAMLprim value caml_icfg_edge_is_ret_cfg_edge(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_icfg_edge(v_e)->isRetCFGEdge()));
}

extern "C" CAMLprim value caml_icfg_edge_is_intra_cfg_edge(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_icfg_edge(v_e)->isIntraCFGEdge()));
}

extern "C" CAMLprim value caml_icfg_edge_get_src_node(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(wrap_icfg_node(unwrap_icfg_edge(v_e)->getSrcNode()));
}

extern "C" CAMLprim value caml_icfg_edge_get_dst_node(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(wrap_icfg_node(unwrap_icfg_edge(v_e)->getDstNode()));
}

/* =========================================================================
 * CallGraph bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_call_graph_get_nodes(value v_cg) {
    CAMLparam1(v_cg);
    CAMLlocal1(result);
    CallGraph* cg = unwrap_call_graph(v_cg);
    std::vector<CallGraphNode*> nodes;
    for (auto it = cg->begin(); it != cg->end(); ++it)
        nodes.push_back(it->second);
    result = Val_int(0);
    for (int i = (int)nodes.size() - 1; i >= 0; --i) {
        CAMLlocal2(head, cons);
        head = wrap_call_graph_node(nodes[i]);
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, head);
        Store_field(cons, 1, result);
        result = cons;
    }
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_call_graph_get_gnode(value v_cg, value v_id) {
    CAMLparam2(v_cg, v_id);
    CallGraphNode* node = unwrap_call_graph(v_cg)->getGNode((NodeID)Int_val(v_id));
    if (!node) caml_failwith("call_graph_get_gnode: node not found");
    CAMLreturn(wrap_call_graph_node(node));
}

/* =========================================================================
 * CallGraphNode bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_call_graph_node_to_string(value v_n) {
    CAMLparam1(v_n);
    std::ostringstream oss;
    oss << unwrap_call_graph_node(v_n)->toString();
    CAMLreturn(caml_copy_string(oss.str().c_str()));
}

extern "C" CAMLprim value caml_call_graph_node_get_id(value v_n) {
    CAMLparam1(v_n);
    CAMLreturn(Val_int((int)unwrap_call_graph_node(v_n)->getId()));
}

extern "C" CAMLprim value caml_call_graph_node_get_name(value v_n) {
    CAMLparam1(v_n);
    CAMLreturn(caml_copy_string(unwrap_call_graph_node(v_n)->getName().c_str()));
}

extern "C" CAMLprim value caml_call_graph_node_get_out_edges(value v_n) {
    CAMLparam1(v_n);
    CAMLlocal1(result);
    CallGraphNode* node = unwrap_call_graph_node(v_n);
    std::vector<CallGraphEdge*> edges;
    for (auto& e : node->getOutEdges()) edges.push_back(e);
    result = Val_int(0);
    for (int i = (int)edges.size() - 1; i >= 0; --i) {
        CAMLlocal2(head, cons);
        head = wrap_call_graph_edge(edges[i]);
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, head);
        Store_field(cons, 1, result);
        result = cons;
    }
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_call_graph_node_get_in_edges(value v_n) {
    CAMLparam1(v_n);
    CAMLlocal1(result);
    CallGraphNode* node = unwrap_call_graph_node(v_n);
    std::vector<CallGraphEdge*> edges;
    for (auto& e : node->getInEdges()) edges.push_back(e);
    result = Val_int(0);
    for (int i = (int)edges.size() - 1; i >= 0; --i) {
        CAMLlocal2(head, cons);
        head = wrap_call_graph_edge(edges[i]);
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, head);
        Store_field(cons, 1, result);
        result = cons;
    }
    CAMLreturn(result);
}

/* =========================================================================
 * CallGraphEdge bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_call_graph_edge_get_src(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(wrap_call_graph_node(unwrap_call_graph_edge(v_e)->getSrcNode()));
}

extern "C" CAMLprim value caml_call_graph_edge_get_dst(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(wrap_call_graph_node(unwrap_call_graph_edge(v_e)->getDstNode()));
}

/* =========================================================================
 * SVFBasicBlock bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_svf_basic_block_get_name(value v_bb) {
    CAMLparam1(v_bb);
    CAMLreturn(caml_copy_string(unwrap_svf_basic_block(v_bb)->getName().c_str()));
}

/**
 * graphs_stubs.cpp
 *
 * OCaml C FFI stubs for graph structures:
 *   ICFG, ICFGNode, ICFGEdge, CallGraph, CallGraphNode, CallGraphEdge,
 *   SVFBasicBlock, VFG, VFGNode, VFGEdge, SVFG.
 * Mirrors SVF-Python/pybind/Graphs.cpp.
 */

#include "svf_ocaml_types.h"

#include "SVF-LLVM/SVFIRBuilder.h"
#include "Graphs/ICFG.h"
#include "Graphs/ICFGNode.h"
#include "Graphs/ICFGEdge.h"
#include "Graphs/CallGraph.h"
#include "Graphs/ThreadCallGraph.h"
#include "Graphs/VFG.h"
#include "Graphs/VFGNode.h"
#include "Graphs/VFGEdge.h"
#include "Graphs/SVFG.h"
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
 * Helper: build an OCaml list from a vector
 * ========================================================================= */
#define BUILD_LIST_FROM_VEC(vec, wrap_fn, result) \
    do { \
        result = Val_int(0); \
        for (int _i = (int)(vec).size() - 1; _i >= 0; --_i) { \
            CAMLlocal2(_head, _cons); \
            _head = wrap_fn((vec)[_i]); \
            _cons = caml_alloc(2, 0); \
            Store_field(_cons, 0, _head); \
            Store_field(_cons, 1, result); \
            result = _cons; \
        } \
    } while(0)

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
    BUILD_LIST_FROM_VEC(nodes, wrap_icfg_node, result);
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

extern "C" CAMLprim value caml_icfg_get_global_icfg_node(value v_icfg) {
    CAMLparam1(v_icfg);
    GlobalICFGNode* g = unwrap_icfg(v_icfg)->getGlobalICFGNode();
    if (!g) caml_failwith("icfg_get_global_icfg_node: null");
    CAMLreturn(wrap_icfg_node(static_cast<ICFGNode*>(g)));
}

extern "C" CAMLprim value caml_icfg_get_fun_entry_icfg_node(value v_icfg, value v_fun) {
    CAMLparam2(v_icfg, v_fun);
    FunEntryICFGNode* e = unwrap_icfg(v_icfg)->getFunEntryICFGNode(unwrap_fun_obj_var(v_fun));
    if (!e) caml_failwith("icfg_get_fun_entry_icfg_node: null");
    CAMLreturn(wrap_icfg_node(static_cast<ICFGNode*>(e)));
}

extern "C" CAMLprim value caml_icfg_dump(value v_icfg, value v_file) {
    CAMLparam2(v_icfg, v_file);
    unwrap_icfg(v_icfg)->dump(std::string(String_val(v_file)));
    CAMLreturn(Val_unit);
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
    BUILD_LIST_FROM_VEC(stmts, wrap_svf_stmt, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_icfg_node_get_out_edges(value v_node) {
    CAMLparam1(v_node);
    CAMLlocal1(result);
    ICFGNode* node = unwrap_icfg_node(v_node);
    std::vector<ICFGEdge*> edges;
    for (auto& e : node->getOutEdges()) edges.push_back(e);
    BUILD_LIST_FROM_VEC(edges, wrap_icfg_edge, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_icfg_node_get_in_edges(value v_node) {
    CAMLparam1(v_node);
    CAMLlocal1(result);
    ICFGNode* node = unwrap_icfg_node(v_node);
    std::vector<ICFGEdge*> edges;
    for (auto& e : node->getInEdges()) edges.push_back(e);
    BUILD_LIST_FROM_VEC(edges, wrap_icfg_edge, result);
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

extern "C" CAMLprim value caml_icfg_node_get_fun(value v_node) {
    CAMLparam1(v_node);
    const FunObjVar* fun = unwrap_icfg_node(v_node)->getFun();
    CAMLreturn(wrap_fun_obj_var_opt(const_cast<FunObjVar*>(fun)));
}

extern "C" CAMLprim value caml_icfg_node_get_bb(value v_node) {
    CAMLparam1(v_node);
    const SVFBasicBlock* bb = unwrap_icfg_node(v_node)->getBB();
    CAMLreturn(wrap_svf_basic_block_opt(const_cast<SVFBasicBlock*>(bb)));
}

extern "C" CAMLprim value caml_icfg_node_get_source_loc(value v_node) {
    CAMLparam1(v_node);
    std::string loc = unwrap_icfg_node(v_node)->getSourceLoc();
    CAMLreturn(caml_copy_string(loc.c_str()));
}

extern "C" CAMLprim value caml_icfg_node_as_fun_entry(value v_node) {
    CAMLparam1(v_node);
    FunEntryICFGNode* e = SVFUtil::dyn_cast<FunEntryICFGNode>(unwrap_icfg_node(v_node));
    CAMLreturn(wrap_icfg_node_opt(static_cast<ICFGNode*>(e)));
}

extern "C" CAMLprim value caml_icfg_node_as_fun_exit(value v_node) {
    CAMLparam1(v_node);
    FunExitICFGNode* e = SVFUtil::dyn_cast<FunExitICFGNode>(unwrap_icfg_node(v_node));
    CAMLreturn(wrap_icfg_node_opt(static_cast<ICFGNode*>(e)));
}

extern "C" CAMLprim value caml_icfg_node_as_call(value v_node) {
    CAMLparam1(v_node);
    CallICFGNode* c = SVFUtil::dyn_cast<CallICFGNode>(unwrap_icfg_node(v_node));
    CAMLreturn(wrap_icfg_node_opt(static_cast<ICFGNode*>(c)));
}

extern "C" CAMLprim value caml_icfg_node_as_ret(value v_node) {
    CAMLparam1(v_node);
    RetICFGNode* r = SVFUtil::dyn_cast<RetICFGNode>(unwrap_icfg_node(v_node));
    CAMLreturn(wrap_icfg_node_opt(static_cast<ICFGNode*>(r)));
}

/* IntraICFGNode */
extern "C" CAMLprim value caml_intra_icfg_node_is_ret_inst(value v_node) {
    CAMLparam1(v_node);
    IntraICFGNode* n = SVFUtil::dyn_cast<IntraICFGNode>(unwrap_icfg_node(v_node));
    if (!n) CAMLreturn(Val_bool(false));
    CAMLreturn(Val_bool(n->isRetInst()));
}

/* FunEntryICFGNode methods */
extern "C" CAMLprim value caml_fun_entry_icfg_node_get_formal_parms(value v_node) {
    CAMLparam1(v_node);
    CAMLlocal1(result);
    FunEntryICFGNode* e = SVFUtil::dyn_cast<FunEntryICFGNode>(unwrap_icfg_node(v_node));
    if (!e) CAMLreturn(Val_int(0));
    std::vector<SVFVar*> vars;
    for (const SVFVar* v : e->getFormalParms())
        vars.push_back(const_cast<SVFVar*>(v));
    BUILD_LIST_FROM_VEC(vars, wrap_svf_var, result);
    CAMLreturn(result);
}

/* FunExitICFGNode methods */
extern "C" CAMLprim value caml_fun_exit_icfg_node_get_formal_ret(value v_node) {
    CAMLparam1(v_node);
    FunExitICFGNode* e = SVFUtil::dyn_cast<FunExitICFGNode>(unwrap_icfg_node(v_node));
    if (!e) caml_failwith("not a FunExitICFGNode");
    const SVFVar* r = e->getFormalRet();
    if (!r) caml_failwith("fun_exit_icfg_node_get_formal_ret: null");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(r)));
}

/* CallICFGNode methods */
extern "C" CAMLprim value caml_call_icfg_node_get_called_function(value v_node) {
    CAMLparam1(v_node);
    CallICFGNode* c = SVFUtil::dyn_cast<CallICFGNode>(unwrap_icfg_node(v_node));
    if (!c) CAMLreturn(Val_int(0));
    const FunObjVar* fun = c->getCalledFunction();
    CAMLreturn(wrap_fun_obj_var_opt(const_cast<FunObjVar*>(fun)));
}

extern "C" CAMLprim value caml_call_icfg_node_get_actual_parms(value v_node) {
    CAMLparam1(v_node);
    CAMLlocal1(result);
    CallICFGNode* c = SVFUtil::dyn_cast<CallICFGNode>(unwrap_icfg_node(v_node));
    if (!c) CAMLreturn(Val_int(0));
    std::vector<SVFVar*> vars;
    for (const SVFVar* v : c->getActualParms())
        vars.push_back(const_cast<SVFVar*>(v));
    BUILD_LIST_FROM_VEC(vars, wrap_svf_var, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_call_icfg_node_get_argument(value v_node, value v_idx) {
    CAMLparam2(v_node, v_idx);
    CallICFGNode* c = SVFUtil::dyn_cast<CallICFGNode>(unwrap_icfg_node(v_node));
    if (!c) caml_failwith("not a CallICFGNode");
    const SVFVar* arg = c->getArgument((u32_t)Int_val(v_idx));
    if (!arg) caml_failwith("call_icfg_node_get_argument: null");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(arg)));
}

extern "C" CAMLprim value caml_call_icfg_node_is_var_arg(value v_node) {
    CAMLparam1(v_node);
    CallICFGNode* c = SVFUtil::dyn_cast<CallICFGNode>(unwrap_icfg_node(v_node));
    if (!c) CAMLreturn(Val_bool(false));
    CAMLreturn(Val_bool(c->isVarArg()));
}

extern "C" CAMLprim value caml_call_icfg_node_is_virtual_call(value v_node) {
    CAMLparam1(v_node);
    CallICFGNode* c = SVFUtil::dyn_cast<CallICFGNode>(unwrap_icfg_node(v_node));
    if (!c) CAMLreturn(Val_bool(false));
    CAMLreturn(Val_bool(c->isVirtualCall()));
}

extern "C" CAMLprim value caml_call_icfg_node_get_ret_icfg_node(value v_node) {
    CAMLparam1(v_node);
    CallICFGNode* c = SVFUtil::dyn_cast<CallICFGNode>(unwrap_icfg_node(v_node));
    if (!c) caml_failwith("not a CallICFGNode");
    const RetICFGNode* r = c->getRetICFGNode();
    if (!r) caml_failwith("call_icfg_node_get_ret_icfg_node: null");
    CAMLreturn(wrap_icfg_node(const_cast<ICFGNode*>(static_cast<const ICFGNode*>(r))));
}

extern "C" CAMLprim value caml_call_icfg_node_get_caller_entry(value v_node) {
    CAMLparam1(v_node);
    CallICFGNode* c = SVFUtil::dyn_cast<CallICFGNode>(unwrap_icfg_node(v_node));
    if (!c) caml_failwith("not a CallICFGNode");
    const FunObjVar* caller = c->getCaller();
    if (!caller) caml_failwith("call_icfg_node_get_caller_entry: null");
    /* getCaller returns FunObjVar*, use getFun() context - the caller's entry node */
    /* Actually getCaller() returns FunObjVar* - wrap as fun_obj_var */
    CAMLreturn(wrap_fun_obj_var(const_cast<FunObjVar*>(caller)));
}

/* RetICFGNode methods */
extern "C" CAMLprim value caml_ret_icfg_node_get_actual_ret(value v_node) {
    CAMLparam1(v_node);
    RetICFGNode* r = SVFUtil::dyn_cast<RetICFGNode>(unwrap_icfg_node(v_node));
    if (!r) caml_failwith("not a RetICFGNode");
    const SVFVar* ar = r->getActualRet();
    if (!ar) caml_failwith("ret_icfg_node_get_actual_ret: null");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(ar)));
}

extern "C" CAMLprim value caml_ret_icfg_node_get_call_icfg_node(value v_node) {
    CAMLparam1(v_node);
    RetICFGNode* r = SVFUtil::dyn_cast<RetICFGNode>(unwrap_icfg_node(v_node));
    if (!r) caml_failwith("not a RetICFGNode");
    const CallICFGNode* c = r->getCallICFGNode();
    if (!c) caml_failwith("ret_icfg_node_get_call_icfg_node: null");
    CAMLreturn(wrap_icfg_node(const_cast<ICFGNode*>(static_cast<const ICFGNode*>(c))));
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

extern "C" CAMLprim value caml_icfg_edge_get_src_id(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_int((int)unwrap_icfg_edge(v_e)->getSrcID()));
}

extern "C" CAMLprim value caml_icfg_edge_get_dst_id(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_int((int)unwrap_icfg_edge(v_e)->getDstID()));
}

extern "C" CAMLprim value caml_icfg_edge_as_intra(value v_e) {
    CAMLparam1(v_e);
    IntraCFGEdge* ie = SVFUtil::dyn_cast<IntraCFGEdge>(unwrap_icfg_edge(v_e));
    CAMLreturn(wrap_icfg_edge_opt(static_cast<ICFGEdge*>(ie)));
}

extern "C" CAMLprim value caml_icfg_edge_as_call(value v_e) {
    CAMLparam1(v_e);
    CallCFGEdge* ce = SVFUtil::dyn_cast<CallCFGEdge>(unwrap_icfg_edge(v_e));
    CAMLreturn(wrap_icfg_edge_opt(static_cast<ICFGEdge*>(ce)));
}

extern "C" CAMLprim value caml_icfg_edge_as_ret(value v_e) {
    CAMLparam1(v_e);
    RetCFGEdge* re = SVFUtil::dyn_cast<RetCFGEdge>(unwrap_icfg_edge(v_e));
    CAMLreturn(wrap_icfg_edge_opt(static_cast<ICFGEdge*>(re)));
}

/* IntraCFGEdge */
extern "C" CAMLprim value caml_intra_cfg_edge_get_condition(value v_e) {
    CAMLparam1(v_e);
    IntraCFGEdge* ie = SVFUtil::dyn_cast<IntraCFGEdge>(unwrap_icfg_edge(v_e));
    if (!ie) CAMLreturn(Val_int(0));
    const SVFVar* cond = ie->getCondition();
    CAMLreturn(wrap_svf_var_opt(const_cast<SVFVar*>(cond)));
}

/* CallCFGEdge */
extern "C" CAMLprim value caml_call_cfg_edge_get_call_site(value v_e) {
    CAMLparam1(v_e);
    CallCFGEdge* ce = SVFUtil::dyn_cast<CallCFGEdge>(unwrap_icfg_edge(v_e));
    if (!ce) caml_failwith("not a CallCFGEdge");
    const CallICFGNode* cs = ce->getCallSite();
    if (!cs) caml_failwith("call_cfg_edge_get_call_site: null");
    CAMLreturn(wrap_icfg_node(const_cast<ICFGNode*>(static_cast<const ICFGNode*>(cs))));
}

extern "C" CAMLprim value caml_call_cfg_edge_get_call_pes(value v_e) {
    CAMLparam1(v_e);
    CAMLlocal1(result);
    CallCFGEdge* ce = SVFUtil::dyn_cast<CallCFGEdge>(unwrap_icfg_edge(v_e));
    if (!ce) CAMLreturn(Val_int(0));
    std::vector<SVFStmt*> stmts;
    for (const CallPE* pe : ce->getCallPEs())
        stmts.push_back(const_cast<SVFStmt*>(static_cast<const SVFStmt*>(pe)));
    BUILD_LIST_FROM_VEC(stmts, wrap_svf_stmt, result);
    CAMLreturn(result);
}

/* RetCFGEdge */
extern "C" CAMLprim value caml_ret_cfg_edge_get_call_site(value v_e) {
    CAMLparam1(v_e);
    RetCFGEdge* re = SVFUtil::dyn_cast<RetCFGEdge>(unwrap_icfg_edge(v_e));
    if (!re) caml_failwith("not a RetCFGEdge");
    const CallICFGNode* cs = re->getCallSite();
    if (!cs) caml_failwith("ret_cfg_edge_get_call_site: null");
    CAMLreturn(wrap_icfg_node(const_cast<ICFGNode*>(static_cast<const ICFGNode*>(cs))));
}

extern "C" CAMLprim value caml_ret_cfg_edge_get_ret_pe(value v_e) {
    CAMLparam1(v_e);
    RetCFGEdge* re = SVFUtil::dyn_cast<RetCFGEdge>(unwrap_icfg_edge(v_e));
    if (!re) caml_failwith("not a RetCFGEdge");
    const RetPE* pe = re->getRetPE();
    if (!pe) caml_failwith("ret_cfg_edge_get_ret_pe: null");
    CAMLreturn(wrap_svf_stmt(const_cast<SVFStmt*>(static_cast<const SVFStmt*>(pe))));
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
    BUILD_LIST_FROM_VEC(nodes, wrap_call_graph_node, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_call_graph_get_gnode(value v_cg, value v_id) {
    CAMLparam2(v_cg, v_id);
    CallGraphNode* node = unwrap_call_graph(v_cg)->getGNode((NodeID)Int_val(v_id));
    if (!node) caml_failwith("call_graph_get_gnode: node not found");
    CAMLreturn(wrap_call_graph_node(node));
}

extern "C" CAMLprim value caml_call_graph_dump(value v_cg, value v_file) {
    CAMLparam2(v_cg, v_file);
    unwrap_call_graph(v_cg)->dump(std::string(String_val(v_file)));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_call_graph_view(value v_cg) {
    CAMLparam1(v_cg);
    unwrap_call_graph(v_cg)->view();
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_call_graph_get_node_by_name(value v_cg, value v_name) {
    CAMLparam2(v_cg, v_name);
    const CallGraphNode* n = unwrap_call_graph(v_cg)->getCallGraphNode(std::string(String_val(v_name)));
    CAMLreturn(wrap_call_graph_node_opt(const_cast<CallGraphNode*>(n)));
}

/* ThreadCallGraph bindings (reuse call_graph type) */
extern "C" CAMLprim value caml_thread_call_graph_get_fork_sites(value v_tcg) {
    CAMLparam1(v_tcg);
    CAMLlocal1(result);
    ThreadCallGraph* tcg = SVFUtil::dyn_cast<ThreadCallGraph>(unwrap_call_graph(v_tcg));
    if (!tcg) CAMLreturn(Val_int(0));
    std::vector<ICFGNode*> nodes;
    for (auto it = tcg->forksitesBegin(); it != tcg->forksitesEnd(); ++it)
        nodes.push_back(const_cast<ICFGNode*>(static_cast<const ICFGNode*>(*it)));
    BUILD_LIST_FROM_VEC(nodes, wrap_icfg_node, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_thread_call_graph_get_fork_edges(value v_tcg) {
    CAMLparam1(v_tcg);
    CAMLlocal1(result);
    ThreadCallGraph* tcg = SVFUtil::dyn_cast<ThreadCallGraph>(unwrap_call_graph(v_tcg));
    if (!tcg) CAMLreturn(Val_int(0));
    std::vector<CallGraphEdge*> edges;
    for (auto sit = tcg->forksitesBegin(); sit != tcg->forksitesEnd(); ++sit) {
        for (auto eit = tcg->getForkEdgeBegin(*sit); eit != tcg->getForkEdgeEnd(*sit); ++eit)
            edges.push_back(const_cast<CallGraphEdge*>(static_cast<const CallGraphEdge*>(*eit)));
    }
    BUILD_LIST_FROM_VEC(edges, wrap_call_graph_edge, result);
    CAMLreturn(result);
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
    BUILD_LIST_FROM_VEC(edges, wrap_call_graph_edge, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_call_graph_node_get_in_edges(value v_n) {
    CAMLparam1(v_n);
    CAMLlocal1(result);
    CallGraphNode* node = unwrap_call_graph_node(v_n);
    std::vector<CallGraphEdge*> edges;
    for (auto& e : node->getInEdges()) edges.push_back(e);
    BUILD_LIST_FROM_VEC(edges, wrap_call_graph_edge, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_call_graph_node_is_reachable_from_prog_entry(value v_n) {
    CAMLparam1(v_n);
    CallGraphNode* node = unwrap_call_graph_node(v_n);
    Map<NodeID, bool> reachableMap;
    NodeBS visitedNodes;
    CAMLreturn(Val_bool(node->isReachableFromProgEntry(reachableMap, visitedNodes)));
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

extern "C" CAMLprim value caml_call_graph_edge_get_call_site_id(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_int((int)unwrap_call_graph_edge(v_e)->getCallSiteID()));
}

extern "C" CAMLprim value caml_call_graph_edge_get_src_id(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_int((int)unwrap_call_graph_edge(v_e)->getSrcID()));
}

extern "C" CAMLprim value caml_call_graph_edge_get_dst_id(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_int((int)unwrap_call_graph_edge(v_e)->getDstID()));
}

extern "C" CAMLprim value caml_call_graph_edge_is_direct(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_call_graph_edge(v_e)->isDirectCallEdge()));
}

extern "C" CAMLprim value caml_call_graph_edge_is_indirect(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_call_graph_edge(v_e)->isIndirectCallEdge()));
}

extern "C" CAMLprim value caml_call_graph_edge_get_direct_calls(value v_e) {
    CAMLparam1(v_e);
    CAMLlocal1(result);
    CallGraphEdge* e = unwrap_call_graph_edge(v_e);
    std::vector<ICFGNode*> nodes;
    for (const CallICFGNode* cs : e->getDirectCalls())
        nodes.push_back(const_cast<ICFGNode*>(static_cast<const ICFGNode*>(cs)));
    BUILD_LIST_FROM_VEC(nodes, wrap_icfg_node, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_call_graph_edge_get_indirect_calls(value v_e) {
    CAMLparam1(v_e);
    CAMLlocal1(result);
    CallGraphEdge* e = unwrap_call_graph_edge(v_e);
    std::vector<ICFGNode*> nodes;
    for (const CallICFGNode* cs : e->getIndirectCalls())
        nodes.push_back(const_cast<ICFGNode*>(static_cast<const ICFGNode*>(cs)));
    BUILD_LIST_FROM_VEC(nodes, wrap_icfg_node, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_call_graph_edge_to_string(value v_e) {
    CAMLparam1(v_e);
    std::ostringstream oss;
    oss << unwrap_call_graph_edge(v_e)->toString();
    CAMLreturn(caml_copy_string(oss.str().c_str()));
}

/* =========================================================================
 * SVFBasicBlock bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_svf_basic_block_get_name(value v_bb) {
    CAMLparam1(v_bb);
    CAMLreturn(caml_copy_string(unwrap_svf_basic_block(v_bb)->getName().c_str()));
}

extern "C" CAMLprim value caml_svf_basic_block_get_id(value v_bb) {
    CAMLparam1(v_bb);
    CAMLreturn(Val_int((int)unwrap_svf_basic_block(v_bb)->getId()));
}

extern "C" CAMLprim value caml_svf_basic_block_get_icfg_node_list(value v_bb) {
    CAMLparam1(v_bb);
    CAMLlocal1(result);
    SVFBasicBlock* bb = unwrap_svf_basic_block(v_bb);
    std::vector<ICFGNode*> nodes;
    for (const ICFGNode* n : bb->getICFGNodeList())
        nodes.push_back(const_cast<ICFGNode*>(n));
    BUILD_LIST_FROM_VEC(nodes, wrap_icfg_node, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_svf_basic_block_get_successors(value v_bb) {
    CAMLparam1(v_bb);
    CAMLlocal1(result);
    SVFBasicBlock* bb = unwrap_svf_basic_block(v_bb);
    std::vector<SVFBasicBlock*> succs;
    for (const SVFBasicBlock* s : bb->getSuccessors())
        succs.push_back(const_cast<SVFBasicBlock*>(s));
    BUILD_LIST_FROM_VEC(succs, wrap_svf_basic_block, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_svf_basic_block_get_predecessors(value v_bb) {
    CAMLparam1(v_bb);
    CAMLlocal1(result);
    SVFBasicBlock* bb = unwrap_svf_basic_block(v_bb);
    std::vector<SVFBasicBlock*> preds;
    for (const SVFBasicBlock* p : bb->getPredecessors())
        preds.push_back(const_cast<SVFBasicBlock*>(p));
    BUILD_LIST_FROM_VEC(preds, wrap_svf_basic_block, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_svf_basic_block_get_num_successors(value v_bb) {
    CAMLparam1(v_bb);
    CAMLreturn(Val_int((int)unwrap_svf_basic_block(v_bb)->getNumSuccessors()));
}

/* =========================================================================
 * VFGNode bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_vfg_node_to_string(value v_n) {
    CAMLparam1(v_n);
    std::ostringstream oss;
    oss << unwrap_vfg_node(v_n)->toString();
    CAMLreturn(caml_copy_string(oss.str().c_str()));
}

extern "C" CAMLprim value caml_vfg_node_get_id(value v_n) {
    CAMLparam1(v_n);
    CAMLreturn(Val_int((int)unwrap_vfg_node(v_n)->getId()));
}

extern "C" CAMLprim value caml_vfg_node_get_icfg_node(value v_n) {
    CAMLparam1(v_n);
    const ICFGNode* icfg_n = unwrap_vfg_node(v_n)->getICFGNode();
    CAMLreturn(wrap_icfg_node_opt(const_cast<ICFGNode*>(icfg_n)));
}

extern "C" CAMLprim value caml_vfg_node_get_out_edges(value v_n) {
    CAMLparam1(v_n);
    CAMLlocal1(result);
    VFGNode* n = unwrap_vfg_node(v_n);
    std::vector<VFGEdge*> edges;
    for (auto& e : n->getOutEdges()) edges.push_back(e);
    BUILD_LIST_FROM_VEC(edges, wrap_vfg_edge, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_vfg_node_get_in_edges(value v_n) {
    CAMLparam1(v_n);
    CAMLlocal1(result);
    VFGNode* n = unwrap_vfg_node(v_n);
    std::vector<VFGEdge*> edges;
    for (auto& e : n->getInEdges()) edges.push_back(e);
    BUILD_LIST_FROM_VEC(edges, wrap_vfg_edge, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_vfg_node_is_stmt_vfg_node(value v_n) {
    CAMLparam1(v_n);
    CAMLreturn(Val_bool(SVFUtil::isa<StmtVFGNode>(unwrap_vfg_node(v_n))));
}

extern "C" CAMLprim value caml_vfg_node_is_phi_vfg_node(value v_n) {
    CAMLparam1(v_n);
    CAMLreturn(Val_bool(SVFUtil::isa<PHIVFGNode>(unwrap_vfg_node(v_n))));
}

extern "C" CAMLprim value caml_vfg_node_is_argument_vfg_node(value v_n) {
    CAMLparam1(v_n);
    CAMLreturn(Val_bool(SVFUtil::isa<ArgumentVFGNode>(unwrap_vfg_node(v_n))));
}

extern "C" CAMLprim value caml_vfg_node_is_cmp_vfg_node(value v_n) {
    CAMLparam1(v_n);
    CAMLreturn(Val_bool(SVFUtil::isa<CmpVFGNode>(unwrap_vfg_node(v_n))));
}

extern "C" CAMLprim value caml_vfg_node_is_binary_op_vfg_node(value v_n) {
    CAMLparam1(v_n);
    CAMLreturn(Val_bool(SVFUtil::isa<BinaryOPVFGNode>(unwrap_vfg_node(v_n))));
}

extern "C" CAMLprim value caml_vfg_node_is_unary_op_vfg_node(value v_n) {
    CAMLparam1(v_n);
    CAMLreturn(Val_bool(SVFUtil::isa<UnaryOPVFGNode>(unwrap_vfg_node(v_n))));
}

extern "C" CAMLprim value caml_vfg_node_is_branch_vfg_node(value v_n) {
    CAMLparam1(v_n);
    CAMLreturn(Val_bool(SVFUtil::isa<BranchVFGNode>(unwrap_vfg_node(v_n))));
}

extern "C" CAMLprim value caml_vfg_node_get_value(value v_n) {
    CAMLparam1(v_n);
    const SVFVar* var = unwrap_vfg_node(v_n)->getValue();
    CAMLreturn(wrap_svf_var_opt(const_cast<SVFVar*>(var)));
}

/* =========================================================================
 * VFGEdge bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_vfg_edge_to_string(value v_e) {
    CAMLparam1(v_e);
    std::ostringstream oss;
    oss << unwrap_vfg_edge(v_e)->toString();
    CAMLreturn(caml_copy_string(oss.str().c_str()));
}

extern "C" CAMLprim value caml_vfg_edge_is_direct_vfg_edge(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_vfg_edge(v_e)->isDirectVFGEdge()));
}

extern "C" CAMLprim value caml_vfg_edge_is_indirect_vfg_edge(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_vfg_edge(v_e)->isIndirectVFGEdge()));
}

extern "C" CAMLprim value caml_vfg_edge_is_call_vfg_edge(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_vfg_edge(v_e)->isCallVFGEdge()));
}

extern "C" CAMLprim value caml_vfg_edge_is_ret_vfg_edge(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_vfg_edge(v_e)->isRetVFGEdge()));
}

extern "C" CAMLprim value caml_vfg_edge_is_intra_vfg_edge(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_vfg_edge(v_e)->isIntraVFGEdge()));
}

extern "C" CAMLprim value caml_vfg_edge_is_thread_mhp_indirect(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_vfg_edge(v_e)->isThreadMHPIndirectVFGEdge()));
}

extern "C" CAMLprim value caml_vfg_edge_get_src_node(value v_e) {
    CAMLparam1(v_e);
    VFGNode* src = unwrap_vfg_edge(v_e)->getSrcNode();
    CAMLreturn(wrap_vfg_node(src));
}

extern "C" CAMLprim value caml_vfg_edge_get_dst_node(value v_e) {
    CAMLparam1(v_e);
    VFGNode* dst = unwrap_vfg_edge(v_e)->getDstNode();
    CAMLreturn(wrap_vfg_node(dst));
}

/* =========================================================================
 * VFG bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_vfg_get_nodes(value v_vfg) {
    CAMLparam1(v_vfg);
    CAMLlocal1(result);
    /* SVFG inherits from VFG; we use SVFG pointer since vfg_node type maps to VFGNode */
    SVFG* svfg = unwrap_svfg(v_vfg);
    std::vector<VFGNode*> nodes;
    for (auto it = svfg->begin(); it != svfg->end(); ++it)
        nodes.push_back(it->second);
    BUILD_LIST_FROM_VEC(nodes, wrap_vfg_node, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_vfg_get_gnode(value v_vfg, value v_id) {
    CAMLparam2(v_vfg, v_id);
    SVFG* svfg = unwrap_svfg(v_vfg);
    VFGNode* n = svfg->getGNode((NodeID)Int_val(v_id));
    if (!n) caml_failwith("vfg_get_gnode: not found");
    CAMLreturn(wrap_vfg_node(n));
}

extern "C" CAMLprim value caml_vfg_dump(value v_vfg, value v_file) {
    CAMLparam2(v_vfg, v_file);
    unwrap_svfg(v_vfg)->dump(std::string(String_val(v_file)));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_vfg_view(value v_vfg) {
    CAMLparam1(v_vfg);
    unwrap_svfg(v_vfg)->view();
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_vfg_get_total_node_num(value v_vfg) {
    CAMLparam1(v_vfg);
    CAMLreturn(Val_int((int)unwrap_svfg(v_vfg)->getTotalNodeNum()));
}

extern "C" CAMLprim value caml_vfg_get_total_edge_num(value v_vfg) {
    CAMLparam1(v_vfg);
    CAMLreturn(Val_int((int)unwrap_svfg(v_vfg)->getTotalEdgeNum()));
}

/* =========================================================================
 * SVFG bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_svfg_has_def_svfg_node(value v_svfg, value v_var) {
    CAMLparam2(v_svfg, v_var);
    SVFG* svfg = unwrap_svfg(v_svfg);
    const ValVar* var = SVFUtil::dyn_cast<ValVar>(unwrap_svf_var(v_var));
    if (!var) CAMLreturn(Val_bool(false));
    CAMLreturn(Val_bool(svfg->hasDefSVFGNode(var)));
}

extern "C" CAMLprim value caml_svfg_get_def_svfg_node(value v_svfg, value v_var) {
    CAMLparam2(v_svfg, v_var);
    SVFG* svfg = unwrap_svfg(v_svfg);
    const ValVar* var = SVFUtil::dyn_cast<ValVar>(unwrap_svf_var(v_var));
    if (!var) caml_failwith("svfg_get_def_svfg_node: not a ValVar");
    const SVFGNode* n = svfg->getDefSVFGNode(var);
    if (!n) caml_failwith("svfg_get_def_svfg_node: null");
    CAMLreturn(wrap_vfg_node(const_cast<VFGNode*>(static_cast<const VFGNode*>(n))));
}

extern "C" CAMLprim value caml_svfg_get_svfg_node(value v_svfg, value v_id) {
    CAMLparam2(v_svfg, v_id);
    SVFG* svfg = unwrap_svfg(v_svfg);
    SVFGNode* n = svfg->getSVFGNode((NodeID)Int_val(v_id));
    if (!n) caml_failwith("svfg_get_svfg_node: null");
    CAMLreturn(wrap_vfg_node(static_cast<VFGNode*>(n)));
}

extern "C" CAMLprim value caml_svfg_has_svfg_node(value v_svfg, value v_id) {
    CAMLparam2(v_svfg, v_id);
    CAMLreturn(Val_bool(unwrap_svfg(v_svfg)->hasSVFGNode((NodeID)Int_val(v_id))));
}

extern "C" CAMLprim value caml_svfg_is_fun_entry_svfg_node(value v_svfg, value v_node) {
    CAMLparam2(v_svfg, v_node);
    SVFG* svfg = unwrap_svfg(v_svfg);
    VFGNode* vn = unwrap_vfg_node(v_node);
    const SVFGNode* sn = SVFUtil::dyn_cast<SVFGNode>(vn);
    if (!sn) CAMLreturn(Val_bool(false));
    const FunObjVar* fun = svfg->isFunEntrySVFGNode(sn);
    CAMLreturn(Val_bool(fun != nullptr));
}

extern "C" CAMLprim value caml_svfg_is_call_site_ret_svfg_node(value v_svfg, value v_node) {
    CAMLparam2(v_svfg, v_node);
    SVFG* svfg = unwrap_svfg(v_svfg);
    VFGNode* vn = unwrap_vfg_node(v_node);
    const SVFGNode* sn = SVFUtil::dyn_cast<SVFGNode>(vn);
    if (!sn) CAMLreturn(Val_bool(false));
    const CallICFGNode* cs = svfg->isCallSiteRetSVFGNode(sn);
    CAMLreturn(Val_bool(cs != nullptr));
}

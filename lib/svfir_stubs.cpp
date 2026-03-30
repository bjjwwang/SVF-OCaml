/**
 * svfir_stubs.cpp
 *
 * OCaml C FFI stubs for SVFIR (PAG), SVFStmt, SVFVar.
 * Mirrors SVF-Python/pybind/SVFIR.cpp.
 */

#include "svf_ocaml_types.h"

#include "SVF-LLVM/SVFIRBuilder.h"
#include "Graphs/ICFG.h"
#include "SVFIR/SVFType.h"
#include "SVFIR/SVFStatements.h"
#include "SVFIR/SVFVariables.h"
#include "MemoryModel/PointerAnalysis.h"
#include "WPA/Andersen.h"
#include "Util/SVFUtil.h"
#include "Util/Casting.h"

#include <sstream>
#include <vector>

using namespace SVF;

/* =========================================================================
 * SVFIR (PAG) bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_svfir_get_icfg(value v_pag) {
    CAMLparam1(v_pag);
    CAMLreturn(wrap_icfg(unwrap_svfir(v_pag)->getICFG()));
}

extern "C" CAMLprim value caml_svfir_get_call_graph(value v_pag) {
    CAMLparam1(v_pag);
    CAMLreturn(wrap_call_graph(const_cast<CallGraph*>(unwrap_svfir(v_pag)->getCallGraph())));
}

extern "C" CAMLprim value caml_svfir_get_call_sites(value v_pag) {
    CAMLparam1(v_pag);
    CAMLlocal1(result);
    SVFIR* pag = unwrap_svfir(v_pag);
    std::vector<ICFGNode*> sites;
    for (const CallICFGNode* cs : pag->getCallSiteSet())
        sites.push_back(const_cast<CallICFGNode*>(cs));
    result = Val_int(0);
    for (int i = (int)sites.size() - 1; i >= 0; --i) {
        CAMLlocal2(head, cons);
        head = wrap_icfg_node(sites[i]);
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, head);
        Store_field(cons, 1, result);
        result = cons;
    }
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_svfir_get_svf_var_num(value v_pag) {
    CAMLparam1(v_pag);
    CAMLreturn(Val_int((int)unwrap_svfir(v_pag)->getSVFVarNum()));
}

extern "C" CAMLprim value caml_svfir_get_gnode(value v_pag, value v_id) {
    CAMLparam2(v_pag, v_id);
    SVFIR* pag = unwrap_svfir(v_pag);
    const PAGNode* node = pag->getGNode((NodeID)Int_val(v_id));
    if (!node) caml_failwith("svfir_get_gnode: node not found");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(static_cast<const SVFVar*>(node))));
}

extern "C" CAMLprim value caml_svfir_get_module_identifier(value v_pag) {
    CAMLparam1(v_pag);
    CAMLreturn(caml_copy_string(unwrap_svfir(v_pag)->getModuleIdentifier().c_str()));
}

extern "C" CAMLprim value caml_svfir_dump(value v_pag, value v_file) {
    CAMLparam2(v_pag, v_file);
    unwrap_svfir(v_pag)->dump(std::string(String_val(v_file)));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_svfir_iter(value v_pag) {
    CAMLparam1(v_pag);
    CAMLlocal3(result, pair, cons);
    SVFIR* pag = unwrap_svfir(v_pag);
    std::vector<std::pair<NodeID, SVFVar*>> nodes;
    for (auto it = pag->begin(); it != pag->end(); ++it)
        nodes.push_back({it->first, it->second});
    result = Val_int(0);
    for (int i = (int)nodes.size() - 1; i >= 0; --i) {
        pair = caml_alloc(2, 0);
        Store_field(pair, 0, Val_int((int)nodes[i].first));
        Store_field(pair, 1, wrap_svf_var(nodes[i].second));
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, pair);
        Store_field(cons, 1, result);
        result = cons;
    }
    CAMLreturn(result);
}

/* =========================================================================
 * SVFStmt bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_svf_stmt_to_string(value v_stmt) {
    CAMLparam1(v_stmt);
    SVFStmt* s = unwrap_svf_stmt(v_stmt);
    if (!s) CAMLreturn(caml_copy_string("nullptr"));
    std::ostringstream oss;
    oss << s->toString();
    CAMLreturn(caml_copy_string(oss.str().c_str()));
}

extern "C" CAMLprim value caml_svf_stmt_get_src_node(value v_stmt) {
    CAMLparam1(v_stmt);
    CAMLreturn(wrap_svf_var(unwrap_svf_stmt(v_stmt)->getSrcNode()));
}

extern "C" CAMLprim value caml_svf_stmt_get_dst_node(value v_stmt) {
    CAMLparam1(v_stmt);
    CAMLreturn(wrap_svf_var(unwrap_svf_stmt(v_stmt)->getDstNode()));
}

extern "C" CAMLprim value caml_svf_stmt_get_src_id(value v_stmt) {
    CAMLparam1(v_stmt);
    CAMLreturn(Val_int((int)unwrap_svf_stmt(v_stmt)->getSrcID()));
}

extern "C" CAMLprim value caml_svf_stmt_get_dst_id(value v_stmt) {
    CAMLparam1(v_stmt);
    CAMLreturn(Val_int((int)unwrap_svf_stmt(v_stmt)->getDstID()));
}

extern "C" CAMLprim value caml_svf_stmt_get_edge_id(value v_stmt) {
    CAMLparam1(v_stmt);
    CAMLreturn(Val_int((int)unwrap_svf_stmt(v_stmt)->getEdgeID()));
}

extern "C" CAMLprim value caml_svf_stmt_get_icfg_node(value v_stmt) {
    CAMLparam1(v_stmt);
    CAMLreturn(wrap_icfg_node(const_cast<ICFGNode*>(unwrap_svf_stmt(v_stmt)->getICFGNode())));
}

/* SVFStmt type-check helpers */
#define STMT_IS(name, CastType) \
extern "C" CAMLprim value caml_svf_stmt_is_##name(value v_stmt) { \
    CAMLparam1(v_stmt); \
    CAMLreturn(Val_bool(SVFUtil::dyn_cast<const CastType>(unwrap_svf_stmt(v_stmt)) != nullptr)); \
}

STMT_IS(addr,      AddrStmt)
STMT_IS(copy,      CopyStmt)
STMT_IS(store,     StoreStmt)
STMT_IS(load,      LoadStmt)
STMT_IS(call_pe,   CallPE)
STMT_IS(ret_pe,    RetPE)
STMT_IS(gep,       GepStmt)
STMT_IS(phi,       PhiStmt)
STMT_IS(select,    SelectStmt)
STMT_IS(cmp,       CmpStmt)
STMT_IS(binary_op, BinaryOPStmt)
STMT_IS(unary_op,  UnaryOPStmt)
STMT_IS(branch,    BranchStmt)

/* =========================================================================
 * SVFVar bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_svf_var_get_name(value v_var) {
    CAMLparam1(v_var);
    CAMLreturn(caml_copy_string(unwrap_svf_var(v_var)->getName().c_str()));
}

extern "C" CAMLprim value caml_svf_var_get_id(value v_var) {
    CAMLparam1(v_var);
    CAMLreturn(Val_int((int)unwrap_svf_var(v_var)->getId()));
}

extern "C" CAMLprim value caml_svf_var_is_pointer(value v_var) {
    CAMLparam1(v_var);
    CAMLreturn(Val_bool(unwrap_svf_var(v_var)->isPointer()));
}

extern "C" CAMLprim value caml_svf_var_get_value_name(value v_var) {
    CAMLparam1(v_var);
    CAMLreturn(caml_copy_string(unwrap_svf_var(v_var)->getValueName().c_str()));
}

extern "C" CAMLprim value caml_svf_var_is_isolated(value v_var) {
    CAMLparam1(v_var);
    CAMLreturn(Val_bool(unwrap_svf_var(v_var)->isIsolatedNode()));
}

extern "C" CAMLprim value caml_svf_var_is_val_var(value v_var) {
    CAMLparam1(v_var);
    CAMLreturn(Val_bool(SVFUtil::isa<ValVar>(unwrap_svf_var(v_var))));
}

extern "C" CAMLprim value caml_svf_var_is_obj_var(value v_var) {
    CAMLparam1(v_var);
    CAMLreturn(Val_bool(SVFUtil::isa<ObjVar>(unwrap_svf_var(v_var))));
}

extern "C" CAMLprim value caml_svf_var_is_fun_obj_var(value v_var) {
    CAMLparam1(v_var);
    CAMLreturn(Val_bool(SVFUtil::isa<FunObjVar>(unwrap_svf_var(v_var))));
}

extern "C" CAMLprim value caml_svf_var_get_in_edges(value v_var) {
    CAMLparam1(v_var);
    CAMLlocal1(result);
    SVFVar* var = unwrap_svf_var(v_var);
    std::vector<SVFStmt*> edges;
    for (auto& e : var->getInEdges()) edges.push_back(e);
    result = Val_int(0);
    for (int i = (int)edges.size() - 1; i >= 0; --i) {
        CAMLlocal2(head, cons);
        head = wrap_svf_stmt(edges[i]);
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, head);
        Store_field(cons, 1, result);
        result = cons;
    }
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_svf_var_get_out_edges(value v_var) {
    CAMLparam1(v_var);
    CAMLlocal1(result);
    SVFVar* var = unwrap_svf_var(v_var);
    std::vector<SVFStmt*> edges;
    for (auto& e : var->getOutEdges()) edges.push_back(e);
    result = Val_int(0);
    for (int i = (int)edges.size() - 1; i >= 0; --i) {
        CAMLlocal2(head, cons);
        head = wrap_svf_stmt(edges[i]);
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, head);
        Store_field(cons, 1, result);
        result = cons;
    }
    CAMLreturn(result);
}

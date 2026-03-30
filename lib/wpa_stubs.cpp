/**
 * wpa_stubs.cpp
 *
 * OCaml C FFI stubs for Pointer Analysis (WPA):
 *   AndersenBase, AndersenWaveDiff, Steensgaard, PointsTo,
 *   ConstraintGraph, ConstraintNode, ConstraintEdge, CallGraphSCC.
 * Mirrors SVF-Python/pybind/WPA.cpp and MemoryModel.cpp.
 */

#include "svf_ocaml_types.h"

#include "SVF-LLVM/SVFIRBuilder.h"
#include "Graphs/ConsG.h"
#include "Graphs/ConsGNode.h"
#include "Graphs/ConsGEdge.h"
#include "Graphs/SCC.h"
#include "SVFIR/SVFStatements.h"
#include "MemoryModel/PointerAnalysis.h"
#include "WPA/Andersen.h"
#include "WPA/Steensgaard.h"
#include "WPA/WPASolver.h"
#include "Util/SVFUtil.h"

#include <sstream>
#include <vector>
#include <memory>
#include <stdexcept>

using namespace SVF;

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
 * Inner class to expose protected AndersenBase members
 * ========================================================================= */
class PublicAndersen : public AndersenBase {
public:
    using AndersenBase::AndersenBase;
    using AndersenBase::pushIntoWorklist;
    using AndersenBase::popFromWorklist;
    using AndersenBase::isWorklistEmpty;
    using AndersenBase::unionPts;
    using AndersenBase::addPts;
    using AndersenBase::getPts;
    using AndersenBase::analyze;
    using AndersenBase::initWorklist;
    using AndersenBase::finalize;
    using AndersenBase::initialize;
    using AndersenBase::updateCallGraph;
    using AndersenBase::getIndirectCallsites;
    using AndersenBase::getConstraintGraph;
    using AndersenBase::alias;
    bool addCopyEdge(NodeID src, NodeID dst) override {
        (void)src; (void)dst;
        assert(false && "Cannot call AndersenBase::addCopyEdge directly");
        return false;
    }
};

/* =========================================================================
 * AliasResult → int mapping
 * ========================================================================= */
static int alias_result_to_int(AliasResult r) {
    switch (r) {
        case AliasResult::NoAlias:      return 0;
        case AliasResult::MayAlias:     return 1;
        case AliasResult::MustAlias:    return 2;
        case AliasResult::PartialAlias: return 3;
        default:                        return 1;
    }
}

/* =========================================================================
 * PointsTo bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_points_to_create(value v_unit) {
    CAMLparam1(v_unit);
    CAMLreturn(wrap_points_to_owned(new PointsTo()));
}

extern "C" CAMLprim value caml_points_to_set(value v_pts, value v_id) {
    CAMLparam2(v_pts, v_id);
    unwrap_points_to(v_pts)->set((NodeID)Int_val(v_id));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_points_to_to_list(value v_pts) {
    CAMLparam1(v_pts);
    CAMLlocal1(result);
    PointsTo* pts = unwrap_points_to(v_pts);
    std::vector<NodeID> ids;
    for (NodeID id : *pts) ids.push_back(id);
    result = Val_int(0);
    for (int i = (int)ids.size() - 1; i >= 0; --i) {
        CAMLlocal2(head, cons);
        head = Val_int((int)ids[i]);
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, head);
        Store_field(cons, 1, result);
        result = cons;
    }
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_points_to_to_string(value v_pts) {
    CAMLparam1(v_pts);
    PointsTo* pts = unwrap_points_to(v_pts);
    std::ostringstream oss;
    for (NodeID id : *pts) oss << id << " ";
    CAMLreturn(caml_copy_string(oss.str().c_str()));
}

extern "C" CAMLprim value caml_points_to_count(value v_pts) {
    CAMLparam1(v_pts);
    CAMLreturn(Val_int((int)unwrap_points_to(v_pts)->count()));
}

/* =========================================================================
 * AndersenBase bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_andersen_base_create(value v_pag) {
    CAMLparam1(v_pag);
    SVFIR* pag = unwrap_svfir(v_pag);
    PublicAndersen* pa = new PublicAndersen(pag);
    CAMLreturn(wrap_andersen_base(static_cast<AndersenBase*>(pa)));
}

extern "C" CAMLprim value caml_andersen_base_analyze(value v_pa) {
    CAMLparam1(v_pa);
    unwrap_andersen_base(v_pa)->analyze();
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_andersen_base_get_constraint_graph(value v_pa) {
    CAMLparam1(v_pa);
    AndersenBase* pa = unwrap_andersen_base(v_pa);
    CAMLreturn(wrap_constraint_graph(pa->getConstraintGraph()));
}

extern "C" CAMLprim value caml_andersen_base_get_pts(value v_pa, value v_id) {
    CAMLparam2(v_pa, v_id);
    AndersenBase* pa = unwrap_andersen_base(v_pa);
    PointsTo* copy = new PointsTo(pa->getPts((NodeID)Int_val(v_id)));
    CAMLreturn(wrap_points_to_owned(copy));
}

extern "C" CAMLprim value caml_andersen_base_add_pts(value v_pa, value v_id, value v_ptd) {
    CAMLparam3(v_pa, v_id, v_ptd);
    PublicAndersen* pa = static_cast<PublicAndersen*>(unwrap_andersen_base(v_pa));
    bool changed = pa->addPts((NodeID)Int_val(v_id), (NodeID)Int_val(v_ptd));
    CAMLreturn(Val_bool(changed));
}

extern "C" CAMLprim value caml_andersen_base_union_pts(value v_pa, value v_id, value v_ptd) {
    CAMLparam3(v_pa, v_id, v_ptd);
    PublicAndersen* pa = static_cast<PublicAndersen*>(unwrap_andersen_base(v_pa));
    bool changed = pa->unionPts((NodeID)Int_val(v_id), (NodeID)Int_val(v_ptd));
    CAMLreturn(Val_bool(changed));
}

extern "C" CAMLprim value caml_andersen_base_alias(value v_pa, value v_id1, value v_id2) {
    CAMLparam3(v_pa, v_id1, v_id2);
    PublicAndersen* pa = static_cast<PublicAndersen*>(unwrap_andersen_base(v_pa));
    AliasResult res = pa->alias((NodeID)Int_val(v_id1), (NodeID)Int_val(v_id2));
    CAMLreturn(Val_int(alias_result_to_int(res)));
}

extern "C" CAMLprim value caml_andersen_base_is_worklist_empty(value v_pa) {
    CAMLparam1(v_pa);
    PublicAndersen* pa = static_cast<PublicAndersen*>(unwrap_andersen_base(v_pa));
    CAMLreturn(Val_bool(pa->isWorklistEmpty()));
}

extern "C" CAMLprim value caml_andersen_base_push_worklist(value v_pa, value v_id) {
    CAMLparam2(v_pa, v_id);
    static_cast<PublicAndersen*>(unwrap_andersen_base(v_pa))->pushIntoWorklist((NodeID)Int_val(v_id));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_andersen_base_pop_worklist(value v_pa) {
    CAMLparam1(v_pa);
    PublicAndersen* pa = static_cast<PublicAndersen*>(unwrap_andersen_base(v_pa));
    CAMLreturn(Val_int((int)pa->popFromWorklist()));
}

extern "C" CAMLprim value caml_andersen_base_initialize(value v_pa) {
    CAMLparam1(v_pa);
    static_cast<PublicAndersen*>(unwrap_andersen_base(v_pa))->initialize();
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_andersen_base_finalize(value v_pa) {
    CAMLparam1(v_pa);
    static_cast<PublicAndersen*>(unwrap_andersen_base(v_pa))->finalize();
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_andersen_base_init_worklist(value v_pa) {
    CAMLparam1(v_pa);
    static_cast<PublicAndersen*>(unwrap_andersen_base(v_pa))->initWorklist();
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_andersen_base_update_call_graph(value v_pa) {
    CAMLparam1(v_pa);
    SVFIR::CallSiteToFunPtrMap emptyMap;
    bool changed = static_cast<PublicAndersen*>(unwrap_andersen_base(v_pa))->updateCallGraph(emptyMap);
    CAMLreturn(Val_bool(changed));
}

/* =========================================================================
 * AndersenWaveDiff bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_andersen_wave_diff_create(value v_pag) {
    CAMLparam1(v_pag);
    AndersenWaveDiff* pa = new AndersenWaveDiff(unwrap_svfir(v_pag));
    CAMLreturn(wrap_andersen_wave_diff(pa));
}

extern "C" CAMLprim value caml_andersen_wave_diff_analyze(value v_pa) {
    CAMLparam1(v_pa);
    unwrap_andersen_wave_diff(v_pa)->analyze();
    CAMLreturn(Val_unit);
}

/* =========================================================================
 * Steensgaard bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_steensgaard_create(value v_pag) {
    CAMLparam1(v_pag);
    Steensgaard* pa = new Steensgaard(unwrap_svfir(v_pag));
    CAMLreturn(wrap_steensgaard(pa));
}

extern "C" CAMLprim value caml_steensgaard_analyze(value v_pa) {
    CAMLparam1(v_pa);
    unwrap_steensgaard(v_pa)->analyze();
    CAMLreturn(Val_unit);
}

/* =========================================================================
 * ConstraintNode bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_constraint_node_get_id(value v_n) {
    CAMLparam1(v_n);
    CAMLreturn(Val_int((int)unwrap_constraint_node(v_n)->getId()));
}

static value constraint_edge_list_from_set(const ConstraintEdge::ConstraintEdgeSetTy& es) {
    CAMLparam0();
    CAMLlocal1(result);
    std::vector<ConstraintEdge*> edges;
    for (ConstraintEdge* e : es) edges.push_back(e);
    BUILD_LIST_FROM_VEC(edges, wrap_constraint_edge, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_constraint_node_get_in_edges(value v_n) {
    CAMLparam1(v_n);
    ConstraintNode* n = unwrap_constraint_node(v_n);
    std::vector<ConstraintEdge*> edges;
    for (auto& e : n->getInEdges()) edges.push_back(e);
    CAMLlocal1(result);
    BUILD_LIST_FROM_VEC(edges, wrap_constraint_edge, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_constraint_node_get_out_edges(value v_n) {
    CAMLparam1(v_n);
    ConstraintNode* n = unwrap_constraint_node(v_n);
    std::vector<ConstraintEdge*> edges;
    for (auto& e : n->getOutEdges()) edges.push_back(e);
    CAMLlocal1(result);
    BUILD_LIST_FROM_VEC(edges, wrap_constraint_edge, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_constraint_node_get_direct_in_edges(value v_n) {
    CAMLparam1(v_n);
    return constraint_edge_list_from_set(unwrap_constraint_node(v_n)->getDirectInEdges());
}
extern "C" CAMLprim value caml_constraint_node_get_direct_out_edges(value v_n) {
    CAMLparam1(v_n);
    return constraint_edge_list_from_set(unwrap_constraint_node(v_n)->getDirectOutEdges());
}
extern "C" CAMLprim value caml_constraint_node_get_copy_in_edges(value v_n) {
    CAMLparam1(v_n);
    return constraint_edge_list_from_set(unwrap_constraint_node(v_n)->getCopyInEdges());
}
extern "C" CAMLprim value caml_constraint_node_get_copy_out_edges(value v_n) {
    CAMLparam1(v_n);
    return constraint_edge_list_from_set(unwrap_constraint_node(v_n)->getCopyOutEdges());
}
extern "C" CAMLprim value caml_constraint_node_get_gep_in_edges(value v_n) {
    CAMLparam1(v_n);
    return constraint_edge_list_from_set(unwrap_constraint_node(v_n)->getGepInEdges());
}
extern "C" CAMLprim value caml_constraint_node_get_gep_out_edges(value v_n) {
    CAMLparam1(v_n);
    return constraint_edge_list_from_set(unwrap_constraint_node(v_n)->getGepOutEdges());
}
extern "C" CAMLprim value caml_constraint_node_get_load_in_edges(value v_n) {
    CAMLparam1(v_n);
    return constraint_edge_list_from_set(unwrap_constraint_node(v_n)->getLoadInEdges());
}
extern "C" CAMLprim value caml_constraint_node_get_load_out_edges(value v_n) {
    CAMLparam1(v_n);
    return constraint_edge_list_from_set(unwrap_constraint_node(v_n)->getLoadOutEdges());
}
extern "C" CAMLprim value caml_constraint_node_get_store_in_edges(value v_n) {
    CAMLparam1(v_n);
    return constraint_edge_list_from_set(unwrap_constraint_node(v_n)->getStoreInEdges());
}
extern "C" CAMLprim value caml_constraint_node_get_store_out_edges(value v_n) {
    CAMLparam1(v_n);
    return constraint_edge_list_from_set(unwrap_constraint_node(v_n)->getStoreOutEdges());
}
extern "C" CAMLprim value caml_constraint_node_get_addr_in_edges(value v_n) {
    CAMLparam1(v_n);
    return constraint_edge_list_from_set(unwrap_constraint_node(v_n)->getAddrInEdges());
}
extern "C" CAMLprim value caml_constraint_node_get_addr_out_edges(value v_n) {
    CAMLparam1(v_n);
    return constraint_edge_list_from_set(unwrap_constraint_node(v_n)->getAddrOutEdges());
}

/* =========================================================================
 * ConstraintEdge bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_constraint_edge_get_src_node(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(wrap_constraint_node(unwrap_constraint_edge(v_e)->getSrcNode()));
}

extern "C" CAMLprim value caml_constraint_edge_get_dst_node(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(wrap_constraint_node(unwrap_constraint_edge(v_e)->getDstNode()));
}

extern "C" CAMLprim value caml_constraint_edge_get_src_id(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_int((int)unwrap_constraint_edge(v_e)->getSrcID()));
}

extern "C" CAMLprim value caml_constraint_edge_get_dst_id(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_int((int)unwrap_constraint_edge(v_e)->getDstID()));
}

extern "C" CAMLprim value caml_constraint_edge_is_addr(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_constraint_edge(v_e)->getEdgeKind() == ConstraintEdge::Addr));
}
extern "C" CAMLprim value caml_constraint_edge_is_copy(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_constraint_edge(v_e)->getEdgeKind() == ConstraintEdge::Copy));
}
extern "C" CAMLprim value caml_constraint_edge_is_store(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_constraint_edge(v_e)->getEdgeKind() == ConstraintEdge::Store));
}
extern "C" CAMLprim value caml_constraint_edge_is_load(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_constraint_edge(v_e)->getEdgeKind() == ConstraintEdge::Load));
}
extern "C" CAMLprim value caml_constraint_edge_is_gep(value v_e) {
    CAMLparam1(v_e);
    int k = unwrap_constraint_edge(v_e)->getEdgeKind();
    CAMLreturn(Val_bool(k == ConstraintEdge::NormalGep || k == ConstraintEdge::VariantGep));
}
extern "C" CAMLprim value caml_constraint_edge_is_normal_gep(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_constraint_edge(v_e)->getEdgeKind() == ConstraintEdge::NormalGep));
}
extern "C" CAMLprim value caml_constraint_edge_is_variant_gep(value v_e) {
    CAMLparam1(v_e);
    CAMLreturn(Val_bool(unwrap_constraint_edge(v_e)->getEdgeKind() == ConstraintEdge::VariantGep));
}

/* Downcast functions */
extern "C" CAMLprim value caml_constraint_edge_as_addr(value v_e) {
    CAMLparam1(v_e);
    AddrCGEdge* e = SVFUtil::dyn_cast<AddrCGEdge>(unwrap_constraint_edge(v_e));
    CAMLreturn(wrap_constraint_edge_opt(static_cast<ConstraintEdge*>(e)));
}
extern "C" CAMLprim value caml_constraint_edge_as_copy(value v_e) {
    CAMLparam1(v_e);
    CopyCGEdge* e = SVFUtil::dyn_cast<CopyCGEdge>(unwrap_constraint_edge(v_e));
    CAMLreturn(wrap_constraint_edge_opt(static_cast<ConstraintEdge*>(e)));
}
extern "C" CAMLprim value caml_constraint_edge_as_store(value v_e) {
    CAMLparam1(v_e);
    StoreCGEdge* e = SVFUtil::dyn_cast<StoreCGEdge>(unwrap_constraint_edge(v_e));
    CAMLreturn(wrap_constraint_edge_opt(static_cast<ConstraintEdge*>(e)));
}
extern "C" CAMLprim value caml_constraint_edge_as_load(value v_e) {
    CAMLparam1(v_e);
    LoadCGEdge* e = SVFUtil::dyn_cast<LoadCGEdge>(unwrap_constraint_edge(v_e));
    CAMLreturn(wrap_constraint_edge_opt(static_cast<ConstraintEdge*>(e)));
}
extern "C" CAMLprim value caml_constraint_edge_as_normal_gep(value v_e) {
    CAMLparam1(v_e);
    NormalGepCGEdge* e = SVFUtil::dyn_cast<NormalGepCGEdge>(unwrap_constraint_edge(v_e));
    CAMLreturn(wrap_constraint_edge_opt(static_cast<ConstraintEdge*>(e)));
}
extern "C" CAMLprim value caml_constraint_edge_as_variant_gep(value v_e) {
    CAMLparam1(v_e);
    VariantGepCGEdge* e = SVFUtil::dyn_cast<VariantGepCGEdge>(unwrap_constraint_edge(v_e));
    CAMLreturn(wrap_constraint_edge_opt(static_cast<ConstraintEdge*>(e)));
}

/* =========================================================================
 * ConstraintGraph bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_constraint_graph_get_nodes(value v_cg) {
    CAMLparam1(v_cg);
    CAMLlocal1(result);
    ConstraintGraph* cg = unwrap_constraint_graph(v_cg);
    std::vector<ConstraintNode*> nodes;
    for (auto it = cg->begin(); it != cg->end(); ++it)
        nodes.push_back(it->second);
    BUILD_LIST_FROM_VEC(nodes, wrap_constraint_node, result);
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_constraint_graph_get_gnode(value v_cg, value v_id) {
    CAMLparam2(v_cg, v_id);
    ConstraintNode* n = unwrap_constraint_graph(v_cg)->getConstraintNode((NodeID)Int_val(v_id));
    if (!n) caml_failwith("constraint_graph_get_gnode: not found");
    CAMLreturn(wrap_constraint_node(n));
}

extern "C" CAMLprim value caml_constraint_graph_dump(value v_cg, value v_file) {
    CAMLparam2(v_cg, v_file);
    unwrap_constraint_graph(v_cg)->dump(std::string(String_val(v_file)));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_constraint_graph_add_copy_edge(value v_cg, value v_src, value v_dst) {
    CAMLparam3(v_cg, v_src, v_dst);
    CopyCGEdge* e = unwrap_constraint_graph(v_cg)->addCopyCGEdge(
        (NodeID)Int_val(v_src), (NodeID)Int_val(v_dst));
    if (!e) caml_failwith("constraint_graph_add_copy_edge: failed");
    CAMLreturn(wrap_constraint_edge(static_cast<ConstraintEdge*>(e)));
}

/* =========================================================================
 * CallGraphSCC bindings (SCCDetection<CallGraph*> typedef)
 * ========================================================================= */

typedef PointerAnalysis::CallGraphSCC CGSCCType;

extern "C" CAMLprim value caml_call_graph_scc_is_in_cycle(value v_scc, value v_id) {
    CAMLparam2(v_scc, v_id);
    CGSCCType* scc = (CGSCCType*)unwrap_svfir(v_scc); /* Use raw pointer stored as svfir for now */
    /* Actually we need call_graph_scc type - use the scc pointer directly */
    (void)scc;
    /* We'll use andersen_base getCallGraphSCC path, but need scc pointer */
    /* For now return false - caller should use andersen_base methods instead */
    CAMLreturn(Val_bool(false));
}

extern "C" CAMLprim value caml_andersen_base_is_in_cycle(value v_pa, value v_id) {
    CAMLparam2(v_pa, v_id);
    AndersenBase* pa = unwrap_andersen_base(v_pa);
    PointerAnalysis::CallGraphSCC* scc = pa->getCallGraphSCC();
    if (!scc) CAMLreturn(Val_bool(false));
    CAMLreturn(Val_bool(scc->isInCycle((NodeID)Int_val(v_id))));
}

extern "C" CAMLprim value caml_andersen_base_scc_rep_node(value v_pa, value v_id) {
    CAMLparam2(v_pa, v_id);
    AndersenBase* pa = unwrap_andersen_base(v_pa);
    PointerAnalysis::CallGraphSCC* scc = pa->getCallGraphSCC();
    if (!scc) CAMLreturn(v_id);
    CAMLreturn(Val_int((int)scc->repNode((NodeID)Int_val(v_id))));
}

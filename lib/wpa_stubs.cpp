/**
 * wpa_stubs.cpp
 *
 * OCaml C FFI stubs for Pointer Analysis (WPA):
 *   AndersenBase, AndersenWaveDiff, Steensgaard, PointsTo, ConstraintGraph.
 * Mirrors SVF-Python/pybind/WPA.cpp and MemoryModel.cpp.
 */

#include "svf_ocaml_types.h"

#include "SVF-LLVM/SVFIRBuilder.h"
#include "Graphs/ConsG.h"
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

/* =========================================================================
 * Inner class to expose protected AndersenBase members
 * (mirrors SVF-Python WPA.cpp)
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
 * AliasResult → int mapping (matches OCaml variant order in wpa.ml)
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
        CAMLlocal1(cons);
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, Val_int((int)ids[i]));
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

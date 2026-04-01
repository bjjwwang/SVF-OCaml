/**
 * mta_stubs.cpp
 *
 * OCaml C FFI stubs for Multi-Thread Analysis (MTA):
 *   MTA, MHP, LockAnalysis, TCT.
 * Mirrors SVF-Python/pybind/MTA.cpp.
 */

#include "svf_ocaml_types.h"

#include "SVF-LLVM/SVFIRBuilder.h"
#include "Graphs/ICFG.h"
#include "MTA/TCT.h"
#include "MTA/MTA.h"
#include "MTA/MHP.h"
#include "MTA/LockAnalysis.h"
#include "WPA/Andersen.h"
#include "Util/SVFUtil.h"
#include "Util/Casting.h"

#include <stdexcept>

using namespace SVF;

/* =========================================================================
 * MTA bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_mta_create(value v_unit) {
    CAMLparam1(v_unit);
    CAMLreturn(wrap_mta(new MTA()));
}

extern "C" CAMLprim value caml_mta_run_on_module(value v_mta, value v_pag) {
    CAMLparam2(v_mta, v_pag);
    unwrap_mta(v_mta)->runOnModule(unwrap_svfir(v_pag));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_mta_detect(value v_mta) {
    CAMLparam1(v_mta);
    unwrap_mta(v_mta)->detect();
    CAMLreturn(Val_unit);
}

/* =========================================================================
 * TCT bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_tct_create(value v_pa) {
    CAMLparam1(v_pa);
    AndersenBase* pa = unwrap_andersen_base(v_pa);
    TCT* tct = new TCT(SVFUtil::dyn_cast<PointerAnalysis>(pa));
    CAMLreturn(wrap_tct(tct));
}

extern "C" CAMLprim value caml_tct_get_thread_call_graph(value v_tct) {
    CAMLparam1(v_tct);
    ThreadCallGraph* tcg = unwrap_tct(v_tct)->getThreadCallGraph();
    if (!tcg) caml_failwith("tct_get_thread_call_graph: null thread call graph");
    CAMLreturn(wrap_call_graph(static_cast<CallGraph*>(tcg)));
}

extern "C" CAMLprim value caml_tct_dump(value v_tct) {
    CAMLparam1(v_tct);
    unwrap_tct(v_tct)->dump("tct.dot");
    CAMLreturn(Val_unit);
}

/* =========================================================================
 * MHP bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_mhp_create(value v_tct) {
    CAMLparam1(v_tct);
    CAMLreturn(wrap_mhp(new MHP(unwrap_tct(v_tct))));
}

extern "C" CAMLprim value caml_mhp_analyze(value v_mhp) {
    CAMLparam1(v_mhp);
    unwrap_mhp(v_mhp)->analyze();
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_mhp_may_happen_in_parallel(value v_mhp, value v_n1, value v_n2) {
    CAMLparam3(v_mhp, v_n1, v_n2);
    MHP* mhp = unwrap_mhp(v_mhp);
    ICFGNode* n1 = unwrap_icfg_node(v_n1);
    ICFGNode* n2 = unwrap_icfg_node(v_n2);
    CAMLreturn(Val_bool(mhp->mayHappenInParallelInst(n1, n2)));
}

/* =========================================================================
 * LockAnalysis bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_lock_analysis_create(value v_tct) {
    CAMLparam1(v_tct);
    CAMLreturn(wrap_lock_analysis(new LockAnalysis(unwrap_tct(v_tct))));
}

extern "C" CAMLprim value caml_lock_analysis_analyze(value v_la) {
    CAMLparam1(v_la);
    unwrap_lock_analysis(v_la)->analyze();
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_lock_analysis_is_protected(value v_la, value v_n1, value v_n2) {
    CAMLparam3(v_la, v_n1, v_n2);
    LockAnalysis* la = unwrap_lock_analysis(v_la);
    ICFGNode* n1 = unwrap_icfg_node(v_n1);
    ICFGNode* n2 = unwrap_icfg_node(v_n2);
    CAMLreturn(Val_bool(la->isProtectedByCommonLock(n1, n2)));
}

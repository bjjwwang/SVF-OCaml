/**
 * ae_stubs.cpp
 *
 * OCaml C FFI stubs for Abstract Execution (AE).
 * Mirrors SVF-Python/pybind/AE.cpp (core subset).
 */

#include "svf_ocaml_types.h"

#include "AE/Core/AbstractState.h"
#include "AE/Core/IntervalValue.h"
#include "Util/SVFUtil.h"

#include <sstream>

using namespace SVF;

/* =========================================================================
 * AbstractState bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_abstract_state_to_string(value v_as) {
    CAMLparam1(v_as);
    AbstractState* as = unwrap_abstract_state(v_as);
    std::ostringstream oss;
    oss << as->toString();
    CAMLreturn(caml_copy_string(oss.str().c_str()));
}

/* =========================================================================
 * IntervalValue bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_interval_value_create(value v_lb, value v_ub) {
    CAMLparam2(v_lb, v_ub);
    IntervalValue* iv = new IntervalValue((s64_t)Int_val(v_lb), (s64_t)Int_val(v_ub));
    CAMLreturn(wrap_interval_value(iv));
}

extern "C" CAMLprim value caml_interval_value_lb(value v_iv) {
    CAMLparam1(v_iv);
    IntervalValue* iv = unwrap_interval_value(v_iv);
    CAMLreturn(Val_int((int)iv->lb().getNumeral()));
}

extern "C" CAMLprim value caml_interval_value_ub(value v_iv) {
    CAMLparam1(v_iv);
    IntervalValue* iv = unwrap_interval_value(v_iv);
    CAMLreturn(Val_int((int)iv->ub().getNumeral()));
}

extern "C" CAMLprim value caml_interval_value_is_bottom(value v_iv) {
    CAMLparam1(v_iv);
    CAMLreturn(Val_bool(unwrap_interval_value(v_iv)->isBottom()));
}

extern "C" CAMLprim value caml_interval_value_is_top(value v_iv) {
    CAMLparam1(v_iv);
    CAMLreturn(Val_bool(unwrap_interval_value(v_iv)->isTop()));
}

extern "C" CAMLprim value caml_interval_value_to_string(value v_iv) {
    CAMLparam1(v_iv);
    std::ostringstream oss;
    oss << unwrap_interval_value(v_iv)->toString();
    CAMLreturn(caml_copy_string(oss.str().c_str()));
}

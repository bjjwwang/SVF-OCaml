/**
 * ae_stubs.cpp
 *
 * OCaml C FFI stubs for Abstract Execution (AE).
 * Mirrors SVF-Python/pybind/AE.cpp.
 */

#include "svf_ocaml_types.h"

#include "AE/Core/AbstractState.h"
#include "AE/Core/IntervalValue.h"
#include "AE/Core/AddressValue.h"
#include "SVFIR/SVFStatements.h"
#include "SVFIR/SVFVariables.h"
#include "Util/SVFUtil.h"

#include <sstream>
#include <vector>

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
 * BoundedInt bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_bounded_int_create(value v_val, value v_is_inf) {
    CAMLparam2(v_val, v_is_inf);
    s64_t val = (s64_t)Int_val(v_val);
    bool is_inf = Bool_val(v_is_inf);
    BoundedInt* b = new BoundedInt(val, is_inf);
    CAMLreturn(wrap_bounded_int(b));
}

extern "C" CAMLprim value caml_bounded_int_plus_infinity(value v_unit) {
    CAMLparam1(v_unit);
    CAMLreturn(wrap_bounded_int(new BoundedInt(BoundedInt::plus_infinity())));
}

extern "C" CAMLprim value caml_bounded_int_minus_infinity(value v_unit) {
    CAMLparam1(v_unit);
    CAMLreturn(wrap_bounded_int(new BoundedInt(BoundedInt::minus_infinity())));
}

extern "C" CAMLprim value caml_bounded_int_get_numeral(value v_b) {
    CAMLparam1(v_b);
    CAMLreturn(Val_int((int)unwrap_bounded_int(v_b)->getNumeral()));
}

extern "C" CAMLprim value caml_bounded_int_is_infinity(value v_b) {
    CAMLparam1(v_b); CAMLreturn(Val_bool(unwrap_bounded_int(v_b)->is_infinity()));
}
extern "C" CAMLprim value caml_bounded_int_is_plus_infinity(value v_b) {
    CAMLparam1(v_b); CAMLreturn(Val_bool(unwrap_bounded_int(v_b)->is_plus_infinity()));
}
extern "C" CAMLprim value caml_bounded_int_is_minus_infinity(value v_b) {
    CAMLparam1(v_b); CAMLreturn(Val_bool(unwrap_bounded_int(v_b)->is_minus_infinity()));
}
extern "C" CAMLprim value caml_bounded_int_is_zero(value v_b) {
    CAMLparam1(v_b); CAMLreturn(Val_bool(unwrap_bounded_int(v_b)->is_zero()));
}
extern "C" CAMLprim value caml_bounded_int_leq(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    CAMLreturn(Val_bool(unwrap_bounded_int(v_a)->leq(*unwrap_bounded_int(v_b))));
}
extern "C" CAMLprim value caml_bounded_int_geq(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    CAMLreturn(Val_bool(unwrap_bounded_int(v_a)->geq(*unwrap_bounded_int(v_b))));
}
extern "C" CAMLprim value caml_bounded_int_equal(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    CAMLreturn(Val_bool(unwrap_bounded_int(v_a)->equal(*unwrap_bounded_int(v_b))));
}

/* =========================================================================
 * IntervalValue bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_interval_value_create(value v_lb, value v_ub) {
    CAMLparam2(v_lb, v_ub);
    IntervalValue* iv = new IntervalValue((s64_t)Int_val(v_lb), (s64_t)Int_val(v_ub));
    CAMLreturn(wrap_interval_value(iv));
}

extern "C" CAMLprim value caml_interval_value_top(value v_unit) {
    CAMLparam1(v_unit);
    CAMLreturn(wrap_interval_value(new IntervalValue(IntervalValue::top())));
}

extern "C" CAMLprim value caml_interval_value_bottom(value v_unit) {
    CAMLparam1(v_unit);
    CAMLreturn(wrap_interval_value(new IntervalValue(IntervalValue::bottom())));
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

extern "C" CAMLprim value caml_interval_value_lb_bounded(value v_iv) {
    CAMLparam1(v_iv);
    IntervalValue* iv = unwrap_interval_value(v_iv);
    CAMLreturn(wrap_bounded_int(new BoundedInt(iv->lb())));
}

extern "C" CAMLprim value caml_interval_value_ub_bounded(value v_iv) {
    CAMLparam1(v_iv);
    IntervalValue* iv = unwrap_interval_value(v_iv);
    CAMLreturn(wrap_bounded_int(new BoundedInt(iv->ub())));
}

extern "C" CAMLprim value caml_interval_value_is_bottom(value v_iv) {
    CAMLparam1(v_iv); CAMLreturn(Val_bool(unwrap_interval_value(v_iv)->isBottom()));
}
extern "C" CAMLprim value caml_interval_value_is_top(value v_iv) {
    CAMLparam1(v_iv); CAMLreturn(Val_bool(unwrap_interval_value(v_iv)->isTop()));
}
extern "C" CAMLprim value caml_interval_value_is_zero(value v_iv) {
    CAMLparam1(v_iv); CAMLreturn(Val_bool(unwrap_interval_value(v_iv)->is_zero()));
}
extern "C" CAMLprim value caml_interval_value_is_infinity(value v_iv) {
    CAMLparam1(v_iv); CAMLreturn(Val_bool(unwrap_interval_value(v_iv)->is_infinite()));
}
extern "C" CAMLprim value caml_interval_value_is_numeral(value v_iv) {
    CAMLparam1(v_iv);
    IntervalValue* iv = unwrap_interval_value(v_iv);
    CAMLreturn(Val_bool(!iv->isBottom() && !iv->isTop() && !iv->is_infinite()));
}
extern "C" CAMLprim value caml_interval_value_get_numeral(value v_iv) {
    CAMLparam1(v_iv);
    CAMLreturn(Val_int((int)unwrap_interval_value(v_iv)->getNumeral()));
}
extern "C" CAMLprim value caml_interval_value_to_string(value v_iv) {
    CAMLparam1(v_iv);
    std::ostringstream oss;
    oss << unwrap_interval_value(v_iv)->toString();
    CAMLreturn(caml_copy_string(oss.str().c_str()));
}
extern "C" CAMLprim value caml_interval_value_equals(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    CAMLreturn(Val_bool(unwrap_interval_value(v_a)->equals(*unwrap_interval_value(v_b))));
}
extern "C" CAMLprim value caml_interval_value_clone(value v_iv) {
    CAMLparam1(v_iv);
    CAMLreturn(wrap_interval_value(new IntervalValue(*unwrap_interval_value(v_iv))));
}
extern "C" CAMLprim value caml_interval_value_join_with(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    IntervalValue* a = unwrap_interval_value(v_a);
    a->join_with(*unwrap_interval_value(v_b));
    CAMLreturn(Val_unit);
}
extern "C" CAMLprim value caml_interval_value_meet_with(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    IntervalValue* a = unwrap_interval_value(v_a);
    a->meet_with(*unwrap_interval_value(v_b));
    CAMLreturn(Val_unit);
}
extern "C" CAMLprim value caml_interval_value_widen_with(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    IntervalValue* a = unwrap_interval_value(v_a);
    a->widen_with(*unwrap_interval_value(v_b));
    CAMLreturn(Val_unit);
}
extern "C" CAMLprim value caml_interval_value_narrow_with(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    IntervalValue* a = unwrap_interval_value(v_a);
    a->narrow_with(*unwrap_interval_value(v_b));
    CAMLreturn(Val_unit);
}
extern "C" CAMLprim value caml_interval_value_set_to_bottom(value v_iv) {
    CAMLparam1(v_iv);
    unwrap_interval_value(v_iv)->set_to_bottom();
    CAMLreturn(Val_unit);
}
extern "C" CAMLprim value caml_interval_value_set_to_top(value v_iv) {
    CAMLparam1(v_iv);
    unwrap_interval_value(v_iv)->set_to_top();
    CAMLreturn(Val_unit);
}

/* Arithmetic on IntervalValue (return new copies) */
extern "C" CAMLprim value caml_interval_value_add(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    IntervalValue r = *unwrap_interval_value(v_a) + *unwrap_interval_value(v_b);
    CAMLreturn(wrap_interval_value(new IntervalValue(r)));
}
extern "C" CAMLprim value caml_interval_value_sub(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    IntervalValue r = *unwrap_interval_value(v_a) - *unwrap_interval_value(v_b);
    CAMLreturn(wrap_interval_value(new IntervalValue(r)));
}
extern "C" CAMLprim value caml_interval_value_mul(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    IntervalValue r = *unwrap_interval_value(v_a) * *unwrap_interval_value(v_b);
    CAMLreturn(wrap_interval_value(new IntervalValue(r)));
}
extern "C" CAMLprim value caml_interval_value_div(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    IntervalValue r = *unwrap_interval_value(v_a) / *unwrap_interval_value(v_b);
    CAMLreturn(wrap_interval_value(new IntervalValue(r)));
}
extern "C" CAMLprim value caml_interval_value_mod(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    IntervalValue r = *unwrap_interval_value(v_a) % *unwrap_interval_value(v_b);
    CAMLreturn(wrap_interval_value(new IntervalValue(r)));
}
extern "C" CAMLprim value caml_interval_value_and(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    IntervalValue r = *unwrap_interval_value(v_a) & *unwrap_interval_value(v_b);
    CAMLreturn(wrap_interval_value(new IntervalValue(r)));
}
extern "C" CAMLprim value caml_interval_value_or(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    IntervalValue r = *unwrap_interval_value(v_a) | *unwrap_interval_value(v_b);
    CAMLreturn(wrap_interval_value(new IntervalValue(r)));
}
extern "C" CAMLprim value caml_interval_value_xor(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    IntervalValue r = *unwrap_interval_value(v_a) ^ *unwrap_interval_value(v_b);
    CAMLreturn(wrap_interval_value(new IntervalValue(r)));
}
extern "C" CAMLprim value caml_interval_value_lshift(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    IntervalValue r = *unwrap_interval_value(v_a) << *unwrap_interval_value(v_b);
    CAMLreturn(wrap_interval_value(new IntervalValue(r)));
}
extern "C" CAMLprim value caml_interval_value_rshift(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    IntervalValue r = *unwrap_interval_value(v_a) >> *unwrap_interval_value(v_b);
    CAMLreturn(wrap_interval_value(new IntervalValue(r)));
}

/* =========================================================================
 * AddressValue bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_address_value_create(value v_unit) {
    CAMLparam1(v_unit);
    CAMLreturn(wrap_address_value(new AddressValue()));
}

extern "C" CAMLprim value caml_address_value_insert(value v_av, value v_addr) {
    CAMLparam2(v_av, v_addr);
    unwrap_address_value(v_av)->insert((u32_t)Int_val(v_addr));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_address_value_contains(value v_av, value v_addr) {
    CAMLparam2(v_av, v_addr);
    CAMLreturn(Val_bool(unwrap_address_value(v_av)->contains((u32_t)Int_val(v_addr))));
}

extern "C" CAMLprim value caml_address_value_is_bottom(value v_av) {
    CAMLparam1(v_av); CAMLreturn(Val_bool(unwrap_address_value(v_av)->isBottom()));
}

extern "C" CAMLprim value caml_address_value_get_vals(value v_av) {
    CAMLparam1(v_av);
    CAMLlocal1(result);
    AddressValue* av = unwrap_address_value(v_av);
    std::vector<u32_t> addrs;
    for (u32_t a : av->getVals()) addrs.push_back(a);
    result = Val_int(0);
    for (int i = (int)addrs.size() - 1; i >= 0; --i) {
        CAMLlocal2(head, cons);
        head = Val_int((int)addrs[i]);
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, head);
        Store_field(cons, 1, result);
        result = cons;
    }
    CAMLreturn(result);
}

extern "C" CAMLprim value caml_address_value_join_with(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    unwrap_address_value(v_a)->join_with(*unwrap_address_value(v_b));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_address_value_meet_with(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    unwrap_address_value(v_a)->meet_with(*unwrap_address_value(v_b));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_address_value_has_intersect(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    CAMLreturn(Val_bool(unwrap_address_value(v_a)->hasIntersect(*unwrap_address_value(v_b))));
}

extern "C" CAMLprim value caml_address_value_equals(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    CAMLreturn(Val_bool(unwrap_address_value(v_a)->equals(*unwrap_address_value(v_b))));
}

extern "C" CAMLprim value caml_address_value_clone(value v_av) {
    CAMLparam1(v_av);
    CAMLreturn(wrap_address_value(new AddressValue(*unwrap_address_value(v_av))));
}

extern "C" CAMLprim value caml_address_value_to_string(value v_av) {
    CAMLparam1(v_av);
    CAMLreturn(caml_copy_string(unwrap_address_value(v_av)->toString().c_str()));
}

extern "C" CAMLprim value caml_address_value_get_virtual_mem_address(value v_idx) {
    CAMLparam1(v_idx);
    CAMLreturn(Val_int((int)AddressValue::getVirtualMemAddress((u32_t)Int_val(v_idx))));
}

extern "C" CAMLprim value caml_address_value_is_virtual_mem_address(value v_val) {
    CAMLparam1(v_val);
    CAMLreturn(Val_bool(AddressValue::isVirtualMemAddress((u32_t)Int_val(v_val))));
}

/* =========================================================================
 * AbstractValue bindings
 * ========================================================================= */

extern "C" CAMLprim value caml_abstract_value_create_interval(value v_iv) {
    CAMLparam1(v_iv);
    AbstractValue* av = new AbstractValue(*unwrap_interval_value(v_iv));
    CAMLreturn(wrap_abstract_value(av));
}

extern "C" CAMLprim value caml_abstract_value_create_addr(value v_av) {
    CAMLparam1(v_av);
    AbstractValue* av = new AbstractValue(*unwrap_address_value(v_av));
    CAMLreturn(wrap_abstract_value(av));
}

extern "C" CAMLprim value caml_abstract_value_is_interval(value v_av) {
    CAMLparam1(v_av); CAMLreturn(Val_bool(unwrap_abstract_value(v_av)->isInterval()));
}

extern "C" CAMLprim value caml_abstract_value_is_addr(value v_av) {
    CAMLparam1(v_av); CAMLreturn(Val_bool(unwrap_abstract_value(v_av)->isAddr()));
}

extern "C" CAMLprim value caml_abstract_value_get_interval(value v_av) {
    CAMLparam1(v_av);
    AbstractValue* av = unwrap_abstract_value(v_av);
    IntervalValue* iv = new IntervalValue(av->getInterval());
    CAMLreturn(wrap_interval_value(iv));
}

extern "C" CAMLprim value caml_abstract_value_get_addrs(value v_av) {
    CAMLparam1(v_av);
    AbstractValue* av = unwrap_abstract_value(v_av);
    AddressValue* addrs = new AddressValue(av->getAddrs());
    CAMLreturn(wrap_address_value(addrs));
}

extern "C" CAMLprim value caml_abstract_value_equals(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    CAMLreturn(Val_bool(unwrap_abstract_value(v_a)->equals(*unwrap_abstract_value(v_b))));
}

extern "C" CAMLprim value caml_abstract_value_clone(value v_av) {
    CAMLparam1(v_av);
    CAMLreturn(wrap_abstract_value(new AbstractValue(*unwrap_abstract_value(v_av))));
}

extern "C" CAMLprim value caml_abstract_value_to_string(value v_av) {
    CAMLparam1(v_av);
    CAMLreturn(caml_copy_string(unwrap_abstract_value(v_av)->toString().c_str()));
}

extern "C" CAMLprim value caml_abstract_value_join_with(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    AbstractValue* a = unwrap_abstract_value(v_a);
    a->join_with(*unwrap_abstract_value(v_b));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_abstract_value_meet_with(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    AbstractValue* a = unwrap_abstract_value(v_a);
    a->meet_with(*unwrap_abstract_value(v_b));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_abstract_value_widen_with(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    AbstractValue* a = unwrap_abstract_value(v_a);
    a->widen_with(*unwrap_abstract_value(v_b));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_abstract_value_narrow_with(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    AbstractValue* a = unwrap_abstract_value(v_a);
    a->narrow_with(*unwrap_abstract_value(v_b));
    CAMLreturn(Val_unit);
}

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

extern "C" CAMLprim value caml_abstract_state_get_var(value v_as, value v_id) {
    CAMLparam2(v_as, v_id);
    AbstractState* as = unwrap_abstract_state(v_as);
    NodeID id = (NodeID)Int_val(v_id);
    AbstractValue* copy = new AbstractValue((*as)[id]);
    CAMLreturn(wrap_abstract_value(copy));
}

extern "C" CAMLprim value caml_abstract_state_set_var(value v_as, value v_id, value v_val) {
    CAMLparam3(v_as, v_id, v_val);
    AbstractState* as = unwrap_abstract_state(v_as);
    NodeID id = (NodeID)Int_val(v_id);
    (*as)[id] = *unwrap_abstract_value(v_val);
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_abstract_state_store(value v_as, value v_addr, value v_val) {
    CAMLparam3(v_as, v_addr, v_val);
    AbstractState* as = unwrap_abstract_state(v_as);
    u32_t addr = (u32_t)Int_val(v_addr);
    as->store(addr, *unwrap_abstract_value(v_val));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_abstract_state_load(value v_as, value v_addr) {
    CAMLparam2(v_as, v_addr);
    AbstractState* as = unwrap_abstract_state(v_as);
    u32_t addr = (u32_t)Int_val(v_addr);
    AbstractValue* copy = new AbstractValue(as->load(addr));
    CAMLreturn(wrap_abstract_value(copy));
}

extern "C" CAMLprim value caml_abstract_state_equals(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    CAMLreturn(Val_bool(unwrap_abstract_state(v_a)->equals(*unwrap_abstract_state(v_b))));
}

extern "C" CAMLprim value caml_abstract_state_join_with(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    unwrap_abstract_state(v_a)->joinWith(*unwrap_abstract_state(v_b));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_abstract_state_meet_with(value v_a, value v_b) {
    CAMLparam2(v_a, v_b);
    unwrap_abstract_state(v_a)->meetWith(*unwrap_abstract_state(v_b));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_abstract_state_get_id_from_addr(value v_as, value v_addr) {
    CAMLparam2(v_as, v_addr);
    AbstractState* as = unwrap_abstract_state(v_as);
    u32_t result = as->getIDFromAddr((u32_t)Int_val(v_addr));
    CAMLreturn(Val_int((int)result));
}

extern "C" CAMLprim value caml_abstract_state_in_var_to_val_table(value v_as, value v_id) {
    CAMLparam2(v_as, v_id);
    CAMLreturn(Val_bool(unwrap_abstract_state(v_as)->inVarToValTable((u32_t)Int_val(v_id))));
}

extern "C" CAMLprim value caml_abstract_state_in_var_to_addrs_table(value v_as, value v_id) {
    CAMLparam2(v_as, v_id);
    CAMLreturn(Val_bool(unwrap_abstract_state(v_as)->inVarToAddrsTable((u32_t)Int_val(v_id))));
}

extern "C" CAMLprim value caml_abstract_state_in_addr_to_addrs_table(value v_as, value v_id) {
    CAMLparam2(v_as, v_id);
    CAMLreturn(Val_bool(unwrap_abstract_state(v_as)->inAddrToAddrsTable((u32_t)Int_val(v_id))));
}

extern "C" CAMLprim value caml_abstract_state_load_value(value v_as, value v_id) {
    CAMLparam2(v_as, v_id);
    AbstractState* as = unwrap_abstract_state(v_as);
    AbstractValue* copy = new AbstractValue(as->loadValue((NodeID)Int_val(v_id)));
    CAMLreturn(wrap_abstract_value(copy));
}

extern "C" CAMLprim value caml_abstract_state_store_value(value v_as, value v_id, value v_val) {
    CAMLparam3(v_as, v_id, v_val);
    AbstractState* as = unwrap_abstract_state(v_as);
    as->storeValue((NodeID)Int_val(v_id), *unwrap_abstract_value(v_val));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_abstract_state_hash(value v_as) {
    CAMLparam1(v_as);
    CAMLreturn(Val_int((int)unwrap_abstract_state(v_as)->hash()));
}

extern "C" CAMLprim value caml_abstract_state_clear(value v_as) {
    CAMLparam1(v_as);
    unwrap_abstract_state(v_as)->clear();
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_abstract_state_print(value v_as) {
    CAMLparam1(v_as);
    unwrap_abstract_state(v_as)->printAbstractState();
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_abstract_state_add_to_freed_addrs(value v_as, value v_addr) {
    CAMLparam2(v_as, v_addr);
    unwrap_abstract_state(v_as)->addToFreedAddrs((NodeID)Int_val(v_addr));
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_abstract_state_is_freed_mem(value v_as, value v_addr) {
    CAMLparam2(v_as, v_addr);
    CAMLreturn(Val_bool(unwrap_abstract_state(v_as)->isFreedMem((u32_t)Int_val(v_addr))));
}

extern "C" CAMLprim value caml_abstract_state_is_null_mem(value v_addr) {
    CAMLparam1(v_addr);
    CAMLreturn(Val_bool(AbstractState::isNullMem((u32_t)Int_val(v_addr))));
}

extern "C" CAMLprim value caml_abstract_state_is_black_hole_obj_addr(value v_addr) {
    CAMLparam1(v_addr);
    CAMLreturn(Val_bool(AbstractState::isBlackHoleObjAddr((u32_t)Int_val(v_addr))));
}

extern "C" CAMLprim value caml_abstract_state_is_virtual_mem_address(value v_as, value v_val) {
    CAMLparam2(v_as, v_val);
    (void)v_as;
    CAMLreturn(Val_bool(AbstractState::isVirtualMemAddress((u32_t)Int_val(v_val))));
}

extern "C" CAMLprim value caml_abstract_state_get_virtual_mem_address(value v_as, value v_idx) {
    CAMLparam2(v_as, v_idx);
    (void)v_as;
    CAMLreturn(Val_int((int)AbstractState::getVirtualMemAddress((u32_t)Int_val(v_idx))));
}

extern "C" CAMLprim value caml_abstract_state_get_gep_obj_addrs(value v_as, value v_var_id, value v_iv) {
    CAMLparam3(v_as, v_var_id, v_iv);
    AbstractState* as = unwrap_abstract_state(v_as);
    AddressValue result = as->getGepObjAddrs((u32_t)Int_val(v_var_id), *unwrap_interval_value(v_iv));
    CAMLreturn(wrap_address_value(new AddressValue(result)));
}

extern "C" CAMLprim value caml_abstract_state_get_element_index(value v_as, value v_gep) {
    CAMLparam2(v_as, v_gep);
    AbstractState* as = unwrap_abstract_state(v_as);
    GepStmt* gep = SVFUtil::dyn_cast<GepStmt>(unwrap_svf_stmt(v_gep));
    if (!gep) caml_failwith("abstract_state_get_element_index: not a GepStmt");
    IntervalValue result = as->getElementIndex(gep);
    CAMLreturn(wrap_interval_value(new IntervalValue(result)));
}

extern "C" CAMLprim value caml_abstract_state_get_byte_offset(value v_as, value v_gep) {
    CAMLparam2(v_as, v_gep);
    AbstractState* as = unwrap_abstract_state(v_as);
    GepStmt* gep = SVFUtil::dyn_cast<GepStmt>(unwrap_svf_stmt(v_gep));
    if (!gep) caml_failwith("abstract_state_get_byte_offset: not a GepStmt");
    IntervalValue result = as->getByteOffset(gep);
    CAMLreturn(wrap_interval_value(new IntervalValue(result)));
}

extern "C" CAMLprim value caml_abstract_state_get_pointee_element(value v_as, value v_id) {
    CAMLparam2(v_as, v_id);
    AbstractState* as = unwrap_abstract_state(v_as);
    const SVFType* t = as->getPointeeElement((NodeID)Int_val(v_id));
    if (!t) caml_failwith("abstract_state_get_pointee_element: null");
    CAMLreturn(wrap_svf_type(const_cast<SVFType*>(t)));
}

extern "C" CAMLprim value caml_abstract_state_get_alloca_inst_byte_size(value v_as, value v_addr_stmt) {
    CAMLparam2(v_as, v_addr_stmt);
    AbstractState* as = unwrap_abstract_state(v_as);
    AddrStmt* addr = SVFUtil::dyn_cast<AddrStmt>(unwrap_svf_stmt(v_addr_stmt));
    if (!addr) caml_failwith("abstract_state_get_alloca_inst_byte_size: not an AddrStmt");
    u32_t result = as->getAllocaInstByteSize(addr);
    CAMLreturn(Val_int((int)result));
}

extern "C" CAMLprim value caml_abstract_state_init_obj_var(value v_as, value v_obj_var) {
    CAMLparam2(v_as, v_obj_var);
    AbstractState* as = unwrap_abstract_state(v_as);
    ObjVar* obj = SVFUtil::dyn_cast<ObjVar>(unwrap_svf_var(v_obj_var));
    if (!obj) caml_failwith("abstract_state_init_obj_var: not an ObjVar");
    as->initObjVar(obj);
    CAMLreturn(Val_unit);
}

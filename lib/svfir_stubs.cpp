/**
 * svfir_stubs.cpp
 *
 * OCaml C FFI stubs for SVFIR (PAG), SVFStmt, SVFVar, SVFType, FunObjVar.
 * Mirrors SVF-Python/pybind/SVFIR.cpp and related files.
 */

#include "svf_ocaml_types.h"

#include "SVF-LLVM/SVFIRBuilder.h"
#include "Graphs/ICFG.h"
#include "Graphs/CallGraph.h"
#include "Graphs/IRGraph.h"
#include "SVFIR/SVFType.h"
#include "SVFIR/SVFStatements.h"
#include "SVFIR/SVFVariables.h"
#include "Util/SVFUtil.h"
#include "Util/Casting.h"

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

/* SVFStmt */
extern "C" CAMLprim value caml_svf_stmt_to_string(value v_s) {
    CAMLparam1(v_s);
    std::ostringstream oss;
    oss << unwrap_svf_stmt(v_s)->toString();
    CAMLreturn(caml_copy_string(oss.str().c_str()));
}
extern "C" CAMLprim value caml_svf_stmt_get_src_node(value v_s) {
    CAMLparam1(v_s);
    CAMLreturn(wrap_svf_var(unwrap_svf_stmt(v_s)->getSrcNode()));
}
extern "C" CAMLprim value caml_svf_stmt_get_dst_node(value v_s) {
    CAMLparam1(v_s);
    CAMLreturn(wrap_svf_var(unwrap_svf_stmt(v_s)->getDstNode()));
}
extern "C" CAMLprim value caml_svf_stmt_get_src_id(value v_s) {
    CAMLparam1(v_s);
    CAMLreturn(Val_int((int)unwrap_svf_stmt(v_s)->getSrcID()));
}
extern "C" CAMLprim value caml_svf_stmt_get_dst_id(value v_s) {
    CAMLparam1(v_s);
    CAMLreturn(Val_int((int)unwrap_svf_stmt(v_s)->getDstID()));
}
extern "C" CAMLprim value caml_svf_stmt_get_edge_id(value v_s) {
    CAMLparam1(v_s);
    CAMLreturn(Val_int((int)unwrap_svf_stmt(v_s)->getEdgeID()));
}
extern "C" CAMLprim value caml_svf_stmt_get_icfg_node(value v_s) {
    CAMLparam1(v_s);
    ICFGNode* n = unwrap_svf_stmt(v_s)->getICFGNode();
    if (!n) caml_failwith("svf_stmt_get_icfg_node: null");
    CAMLreturn(wrap_icfg_node(n));
}
extern "C" CAMLprim value caml_svf_stmt_get_value(value v_s) {
    CAMLparam1(v_s);
    const SVFVar* v = unwrap_svf_stmt(v_s)->getValue();
    CAMLreturn(wrap_svf_var_opt(const_cast<SVFVar*>(v)));
}
extern "C" CAMLprim value caml_svf_stmt_get_bb(value v_s) {
    CAMLparam1(v_s);
    const SVFBasicBlock* bb = unwrap_svf_stmt(v_s)->getBB();
    CAMLreturn(wrap_svf_basic_block_opt(const_cast<SVFBasicBlock*>(bb)));
}
extern "C" CAMLprim value caml_svf_stmt_is_addr(value v_s) {
    CAMLparam1(v_s); CAMLreturn(Val_bool(unwrap_svf_stmt(v_s)->getEdgeKind() == SVFStmt::Addr));
}
extern "C" CAMLprim value caml_svf_stmt_is_copy(value v_s) {
    CAMLparam1(v_s); CAMLreturn(Val_bool(unwrap_svf_stmt(v_s)->getEdgeKind() == SVFStmt::Copy));
}
extern "C" CAMLprim value caml_svf_stmt_is_store(value v_s) {
    CAMLparam1(v_s); CAMLreturn(Val_bool(unwrap_svf_stmt(v_s)->getEdgeKind() == SVFStmt::Store));
}
extern "C" CAMLprim value caml_svf_stmt_is_load(value v_s) {
    CAMLparam1(v_s); CAMLreturn(Val_bool(unwrap_svf_stmt(v_s)->getEdgeKind() == SVFStmt::Load));
}
extern "C" CAMLprim value caml_svf_stmt_is_call_pe(value v_s) {
    CAMLparam1(v_s); CAMLreturn(Val_bool(unwrap_svf_stmt(v_s)->getEdgeKind() == SVFStmt::Call));
}
extern "C" CAMLprim value caml_svf_stmt_is_ret_pe(value v_s) {
    CAMLparam1(v_s); CAMLreturn(Val_bool(unwrap_svf_stmt(v_s)->getEdgeKind() == SVFStmt::Ret));
}
extern "C" CAMLprim value caml_svf_stmt_is_gep(value v_s) {
    CAMLparam1(v_s); CAMLreturn(Val_bool(unwrap_svf_stmt(v_s)->getEdgeKind() == SVFStmt::Gep));
}
extern "C" CAMLprim value caml_svf_stmt_is_phi(value v_s) {
    CAMLparam1(v_s); CAMLreturn(Val_bool(unwrap_svf_stmt(v_s)->getEdgeKind() == SVFStmt::Phi));
}
extern "C" CAMLprim value caml_svf_stmt_is_select(value v_s) {
    CAMLparam1(v_s); CAMLreturn(Val_bool(unwrap_svf_stmt(v_s)->getEdgeKind() == SVFStmt::Select));
}
extern "C" CAMLprim value caml_svf_stmt_is_cmp(value v_s) {
    CAMLparam1(v_s); CAMLreturn(Val_bool(unwrap_svf_stmt(v_s)->getEdgeKind() == SVFStmt::Cmp));
}
extern "C" CAMLprim value caml_svf_stmt_is_binary_op(value v_s) {
    CAMLparam1(v_s); CAMLreturn(Val_bool(unwrap_svf_stmt(v_s)->getEdgeKind() == SVFStmt::BinaryOp));
}
extern "C" CAMLprim value caml_svf_stmt_is_unary_op(value v_s) {
    CAMLparam1(v_s); CAMLreturn(Val_bool(unwrap_svf_stmt(v_s)->getEdgeKind() == SVFStmt::UnaryOp));
}
extern "C" CAMLprim value caml_svf_stmt_is_branch(value v_s) {
    CAMLparam1(v_s); CAMLreturn(Val_bool(unwrap_svf_stmt(v_s)->getEdgeKind() == SVFStmt::Branch));
}

/* SVFStmt downcast */
extern "C" CAMLprim value caml_svf_stmt_as_addr(value v_s) {
    CAMLparam1(v_s);
    AddrStmt* s = SVFUtil::dyn_cast<AddrStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(wrap_svf_stmt_opt(static_cast<SVFStmt*>(s)));
}
extern "C" CAMLprim value caml_svf_stmt_as_copy(value v_s) {
    CAMLparam1(v_s);
    CopyStmt* s = SVFUtil::dyn_cast<CopyStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(wrap_svf_stmt_opt(static_cast<SVFStmt*>(s)));
}
extern "C" CAMLprim value caml_svf_stmt_as_store(value v_s) {
    CAMLparam1(v_s);
    StoreStmt* s = SVFUtil::dyn_cast<StoreStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(wrap_svf_stmt_opt(static_cast<SVFStmt*>(s)));
}
extern "C" CAMLprim value caml_svf_stmt_as_load(value v_s) {
    CAMLparam1(v_s);
    LoadStmt* s = SVFUtil::dyn_cast<LoadStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(wrap_svf_stmt_opt(static_cast<SVFStmt*>(s)));
}
extern "C" CAMLprim value caml_svf_stmt_as_call_pe(value v_s) {
    CAMLparam1(v_s);
    CallPE* s = SVFUtil::dyn_cast<CallPE>(unwrap_svf_stmt(v_s));
    CAMLreturn(wrap_svf_stmt_opt(static_cast<SVFStmt*>(s)));
}
extern "C" CAMLprim value caml_svf_stmt_as_ret_pe(value v_s) {
    CAMLparam1(v_s);
    RetPE* s = SVFUtil::dyn_cast<RetPE>(unwrap_svf_stmt(v_s));
    CAMLreturn(wrap_svf_stmt_opt(static_cast<SVFStmt*>(s)));
}
extern "C" CAMLprim value caml_svf_stmt_as_gep(value v_s) {
    CAMLparam1(v_s);
    GepStmt* s = SVFUtil::dyn_cast<GepStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(wrap_svf_stmt_opt(static_cast<SVFStmt*>(s)));
}
extern "C" CAMLprim value caml_svf_stmt_as_phi(value v_s) {
    CAMLparam1(v_s);
    PhiStmt* s = SVFUtil::dyn_cast<PhiStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(wrap_svf_stmt_opt(static_cast<SVFStmt*>(s)));
}
extern "C" CAMLprim value caml_svf_stmt_as_select(value v_s) {
    CAMLparam1(v_s);
    SelectStmt* s = SVFUtil::dyn_cast<SelectStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(wrap_svf_stmt_opt(static_cast<SVFStmt*>(s)));
}
extern "C" CAMLprim value caml_svf_stmt_as_cmp(value v_s) {
    CAMLparam1(v_s);
    CmpStmt* s = SVFUtil::dyn_cast<CmpStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(wrap_svf_stmt_opt(static_cast<SVFStmt*>(s)));
}
extern "C" CAMLprim value caml_svf_stmt_as_binary_op(value v_s) {
    CAMLparam1(v_s);
    BinaryOPStmt* s = SVFUtil::dyn_cast<BinaryOPStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(wrap_svf_stmt_opt(static_cast<SVFStmt*>(s)));
}
extern "C" CAMLprim value caml_svf_stmt_as_unary_op(value v_s) {
    CAMLparam1(v_s);
    UnaryOPStmt* s = SVFUtil::dyn_cast<UnaryOPStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(wrap_svf_stmt_opt(static_cast<SVFStmt*>(s)));
}
extern "C" CAMLprim value caml_svf_stmt_as_branch(value v_s) {
    CAMLparam1(v_s);
    BranchStmt* s = SVFUtil::dyn_cast<BranchStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(wrap_svf_stmt_opt(static_cast<SVFStmt*>(s)));
}

/* AssignStmt */
extern "C" CAMLprim value caml_assign_stmt_get_lhs_var(value v_s) {
    CAMLparam1(v_s);
    AssignStmt* s = SVFUtil::dyn_cast<AssignStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not an AssignStmt");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(static_cast<const SVFVar*>(s->getLHSVar()))));
}
extern "C" CAMLprim value caml_assign_stmt_get_rhs_var(value v_s) {
    CAMLparam1(v_s);
    AssignStmt* s = SVFUtil::dyn_cast<AssignStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not an AssignStmt");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(static_cast<const SVFVar*>(s->getRHSVar()))));
}
extern "C" CAMLprim value caml_assign_stmt_get_lhs_var_id(value v_s) {
    CAMLparam1(v_s);
    AssignStmt* s = SVFUtil::dyn_cast<AssignStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not an AssignStmt");
    CAMLreturn(Val_int((int)s->getLHSVarID()));
}
extern "C" CAMLprim value caml_assign_stmt_get_rhs_var_id(value v_s) {
    CAMLparam1(v_s);
    AssignStmt* s = SVFUtil::dyn_cast<AssignStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not an AssignStmt");
    CAMLreturn(Val_int((int)s->getRHSVarID()));
}

/* AddrStmt */
extern "C" CAMLprim value caml_addr_stmt_get_arr_size(value v_s) {
    CAMLparam1(v_s);
    CAMLlocal1(result);
    AddrStmt* s = SVFUtil::dyn_cast<AddrStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not an AddrStmt");
    std::vector<SVFVar*> vars;
    for (SVFVar* v : s->getArrSize()) vars.push_back(v);
    BUILD_LIST_FROM_VEC(vars, wrap_svf_var, result);
    CAMLreturn(result);
}

/* CopyStmt */
extern "C" CAMLprim value caml_copy_stmt_get_copy_kind(value v_s) {
    CAMLparam1(v_s);
    CopyStmt* s = SVFUtil::dyn_cast<CopyStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a CopyStmt");
    CAMLreturn(Val_int((int)s->getCopyKind()));
}
extern "C" CAMLprim value caml_copy_stmt_is_bitcast(value v_s) {
    CAMLparam1(v_s);
    CopyStmt* s = SVFUtil::dyn_cast<CopyStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(Val_bool(s && s->getCopyKind() == CopyStmt::BITCAST));
}
extern "C" CAMLprim value caml_copy_stmt_is_int2ptr(value v_s) {
    CAMLparam1(v_s);
    CopyStmt* s = SVFUtil::dyn_cast<CopyStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(Val_bool(s && s->getCopyKind() == CopyStmt::INTTOPTR));
}
extern "C" CAMLprim value caml_copy_stmt_is_ptr2int(value v_s) {
    CAMLparam1(v_s);
    CopyStmt* s = SVFUtil::dyn_cast<CopyStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(Val_bool(s && s->getCopyKind() == CopyStmt::PTRTOINT));
}
extern "C" CAMLprim value caml_copy_stmt_is_zext(value v_s) {
    CAMLparam1(v_s);
    CopyStmt* s = SVFUtil::dyn_cast<CopyStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(Val_bool(s && s->getCopyKind() == CopyStmt::ZEXT));
}
extern "C" CAMLprim value caml_copy_stmt_is_sext(value v_s) {
    CAMLparam1(v_s);
    CopyStmt* s = SVFUtil::dyn_cast<CopyStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(Val_bool(s && s->getCopyKind() == CopyStmt::SEXT));
}
extern "C" CAMLprim value caml_copy_stmt_is_value_copy(value v_s) {
    CAMLparam1(v_s);
    CopyStmt* s = SVFUtil::dyn_cast<CopyStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(Val_bool(s && s->getCopyKind() == CopyStmt::COPYVAL));
}

/* GepStmt */
extern "C" CAMLprim value caml_gep_stmt_is_constant_offset(value v_s) {
    CAMLparam1(v_s);
    GepStmt* s = SVFUtil::dyn_cast<GepStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(Val_bool(s && s->isConstantOffset()));
}
extern "C" CAMLprim value caml_gep_stmt_get_constant_offset(value v_s) {
    CAMLparam1(v_s);
    GepStmt* s = SVFUtil::dyn_cast<GepStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a GepStmt");
    CAMLreturn(Val_int((int)s->accumulateConstantOffset()));
}
extern "C" CAMLprim value caml_gep_stmt_get_constant_byte_offset(value v_s) {
    CAMLparam1(v_s);
    GepStmt* s = SVFUtil::dyn_cast<GepStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a GepStmt");
    CAMLreturn(Val_int((int)s->accumulateConstantByteOffset()));
}
extern "C" CAMLprim value caml_gep_stmt_get_constant_struct_fld_idx(value v_s) {
    CAMLparam1(v_s);
    GepStmt* s = SVFUtil::dyn_cast<GepStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a GepStmt");
    CAMLreturn(Val_int((int)s->getConstantStructFldIdx()));
}

/* MultiOpndStmt */
extern "C" CAMLprim value caml_multi_opnd_stmt_get_op_var(value v_s, value v_id) {
    CAMLparam2(v_s, v_id);
    MultiOpndStmt* s = SVFUtil::dyn_cast<MultiOpndStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a MultiOpndStmt");
    const ValVar* v = s->getOpVar((u32_t)Int_val(v_id));
    if (!v) caml_failwith("multi_opnd_stmt_get_op_var: null");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(static_cast<const SVFVar*>(v))));
}
extern "C" CAMLprim value caml_multi_opnd_stmt_get_opnd_vars(value v_s) {
    CAMLparam1(v_s);
    CAMLlocal1(result);
    MultiOpndStmt* s = SVFUtil::dyn_cast<MultiOpndStmt>(unwrap_svf_stmt(v_s));
    if (!s) CAMLreturn(Val_int(0));
    std::vector<SVFVar*> vars;
    for (const ValVar* v : s->getOpndVars())
        vars.push_back(const_cast<SVFVar*>(static_cast<const SVFVar*>(v)));
    BUILD_LIST_FROM_VEC(vars, wrap_svf_var, result);
    CAMLreturn(result);
}
extern "C" CAMLprim value caml_multi_opnd_stmt_get_res(value v_s) {
    CAMLparam1(v_s);
    MultiOpndStmt* s = SVFUtil::dyn_cast<MultiOpndStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a MultiOpndStmt");
    const ValVar* r = s->getRes();
    if (!r) caml_failwith("multi_opnd_stmt_get_res: null");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(static_cast<const SVFVar*>(r))));
}
extern "C" CAMLprim value caml_multi_opnd_stmt_get_res_id(value v_s) {
    CAMLparam1(v_s);
    MultiOpndStmt* s = SVFUtil::dyn_cast<MultiOpndStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a MultiOpndStmt");
    CAMLreturn(Val_int((int)s->getResID()));
}
extern "C" CAMLprim value caml_multi_opnd_stmt_get_op_var_num(value v_s) {
    CAMLparam1(v_s);
    MultiOpndStmt* s = SVFUtil::dyn_cast<MultiOpndStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a MultiOpndStmt");
    CAMLreturn(Val_int((int)s->getOpVarNum()));
}

/* PhiStmt */
extern "C" CAMLprim value caml_phi_stmt_get_op_icfg_node(value v_s, value v_idx) {
    CAMLparam2(v_s, v_idx);
    PhiStmt* s = SVFUtil::dyn_cast<PhiStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a PhiStmt");
    const ICFGNode* n = s->getOpICFGNode((u32_t)Int_val(v_idx));
    if (!n) caml_failwith("phi_stmt_get_op_icfg_node: null");
    CAMLreturn(wrap_icfg_node(const_cast<ICFGNode*>(n)));
}
extern "C" CAMLprim value caml_phi_stmt_is_function_ret_phi(value v_s) {
    CAMLparam1(v_s);
    PhiStmt* s = SVFUtil::dyn_cast<PhiStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(Val_bool(s && s->isFunctionRetPhi()));
}

/* SelectStmt */
extern "C" CAMLprim value caml_select_stmt_get_condition_var(value v_s) {
    CAMLparam1(v_s);
    SelectStmt* s = SVFUtil::dyn_cast<SelectStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a SelectStmt");
    const SVFVar* c = s->getCondition();
    if (!c) caml_failwith("select_stmt_get_condition_var: null");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(c)));
}
extern "C" CAMLprim value caml_select_stmt_get_true_value(value v_s) {
    CAMLparam1(v_s);
    SelectStmt* s = SVFUtil::dyn_cast<SelectStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a SelectStmt");
    const ValVar* v = s->getTrueValue();
    if (!v) caml_failwith("select_stmt_get_true_value: null");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(static_cast<const SVFVar*>(v))));
}
extern "C" CAMLprim value caml_select_stmt_get_false_value(value v_s) {
    CAMLparam1(v_s);
    SelectStmt* s = SVFUtil::dyn_cast<SelectStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a SelectStmt");
    const ValVar* v = s->getFalseValue();
    if (!v) caml_failwith("select_stmt_get_false_value: null");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(static_cast<const SVFVar*>(v))));
}

/* CmpStmt */
extern "C" CAMLprim value caml_cmp_stmt_get_predicate(value v_s) {
    CAMLparam1(v_s);
    CmpStmt* s = SVFUtil::dyn_cast<CmpStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a CmpStmt");
    CAMLreturn(Val_int((int)s->getPredicate()));
}

/* BinaryOPStmt */
extern "C" CAMLprim value caml_binary_op_stmt_get_opcode(value v_s) {
    CAMLparam1(v_s);
    BinaryOPStmt* s = SVFUtil::dyn_cast<BinaryOPStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a BinaryOPStmt");
    CAMLreturn(Val_int((int)s->getOpcode()));
}

/* UnaryOPStmt */
extern "C" CAMLprim value caml_unary_op_stmt_get_opcode(value v_s) {
    CAMLparam1(v_s);
    UnaryOPStmt* s = SVFUtil::dyn_cast<UnaryOPStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a UnaryOPStmt");
    CAMLreturn(Val_int((int)s->getOpcode()));
}
extern "C" CAMLprim value caml_unary_op_stmt_get_res(value v_s) {
    CAMLparam1(v_s);
    UnaryOPStmt* s = SVFUtil::dyn_cast<UnaryOPStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a UnaryOPStmt");
    const ValVar* r = s->getRes();
    if (!r) caml_failwith("unary_op_stmt_get_res: null");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(static_cast<const SVFVar*>(r))));
}
extern "C" CAMLprim value caml_unary_op_stmt_get_res_id(value v_s) {
    CAMLparam1(v_s);
    UnaryOPStmt* s = SVFUtil::dyn_cast<UnaryOPStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a UnaryOPStmt");
    CAMLreturn(Val_int((int)s->getResID()));
}
extern "C" CAMLprim value caml_unary_op_stmt_get_op_var(value v_s) {
    CAMLparam1(v_s);
    UnaryOPStmt* s = SVFUtil::dyn_cast<UnaryOPStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a UnaryOPStmt");
    const ValVar* v = s->getOpVar();
    if (!v) caml_failwith("unary_op_stmt_get_op_var: null");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(static_cast<const SVFVar*>(v))));
}
extern "C" CAMLprim value caml_unary_op_stmt_get_op_var_id(value v_s) {
    CAMLparam1(v_s);
    UnaryOPStmt* s = SVFUtil::dyn_cast<UnaryOPStmt>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a UnaryOPStmt");
    CAMLreturn(Val_int((int)s->getOpVarID()));
}

/* BranchStmt */
extern "C" CAMLprim value caml_branch_stmt_get_successors(value v_s) {
    CAMLparam1(v_s);
    CAMLlocal1(result);
    BranchStmt* s = SVFUtil::dyn_cast<BranchStmt>(unwrap_svf_stmt(v_s));
    if (!s) CAMLreturn(Val_int(0));
    std::vector<ICFGNode*> nodes;
    for (const auto& pair : s->getSuccessors())
        nodes.push_back(const_cast<ICFGNode*>(pair.first));
    BUILD_LIST_FROM_VEC(nodes, wrap_icfg_node, result);
    CAMLreturn(result);
}
extern "C" CAMLprim value caml_branch_stmt_get_num_successors(value v_s) {
    CAMLparam1(v_s);
    BranchStmt* s = SVFUtil::dyn_cast<BranchStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(Val_int(s ? (int)s->getNumSuccessors() : 0));
}
extern "C" CAMLprim value caml_branch_stmt_is_conditional(value v_s) {
    CAMLparam1(v_s);
    BranchStmt* s = SVFUtil::dyn_cast<BranchStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(Val_bool(s && s->isConditional()));
}
extern "C" CAMLprim value caml_branch_stmt_is_unconditional(value v_s) {
    CAMLparam1(v_s);
    BranchStmt* s = SVFUtil::dyn_cast<BranchStmt>(unwrap_svf_stmt(v_s));
    CAMLreturn(Val_bool(s && s->isUnconditional()));
}
extern "C" CAMLprim value caml_branch_stmt_get_condition(value v_s) {
    CAMLparam1(v_s);
    BranchStmt* s = SVFUtil::dyn_cast<BranchStmt>(unwrap_svf_stmt(v_s));
    if (!s) CAMLreturn(Val_int(0));
    const ValVar* c = s->getCondition();
    CAMLreturn(wrap_svf_var_opt(const_cast<SVFVar*>(static_cast<const SVFVar*>(c))));
}

/* CallPE */
extern "C" CAMLprim value caml_call_pe_get_call_site(value v_s) {
    CAMLparam1(v_s);
    CallPE* s = SVFUtil::dyn_cast<CallPE>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a CallPE");
    const CallICFGNode* cs = s->getCallSite();
    if (!cs) caml_failwith("call_pe_get_call_site: null");
    CAMLreturn(wrap_icfg_node(const_cast<ICFGNode*>(static_cast<const ICFGNode*>(cs))));
}
extern "C" CAMLprim value caml_call_pe_get_fun_entry_icfg_node(value v_s) {
    CAMLparam1(v_s);
    CallPE* s = SVFUtil::dyn_cast<CallPE>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a CallPE");
    const FunEntryICFGNode* e = s->getFunEntryICFGNode();
    if (!e) caml_failwith("call_pe_get_fun_entry_icfg_node: null");
    CAMLreturn(wrap_icfg_node(const_cast<ICFGNode*>(static_cast<const ICFGNode*>(e))));
}

/* RetPE */
extern "C" CAMLprim value caml_ret_pe_get_call_site(value v_s) {
    CAMLparam1(v_s);
    RetPE* s = SVFUtil::dyn_cast<RetPE>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a RetPE");
    const CallICFGNode* cs = s->getCallSite();
    if (!cs) caml_failwith("ret_pe_get_call_site: null");
    CAMLreturn(wrap_icfg_node(const_cast<ICFGNode*>(static_cast<const ICFGNode*>(cs))));
}
extern "C" CAMLprim value caml_ret_pe_get_fun_exit_icfg_node(value v_s) {
    CAMLparam1(v_s);
    RetPE* s = SVFUtil::dyn_cast<RetPE>(unwrap_svf_stmt(v_s));
    if (!s) caml_failwith("not a RetPE");
    const FunExitICFGNode* e = s->getFunExitICFGNode();
    if (!e) caml_failwith("ret_pe_get_fun_exit_icfg_node: null");
    CAMLreturn(wrap_icfg_node(const_cast<ICFGNode*>(static_cast<const ICFGNode*>(e))));
}

/* SVFVar */
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
extern "C" CAMLprim value caml_svf_var_to_string(value v_var) {
    CAMLparam1(v_var);
    CAMLreturn(caml_copy_string(unwrap_svf_var(v_var)->toString().c_str()));
}
extern "C" CAMLprim value caml_svf_var_is_isolated(value v_var) {
    CAMLparam1(v_var);
    CAMLreturn(Val_bool(unwrap_svf_var(v_var)->isIsolatedNode()));
}
extern "C" CAMLprim value caml_svf_var_is_val_var(value v_var) {
    CAMLparam1(v_var); CAMLreturn(Val_bool(SVFUtil::isa<ValVar>(unwrap_svf_var(v_var))));
}
extern "C" CAMLprim value caml_svf_var_is_obj_var(value v_var) {
    CAMLparam1(v_var); CAMLreturn(Val_bool(SVFUtil::isa<ObjVar>(unwrap_svf_var(v_var))));
}
extern "C" CAMLprim value caml_svf_var_is_gep_val_var(value v_var) {
    CAMLparam1(v_var); CAMLreturn(Val_bool(SVFUtil::isa<GepValVar>(unwrap_svf_var(v_var))));
}
extern "C" CAMLprim value caml_svf_var_is_gep_obj_var(value v_var) {
    CAMLparam1(v_var); CAMLreturn(Val_bool(SVFUtil::isa<GepObjVar>(unwrap_svf_var(v_var))));
}
extern "C" CAMLprim value caml_svf_var_is_fun_obj_var(value v_var) {
    CAMLparam1(v_var); CAMLreturn(Val_bool(SVFUtil::isa<FunObjVar>(unwrap_svf_var(v_var))));
}
extern "C" CAMLprim value caml_svf_var_is_fun_val_var(value v_var) {
    CAMLparam1(v_var); CAMLreturn(Val_bool(SVFUtil::isa<FunValVar>(unwrap_svf_var(v_var))));
}
extern "C" CAMLprim value caml_svf_var_is_arg_val_var(value v_var) {
    CAMLparam1(v_var); CAMLreturn(Val_bool(SVFUtil::isa<ArgValVar>(unwrap_svf_var(v_var))));
}
extern "C" CAMLprim value caml_svf_var_is_ret_val_var(value v_var) {
    CAMLparam1(v_var); CAMLreturn(Val_bool(SVFUtil::isa<RetValPN>(unwrap_svf_var(v_var))));
}
extern "C" CAMLprim value caml_svf_var_is_dummy_val_var(value v_var) {
    CAMLparam1(v_var); CAMLreturn(Val_bool(SVFUtil::isa<DummyValVar>(unwrap_svf_var(v_var))));
}
extern "C" CAMLprim value caml_svf_var_is_dummy_obj_var(value v_var) {
    CAMLparam1(v_var); CAMLreturn(Val_bool(SVFUtil::isa<DummyObjVar>(unwrap_svf_var(v_var))));
}
extern "C" CAMLprim value caml_svf_var_is_const_data_or_agg_data(value v_var) {
    CAMLparam1(v_var);
    CAMLreturn(Val_bool(unwrap_svf_var(v_var)->isConstDataOrAggData()));
}
extern "C" CAMLprim value caml_svf_var_is_const_data_or_agg_data_but_not_null_ptr(value v_var) {
    CAMLparam1(v_var);
    CAMLreturn(Val_bool(unwrap_svf_var(v_var)->isConstDataOrAggDataButNotNullPtr()));
}
extern "C" CAMLprim value caml_svf_var_ptr_in_uncalled_function(value v_var) {
    CAMLparam1(v_var);
    CAMLreturn(Val_bool(unwrap_svf_var(v_var)->ptrInUncalledFunction()));
}
extern "C" CAMLprim value caml_svf_var_as_val_var(value v_var) {
    CAMLparam1(v_var);
    ValVar* v = SVFUtil::dyn_cast<ValVar>(unwrap_svf_var(v_var));
    CAMLreturn(wrap_svf_var_opt(static_cast<SVFVar*>(v)));
}
extern "C" CAMLprim value caml_svf_var_as_obj_var(value v_var) {
    CAMLparam1(v_var);
    ObjVar* v = SVFUtil::dyn_cast<ObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(wrap_svf_var_opt(static_cast<SVFVar*>(v)));
}
extern "C" CAMLprim value caml_svf_var_as_gep_val_var(value v_var) {
    CAMLparam1(v_var);
    GepValVar* v = SVFUtil::dyn_cast<GepValVar>(unwrap_svf_var(v_var));
    CAMLreturn(wrap_svf_var_opt(static_cast<SVFVar*>(v)));
}
extern "C" CAMLprim value caml_svf_var_as_gep_obj_var(value v_var) {
    CAMLparam1(v_var);
    GepObjVar* v = SVFUtil::dyn_cast<GepObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(wrap_svf_var_opt(static_cast<SVFVar*>(v)));
}
extern "C" CAMLprim value caml_svf_var_as_fun_obj_var(value v_var) {
    CAMLparam1(v_var);
    FunObjVar* v = SVFUtil::dyn_cast<FunObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(wrap_fun_obj_var_opt(v));
}
extern "C" CAMLprim value caml_svf_var_as_fun_val_var(value v_var) {
    CAMLparam1(v_var);
    FunValVar* v = SVFUtil::dyn_cast<FunValVar>(unwrap_svf_var(v_var));
    CAMLreturn(wrap_svf_var_opt(static_cast<SVFVar*>(v)));
}
extern "C" CAMLprim value caml_svf_var_as_arg_val_var(value v_var) {
    CAMLparam1(v_var);
    ArgValVar* v = SVFUtil::dyn_cast<ArgValVar>(unwrap_svf_var(v_var));
    CAMLreturn(wrap_svf_var_opt(static_cast<SVFVar*>(v)));
}
extern "C" CAMLprim value caml_svf_var_as_ret_val_var(value v_var) {
    CAMLparam1(v_var);
    RetValPN* v = SVFUtil::dyn_cast<RetValPN>(unwrap_svf_var(v_var));
    CAMLreturn(wrap_svf_var_opt(static_cast<SVFVar*>(v)));
}
extern "C" CAMLprim value caml_svf_var_as_dummy_val_var(value v_var) {
    CAMLparam1(v_var);
    DummyValVar* v = SVFUtil::dyn_cast<DummyValVar>(unwrap_svf_var(v_var));
    CAMLreturn(wrap_svf_var_opt(static_cast<SVFVar*>(v)));
}
extern "C" CAMLprim value caml_svf_var_as_dummy_obj_var(value v_var) {
    CAMLparam1(v_var);
    DummyObjVar* v = SVFUtil::dyn_cast<DummyObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(wrap_svf_var_opt(static_cast<SVFVar*>(v)));
}
extern "C" CAMLprim value caml_svf_var_get_in_edges(value v_var) {
    CAMLparam1(v_var);
    CAMLlocal1(result);
    SVFVar* var = unwrap_svf_var(v_var);
    std::vector<SVFStmt*> stmts;
    for (SVFStmt* s : var->getInEdges()) stmts.push_back(s);
    BUILD_LIST_FROM_VEC(stmts, wrap_svf_stmt, result);
    CAMLreturn(result);
}
extern "C" CAMLprim value caml_svf_var_get_out_edges(value v_var) {
    CAMLparam1(v_var);
    CAMLlocal1(result);
    SVFVar* var = unwrap_svf_var(v_var);
    std::vector<SVFStmt*> stmts;
    for (SVFStmt* s : var->getOutEdges()) stmts.push_back(s);
    BUILD_LIST_FROM_VEC(stmts, wrap_svf_stmt, result);
    CAMLreturn(result);
}

/* GepValVar */
extern "C" CAMLprim value caml_gep_val_var_get_constant_field_idx(value v_var) {
    CAMLparam1(v_var);
    GepValVar* v = SVFUtil::dyn_cast<GepValVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a GepValVar");
    CAMLreturn(Val_int((int)v->getConstantFieldIdx()));
}
extern "C" CAMLprim value caml_gep_val_var_get_base_node(value v_var) {
    CAMLparam1(v_var);
    GepValVar* v = SVFUtil::dyn_cast<GepValVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a GepValVar");
    const ValVar* base = v->getBaseNode();
    if (!base) caml_failwith("gep_val_var_get_base_node: null");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(static_cast<const SVFVar*>(base))));
}
extern "C" CAMLprim value caml_gep_val_var_get_type(value v_var) {
    CAMLparam1(v_var);
    GepValVar* v = SVFUtil::dyn_cast<GepValVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a GepValVar");
    const SVFType* t = v->getType();
    if (!t) caml_failwith("gep_val_var_get_type: null");
    CAMLreturn(wrap_svf_type(const_cast<SVFType*>(t)));
}

/* GepObjVar */
extern "C" CAMLprim value caml_gep_obj_var_get_constant_field_idx(value v_var) {
    CAMLparam1(v_var);
    GepObjVar* v = SVFUtil::dyn_cast<GepObjVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a GepObjVar");
    CAMLreturn(Val_int((int)v->getConstantFieldIdx()));
}
extern "C" CAMLprim value caml_gep_obj_var_get_base_node(value v_var) {
    CAMLparam1(v_var);
    GepObjVar* v = SVFUtil::dyn_cast<GepObjVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a GepObjVar");
    CAMLreturn(Val_int((int)v->getBaseNode()));
}
extern "C" CAMLprim value caml_gep_obj_var_get_type(value v_var) {
    CAMLparam1(v_var);
    GepObjVar* v = SVFUtil::dyn_cast<GepObjVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a GepObjVar");
    const SVFType* t = v->getType();
    if (!t) caml_failwith("gep_obj_var_get_type: null");
    CAMLreturn(wrap_svf_type(const_cast<SVFType*>(t)));
}

/* ArgValVar */
extern "C" CAMLprim value caml_arg_val_var_get_arg_no(value v_var) {
    CAMLparam1(v_var);
    ArgValVar* v = SVFUtil::dyn_cast<ArgValVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not an ArgValVar");
    CAMLreturn(Val_int((int)v->getArgNo()));
}
extern "C" CAMLprim value caml_arg_val_var_is_arg_of_uncalled_function(value v_var) {
    CAMLparam1(v_var);
    ArgValVar* v = SVFUtil::dyn_cast<ArgValVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isArgOfUncalledFunction()));
}

/* BaseObjVar */
extern "C" CAMLprim value caml_base_obj_var_get_type(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a BaseObjVar");
    const SVFType* t = v->getType();
    if (!t) caml_failwith("base_obj_var_get_type: null");
    CAMLreturn(wrap_svf_type(const_cast<SVFType*>(t)));
}
extern "C" CAMLprim value caml_base_obj_var_get_byte_size(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a BaseObjVar");
    CAMLreturn(Val_int((int)v->getByteSizeOfObj()));
}
extern "C" CAMLprim value caml_base_obj_var_get_max_field_offset_limit(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a BaseObjVar");
    CAMLreturn(Val_int((int)v->getMaxFieldOffsetLimit()));
}
extern "C" CAMLprim value caml_base_obj_var_get_num_of_elements(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a BaseObjVar");
    CAMLreturn(Val_int((int)v->getNumOfElements()));
}
extern "C" CAMLprim value caml_base_obj_var_is_field_insensitive(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isFieldInsensitive()));
}
extern "C" CAMLprim value caml_base_obj_var_is_function(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isFunction()));
}
extern "C" CAMLprim value caml_base_obj_var_is_global_obj(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isGlobalObj()));
}
extern "C" CAMLprim value caml_base_obj_var_is_heap(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isHeap()));
}
extern "C" CAMLprim value caml_base_obj_var_is_stack(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isStack()));
}
extern "C" CAMLprim value caml_base_obj_var_is_struct(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isStruct()));
}
extern "C" CAMLprim value caml_base_obj_var_is_array(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isArray()));
}
extern "C" CAMLprim value caml_base_obj_var_is_var_struct(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isVarStruct()));
}
extern "C" CAMLprim value caml_base_obj_var_is_var_array(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isVarArray()));
}
extern "C" CAMLprim value caml_base_obj_var_is_constant_struct(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isConstantStruct()));
}
extern "C" CAMLprim value caml_base_obj_var_is_constant_array(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isConstantArray()));
}
extern "C" CAMLprim value caml_base_obj_var_is_constant_byte_size(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isConstantByteSize()));
}
extern "C" CAMLprim value caml_base_obj_var_is_black_hole_obj(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isBlackHoleObj()));
}
extern "C" CAMLprim value caml_base_obj_var_is_static_obj(value v_var) {
    CAMLparam1(v_var);
    BaseObjVar* v = SVFUtil::dyn_cast<BaseObjVar>(unwrap_svf_var(v_var));
    CAMLreturn(Val_bool(v && v->isStaticObj()));
}

/* ConstIntValVar */
extern "C" CAMLprim value caml_const_int_val_var_get_sext_value(value v_var) {
    CAMLparam1(v_var);
    ConstIntValVar* v = SVFUtil::dyn_cast<ConstIntValVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a ConstIntValVar");
    CAMLreturn(Val_int((int)v->getSExtValue()));
}
extern "C" CAMLprim value caml_const_int_val_var_get_zext_value(value v_var) {
    CAMLparam1(v_var);
    ConstIntValVar* v = SVFUtil::dyn_cast<ConstIntValVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a ConstIntValVar");
    CAMLreturn(Val_int((int)(s64_t)v->getZExtValue()));
}

/* ConstFPValVar */
extern "C" CAMLprim value caml_const_fp_val_var_get_fp_value(value v_var) {
    CAMLparam1(v_var);
    ConstFPValVar* v = SVFUtil::dyn_cast<ConstFPValVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a ConstFPValVar");
    CAMLreturn(caml_copy_double((double)v->getFPValue()));
}

/* ConstIntObjVar */
extern "C" CAMLprim value caml_const_int_obj_var_get_sext_value(value v_var) {
    CAMLparam1(v_var);
    ConstIntObjVar* v = SVFUtil::dyn_cast<ConstIntObjVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a ConstIntObjVar");
    CAMLreturn(Val_int((int)v->getSExtValue()));
}
extern "C" CAMLprim value caml_const_int_obj_var_get_zext_value(value v_var) {
    CAMLparam1(v_var);
    ConstIntObjVar* v = SVFUtil::dyn_cast<ConstIntObjVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a ConstIntObjVar");
    CAMLreturn(Val_int((int)(s64_t)v->getZExtValue()));
}

/* ConstFPObjVar */
extern "C" CAMLprim value caml_const_fp_obj_var_get_fp_value(value v_var) {
    CAMLparam1(v_var);
    ConstFPObjVar* v = SVFUtil::dyn_cast<ConstFPObjVar>(unwrap_svf_var(v_var));
    if (!v) caml_failwith("not a ConstFPObjVar");
    CAMLreturn(caml_copy_double((double)v->getFPValue()));
}

/* FunObjVar */
extern "C" CAMLprim value caml_fun_obj_var_get_name(value v_f) {
    CAMLparam1(v_f);
    CAMLreturn(caml_copy_string(unwrap_fun_obj_var(v_f)->getName().c_str()));
}
extern "C" CAMLprim value caml_fun_obj_var_to_string(value v_f) {
    CAMLparam1(v_f);
    CAMLreturn(caml_copy_string(unwrap_fun_obj_var(v_f)->toString().c_str()));
}
extern "C" CAMLprim value caml_fun_obj_var_is_declaration(value v_f) {
    CAMLparam1(v_f); CAMLreturn(Val_bool(unwrap_fun_obj_var(v_f)->isDeclaration()));
}
extern "C" CAMLprim value caml_fun_obj_var_is_intrinsic(value v_f) {
    CAMLparam1(v_f); CAMLreturn(Val_bool(unwrap_fun_obj_var(v_f)->isIntrinsic()));
}
extern "C" CAMLprim value caml_fun_obj_var_has_address_taken(value v_f) {
    CAMLparam1(v_f); CAMLreturn(Val_bool(unwrap_fun_obj_var(v_f)->hasAddressTaken()));
}
extern "C" CAMLprim value caml_fun_obj_var_is_var_arg(value v_f) {
    CAMLparam1(v_f); CAMLreturn(Val_bool(unwrap_fun_obj_var(v_f)->isVarArg()));
}
extern "C" CAMLprim value caml_fun_obj_var_is_uncalled_function(value v_f) {
    CAMLparam1(v_f); CAMLreturn(Val_bool(unwrap_fun_obj_var(v_f)->isUncalledFunction()));
}
extern "C" CAMLprim value caml_fun_obj_var_has_return(value v_f) {
    CAMLparam1(v_f); CAMLreturn(Val_bool(unwrap_fun_obj_var(v_f)->hasReturn()));
}
extern "C" CAMLprim value caml_fun_obj_var_arg_size(value v_f) {
    CAMLparam1(v_f); CAMLreturn(Val_int((int)unwrap_fun_obj_var(v_f)->arg_size()));
}
extern "C" CAMLprim value caml_fun_obj_var_get_arg(value v_f, value v_idx) {
    CAMLparam2(v_f, v_idx);
    FunObjVar* f = unwrap_fun_obj_var(v_f);
    const ArgValVar* arg = f->getArg((u32_t)Int_val(v_idx));
    if (!arg) caml_failwith("fun_obj_var_get_arg: null");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(static_cast<const SVFVar*>(arg))));
}
extern "C" CAMLprim value caml_fun_obj_var_get_function_type(value v_f) {
    CAMLparam1(v_f);
    const SVFFunctionType* ft = unwrap_fun_obj_var(v_f)->getFunctionType();
    if (!ft) caml_failwith("fun_obj_var_get_function_type: null");
    CAMLreturn(wrap_svf_type(const_cast<SVFType*>(static_cast<const SVFType*>(ft))));
}
extern "C" CAMLprim value caml_fun_obj_var_get_return_type(value v_f) {
    CAMLparam1(v_f);
    const SVFType* rt = unwrap_fun_obj_var(v_f)->getReturnType();
    if (!rt) caml_failwith("fun_obj_var_get_return_type: null");
    CAMLreturn(wrap_svf_type(const_cast<SVFType*>(rt)));
}
extern "C" CAMLprim value caml_fun_obj_var_dominates(value v_f, value v_bb1, value v_bb2) {
    CAMLparam3(v_f, v_bb1, v_bb2);
    FunObjVar* f = unwrap_fun_obj_var(v_f);
    CAMLreturn(Val_bool(f->dominate(unwrap_svf_basic_block(v_bb1), unwrap_svf_basic_block(v_bb2))));
}
extern "C" CAMLprim value caml_fun_obj_var_post_dominate(value v_f, value v_bb1, value v_bb2) {
    CAMLparam3(v_f, v_bb1, v_bb2);
    FunObjVar* f = unwrap_fun_obj_var(v_f);
    CAMLreturn(Val_bool(f->postDominate(unwrap_svf_basic_block(v_bb1), unwrap_svf_basic_block(v_bb2))));
}

/* SVFType */
extern "C" CAMLprim value caml_svf_type_get_kind(value v_t) {
    CAMLparam1(v_t); CAMLreturn(Val_int((int)unwrap_svf_type(v_t)->getKind()));
}
extern "C" CAMLprim value caml_svf_type_get_byte_size(value v_t) {
    CAMLparam1(v_t); CAMLreturn(Val_int((int)unwrap_svf_type(v_t)->getByteSize()));
}
extern "C" CAMLprim value caml_svf_type_is_pointer_ty(value v_t) {
    CAMLparam1(v_t); CAMLreturn(Val_bool(unwrap_svf_type(v_t)->isPointerTy()));
}
extern "C" CAMLprim value caml_svf_type_is_array_ty(value v_t) {
    CAMLparam1(v_t); CAMLreturn(Val_bool(unwrap_svf_type(v_t)->isArrayTy()));
}
extern "C" CAMLprim value caml_svf_type_is_struct_ty(value v_t) {
    CAMLparam1(v_t); CAMLreturn(Val_bool(unwrap_svf_type(v_t)->isStructTy()));
}
extern "C" CAMLprim value caml_svf_type_is_single_value_ty(value v_t) {
    CAMLparam1(v_t); CAMLreturn(Val_bool(unwrap_svf_type(v_t)->isSingleValueType()));
}
extern "C" CAMLprim value caml_svf_type_to_string(value v_t) {
    CAMLparam1(v_t); CAMLreturn(caml_copy_string(unwrap_svf_type(v_t)->toString().c_str()));
}
extern "C" CAMLprim value caml_svf_type_is_pointer_type(value v_t) {
    CAMLparam1(v_t); CAMLreturn(Val_bool(SVFUtil::isa<SVFPointerType>(unwrap_svf_type(v_t))));
}
extern "C" CAMLprim value caml_svf_type_is_integer_type(value v_t) {
    CAMLparam1(v_t); CAMLreturn(Val_bool(SVFUtil::isa<SVFIntegerType>(unwrap_svf_type(v_t))));
}
extern "C" CAMLprim value caml_svf_type_is_function_type(value v_t) {
    CAMLparam1(v_t); CAMLreturn(Val_bool(SVFUtil::isa<SVFFunctionType>(unwrap_svf_type(v_t))));
}
extern "C" CAMLprim value caml_svf_type_is_struct_type(value v_t) {
    CAMLparam1(v_t); CAMLreturn(Val_bool(SVFUtil::isa<SVFStructType>(unwrap_svf_type(v_t))));
}
extern "C" CAMLprim value caml_svf_type_is_array_type(value v_t) {
    CAMLparam1(v_t); CAMLreturn(Val_bool(SVFUtil::isa<SVFArrayType>(unwrap_svf_type(v_t))));
}
extern "C" CAMLprim value caml_svf_type_is_other_type(value v_t) {
    CAMLparam1(v_t); CAMLreturn(Val_bool(SVFUtil::isa<SVFOtherType>(unwrap_svf_type(v_t))));
}
extern "C" CAMLprim value caml_svf_type_as_pointer_type(value v_t) {
    CAMLparam1(v_t);
    SVFPointerType* t = SVFUtil::dyn_cast<SVFPointerType>(unwrap_svf_type(v_t));
    CAMLreturn(wrap_svf_type_opt(static_cast<SVFType*>(t)));
}
extern "C" CAMLprim value caml_svf_type_as_integer_type(value v_t) {
    CAMLparam1(v_t);
    SVFIntegerType* t = SVFUtil::dyn_cast<SVFIntegerType>(unwrap_svf_type(v_t));
    CAMLreturn(wrap_svf_type_opt(static_cast<SVFType*>(t)));
}
extern "C" CAMLprim value caml_svf_type_as_function_type(value v_t) {
    CAMLparam1(v_t);
    SVFFunctionType* t = SVFUtil::dyn_cast<SVFFunctionType>(unwrap_svf_type(v_t));
    CAMLreturn(wrap_svf_type_opt(static_cast<SVFType*>(t)));
}
extern "C" CAMLprim value caml_svf_type_as_struct_type(value v_t) {
    CAMLparam1(v_t);
    SVFStructType* t = SVFUtil::dyn_cast<SVFStructType>(unwrap_svf_type(v_t));
    CAMLreturn(wrap_svf_type_opt(static_cast<SVFType*>(t)));
}
extern "C" CAMLprim value caml_svf_type_as_array_type(value v_t) {
    CAMLparam1(v_t);
    SVFArrayType* t = SVFUtil::dyn_cast<SVFArrayType>(unwrap_svf_type(v_t));
    CAMLreturn(wrap_svf_type_opt(static_cast<SVFType*>(t)));
}
extern "C" CAMLprim value caml_svf_type_as_other_type(value v_t) {
    CAMLparam1(v_t);
    SVFOtherType* t = SVFUtil::dyn_cast<SVFOtherType>(unwrap_svf_type(v_t));
    CAMLreturn(wrap_svf_type_opt(static_cast<SVFType*>(t)));
}
extern "C" CAMLprim value caml_svf_integer_type_is_signed(value v_t) {
    CAMLparam1(v_t);
    SVFIntegerType* t = SVFUtil::dyn_cast<SVFIntegerType>(unwrap_svf_type(v_t));
    if (!t) caml_failwith("not an SVFIntegerType");
    CAMLreturn(Val_bool(t->isSigned()));
}
extern "C" CAMLprim value caml_svf_function_type_get_return_type(value v_t) {
    CAMLparam1(v_t);
    SVFFunctionType* t = SVFUtil::dyn_cast<SVFFunctionType>(unwrap_svf_type(v_t));
    if (!t) caml_failwith("not an SVFFunctionType");
    const SVFType* rt = t->getReturnType();
    if (!rt) caml_failwith("svf_function_type_get_return_type: null");
    CAMLreturn(wrap_svf_type(const_cast<SVFType*>(rt)));
}
extern "C" CAMLprim value caml_svf_struct_type_get_name(value v_t) {
    CAMLparam1(v_t);
    SVFStructType* t = SVFUtil::dyn_cast<SVFStructType>(unwrap_svf_type(v_t));
    if (!t) caml_failwith("not an SVFStructType");
    CAMLreturn(caml_copy_string(t->getName().c_str()));
}
extern "C" CAMLprim value caml_svf_array_type_get_type_of_element(value v_t) {
    CAMLparam1(v_t);
    SVFArrayType* t = SVFUtil::dyn_cast<SVFArrayType>(unwrap_svf_type(v_t));
    if (!t) caml_failwith("not an SVFArrayType");
    const SVFType* et = t->getTypeOfElement();
    if (!et) caml_failwith("svf_array_type_get_type_of_element: null");
    CAMLreturn(wrap_svf_type(const_cast<SVFType*>(et)));
}

/* SVFIR (PAG) */
extern "C" CAMLprim value caml_svfir_get_icfg(value v_pag) {
    CAMLparam1(v_pag);
    ICFG* icfg = unwrap_svfir(v_pag)->getICFG();
    if (!icfg) caml_failwith("svfir_get_icfg: null");
    CAMLreturn(wrap_icfg(icfg));
}
extern "C" CAMLprim value caml_svfir_get_call_graph(value v_pag) {
    CAMLparam1(v_pag);
    CallGraph* cg = const_cast<CallGraph*>(unwrap_svfir(v_pag)->getCallGraph());
    if (!cg) caml_failwith("svfir_get_call_graph: null");
    CAMLreturn(wrap_call_graph(cg));
}
extern "C" CAMLprim value caml_svfir_get_call_sites(value v_pag) {
    CAMLparam1(v_pag);
    CAMLlocal1(result);
    SVFIR* pag = unwrap_svfir(v_pag);
    std::vector<ICFGNode*> nodes;
    for (const CallICFGNode* cs : pag->getCallSiteSet())
        nodes.push_back(const_cast<ICFGNode*>(static_cast<const ICFGNode*>(cs)));
    BUILD_LIST_FROM_VEC(nodes, wrap_icfg_node, result);
    CAMLreturn(result);
}
extern "C" CAMLprim value caml_svfir_get_svf_var_num(value v_pag) {
    CAMLparam1(v_pag);
    CAMLreturn(Val_int((int)unwrap_svfir(v_pag)->getSVFVarNum()));
}
extern "C" CAMLprim value caml_svfir_get_gnode(value v_pag, value v_id) {
    CAMLparam2(v_pag, v_id);
    SVFVar* var = unwrap_svfir(v_pag)->getGNode((NodeID)Int_val(v_id));
    if (!var) caml_failwith("svfir_get_gnode: not found");
    CAMLreturn(wrap_svf_var(var));
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
    CAMLlocal1(result);
    SVFIR* pag = unwrap_svfir(v_pag);
    std::vector<std::pair<NodeID, SVFVar*>> pairs;
    for (auto it = pag->begin(); it != pag->end(); ++it)
        pairs.push_back({it->first, it->second});
    result = Val_int(0);
    for (int i = (int)pairs.size() - 1; i >= 0; --i) {
        CAMLlocal3(pair_val, head, cons);
        (void)head;
        pair_val = caml_alloc(2, 0);
        Store_field(pair_val, 0, Val_int((int)pairs[i].first));
        Store_field(pair_val, 1, wrap_svf_var(pairs[i].second));
        cons = caml_alloc(2, 0);
        Store_field(cons, 0, pair_val);
        Store_field(cons, 1, result);
        result = cons;
    }
    CAMLreturn(result);
}
extern "C" CAMLprim value caml_svfir_get_base_object(value v_pag, value v_id) {
    CAMLparam2(v_pag, v_id);
    SVFIR* pag = unwrap_svfir(v_pag);
    const BaseObjVar* obj = pag->getBaseObject((NodeID)Int_val(v_id));
    if (!obj) caml_failwith("svfir_get_base_object: null");
    CAMLreturn(wrap_svf_var(const_cast<SVFVar*>(static_cast<const SVFVar*>(obj))));
}
extern "C" CAMLprim value caml_svfir_get_fun_obj_var(value v_pag, value v_name) {
    CAMLparam2(v_pag, v_name);
    SVFIR* pag = unwrap_svfir(v_pag);
    const FunObjVar* fun = pag->getFunObjVar(std::string(String_val(v_name)));
    CAMLreturn(wrap_fun_obj_var_opt(const_cast<FunObjVar*>(fun)));
}
extern "C" CAMLprim value caml_svfir_get_fun_ret(value v_pag, value v_fun) {
    CAMLparam2(v_pag, v_fun);
    SVFIR* pag = unwrap_svfir(v_pag);
    const FunObjVar* fun = unwrap_fun_obj_var(v_fun);
    const ValVar* ret = pag->getFunRet(fun);
    CAMLreturn(wrap_svf_var_opt(const_cast<SVFVar*>(static_cast<const SVFVar*>(ret))));
}
extern "C" CAMLprim value caml_svfir_get_gep_obj_var(value v_pag, value v_id, value v_offset) {
    CAMLparam3(v_pag, v_id, v_offset);
    SVFIR* pag = unwrap_svfir(v_pag);
    NodeID result = pag->getGepObjVar((NodeID)Int_val(v_id), (APOffset)Int_val(v_offset));
    CAMLreturn(Val_int((int)result));
}
extern "C" CAMLprim value caml_svfir_get_flattened_elem_idx(value v_pag, value v_type, value v_orig_id) {
    CAMLparam3(v_pag, v_type, v_orig_id);
    SVFIR* pag = unwrap_svfir(v_pag);
    SVFType* t = unwrap_svf_type(v_type);
    u32_t result = pag->getFlattenedElemIdx(t, (u32_t)Int_val(v_orig_id));
    CAMLreturn(Val_int((int)result));
}
extern "C" CAMLprim value caml_svfir_get_num_of_flatten_elements(value v_pag, value v_type) {
    CAMLparam2(v_pag, v_type);
    SVFIR* pag = unwrap_svfir(v_pag);
    SVFType* t = unwrap_svf_type(v_type);
    u32_t result = pag->getNumOfFlattenElements(t);
    CAMLreturn(Val_int((int)result));
}

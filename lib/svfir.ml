(** Svfir — SVFIR (Program Assignment Graph) and related IR types.
    Mirrors SVF-Python/pybind/SVFIR.cpp. *)

open Types

(** {1 SVFStmt — IR statements (PAG edges)} *)
module SvfStmt = struct
  (** [to_string stmt] returns a string representation of [stmt]. *)
  external to_string : svf_stmt -> string = "caml_svf_stmt_to_string"

  (** [get_src_node stmt] returns the source variable. *)
  external get_src_node : svf_stmt -> svf_var = "caml_svf_stmt_get_src_node"

  (** [get_dst_node stmt] returns the destination variable. *)
  external get_dst_node : svf_stmt -> svf_var = "caml_svf_stmt_get_dst_node"

  (** [get_src_id stmt] returns the ID of the source variable. *)
  external get_src_id : svf_stmt -> node_id = "caml_svf_stmt_get_src_id"

  (** [get_dst_id stmt] returns the ID of the destination variable. *)
  external get_dst_id : svf_stmt -> node_id = "caml_svf_stmt_get_dst_id"

  (** [get_edge_id stmt] returns the edge ID of the statement. *)
  external get_edge_id : svf_stmt -> edge_id = "caml_svf_stmt_get_edge_id"

  (** [get_icfg_node stmt] returns the ICFG node this statement belongs to. *)
  external get_icfg_node : svf_stmt -> icfg_node = "caml_svf_stmt_get_icfg_node"

  (** [get_value stmt] returns the LLVM value string for the instruction. *)
  external get_value : svf_stmt -> string = "caml_svf_stmt_get_value"

  (** [get_bb stmt] returns the basic block for this statement, or [None]. *)
  external get_bb : svf_stmt -> svf_basic_block option = "caml_svf_stmt_get_bb"

  (** {2 Type checks — mirrors Python is_*_stmt() methods} *)

  external is_addr_stmt      : svf_stmt -> bool = "caml_svf_stmt_is_addr"
  external is_copy_stmt      : svf_stmt -> bool = "caml_svf_stmt_is_copy"
  external is_store_stmt     : svf_stmt -> bool = "caml_svf_stmt_is_store"
  external is_load_stmt      : svf_stmt -> bool = "caml_svf_stmt_is_load"
  external is_call_pe        : svf_stmt -> bool = "caml_svf_stmt_is_call_pe"
  external is_ret_pe         : svf_stmt -> bool = "caml_svf_stmt_is_ret_pe"
  external is_gep_stmt       : svf_stmt -> bool = "caml_svf_stmt_is_gep"
  external is_phi_stmt       : svf_stmt -> bool = "caml_svf_stmt_is_phi"
  external is_select_stmt    : svf_stmt -> bool = "caml_svf_stmt_is_select"
  external is_cmp_stmt       : svf_stmt -> bool = "caml_svf_stmt_is_cmp"
  external is_binary_op_stmt : svf_stmt -> bool = "caml_svf_stmt_is_binary_op"
  external is_unary_op_stmt  : svf_stmt -> bool = "caml_svf_stmt_is_unary_op"
  external is_branch_stmt    : svf_stmt -> bool = "caml_svf_stmt_is_branch"

  (** {2 Downcast helpers — return None if the cast fails} *)

  external as_addr_stmt      : svf_stmt -> svf_stmt option = "caml_svf_stmt_as_addr"
  external as_copy_stmt      : svf_stmt -> svf_stmt option = "caml_svf_stmt_as_copy"
  external as_store_stmt     : svf_stmt -> svf_stmt option = "caml_svf_stmt_as_store"
  external as_load_stmt      : svf_stmt -> svf_stmt option = "caml_svf_stmt_as_load"
  external as_call_pe        : svf_stmt -> svf_stmt option = "caml_svf_stmt_as_call_pe"
  external as_ret_pe         : svf_stmt -> svf_stmt option = "caml_svf_stmt_as_ret_pe"
  external as_gep_stmt       : svf_stmt -> svf_stmt option = "caml_svf_stmt_as_gep"
  external as_phi_stmt       : svf_stmt -> svf_stmt option = "caml_svf_stmt_as_phi"
  external as_select_stmt    : svf_stmt -> svf_stmt option = "caml_svf_stmt_as_select"
  external as_cmp_stmt       : svf_stmt -> svf_stmt option = "caml_svf_stmt_as_cmp"
  external as_binary_op_stmt : svf_stmt -> svf_stmt option = "caml_svf_stmt_as_binary_op"
  external as_unary_op_stmt  : svf_stmt -> svf_stmt option = "caml_svf_stmt_as_unary_op"
  external as_branch_stmt    : svf_stmt -> svf_stmt option = "caml_svf_stmt_as_branch"

  (** {2 AssignStmt (base of Addr/Copy/Store/Load/Gep)} *)

  (** [get_lhs_var stmt] returns the LHS variable of an assignment statement. *)
  external get_lhs_var    : svf_stmt -> svf_var = "caml_assign_stmt_get_lhs_var"

  (** [get_rhs_var stmt] returns the RHS variable of an assignment statement. *)
  external get_rhs_var    : svf_stmt -> svf_var = "caml_assign_stmt_get_rhs_var"

  (** [get_lhs_var_id stmt] returns the ID of the LHS variable. *)
  external get_lhs_var_id : svf_stmt -> node_id = "caml_assign_stmt_get_lhs_var_id"

  (** [get_rhs_var_id stmt] returns the ID of the RHS variable. *)
  external get_rhs_var_id : svf_stmt -> node_id = "caml_assign_stmt_get_rhs_var_id"

  (** {2 AddrStmt-specific} *)

  (** [get_arr_size stmt] returns the array allocation sizes, or [[]] for non-arrays. *)
  external get_arr_size   : svf_stmt -> int list = "caml_addr_stmt_get_arr_size"

  (** {2 CopyStmt-specific} *)

  (** Copy-statement kind integer values mirror llvm::Instruction opcodes:
      0 = ValueCopy, 1 = Zext, 2 = Sext, 3 = Bitcast, 4 = Trunc,
      5 = IntToPtr, 6 = PtrToInt, 7 = FPExt, 8 = FPTrunc, 9 = FPToUI,
      10 = FPToSI, 11 = UIToFP, 12 = SIToFP *)

  (** [get_copy_kind stmt] returns the integer copy kind. *)
  external get_copy_kind  : svf_stmt -> int  = "caml_copy_stmt_get_copy_kind"
  external is_bitcast     : svf_stmt -> bool = "caml_copy_stmt_is_bitcast"
  external is_int2ptr     : svf_stmt -> bool = "caml_copy_stmt_is_int2ptr"
  external is_ptr2int     : svf_stmt -> bool = "caml_copy_stmt_is_ptr2int"
  external is_zext        : svf_stmt -> bool = "caml_copy_stmt_is_zext"
  external is_sext        : svf_stmt -> bool = "caml_copy_stmt_is_sext"
  external is_value_copy  : svf_stmt -> bool = "caml_copy_stmt_is_value_copy"

  (** {2 GepStmt-specific} *)

  external is_constant_offset          : svf_stmt -> bool  = "caml_gep_stmt_is_constant_offset"
  external get_constant_offset         : svf_stmt -> int   = "caml_gep_stmt_get_constant_offset"
  external get_constant_byte_offset    : svf_stmt -> int   = "caml_gep_stmt_get_constant_byte_offset"
  external get_constant_struct_fld_idx : svf_stmt -> int   = "caml_gep_stmt_get_constant_struct_fld_idx"

  (** {2 MultiOpndStmt (base of Phi/Select/Cmp/BinaryOP/UnaryOP)} *)

  (** [get_op_var stmt i] returns the i-th operand variable. *)
  external get_op_var     : svf_stmt -> int -> svf_var = "caml_multi_opnd_stmt_get_op_var"

  (** [get_opnd_vars stmt] returns all operand variables. *)
  external get_opnd_vars  : svf_stmt -> svf_var list   = "caml_multi_opnd_stmt_get_opnd_vars"

  (** [get_res stmt] returns the result variable. *)
  external get_res        : svf_stmt -> svf_var        = "caml_multi_opnd_stmt_get_res"

  (** [get_res_id stmt] returns the ID of the result variable. *)
  external get_res_id     : svf_stmt -> node_id        = "caml_multi_opnd_stmt_get_res_id"

  (** [get_op_var_num stmt] returns the number of operands. *)
  external get_op_var_num : svf_stmt -> int            = "caml_multi_opnd_stmt_get_op_var_num"

  (** {2 PhiStmt-specific} *)

  (** [get_op_icfg_node stmt i] returns the ICFG node for the i-th phi operand. *)
  external get_op_icfg_node      : svf_stmt -> int -> icfg_node
                                                     = "caml_phi_stmt_get_op_icfg_node"

  (** [is_function_ret_phi stmt] returns [true] if this phi is a function return phi. *)
  external is_function_ret_phi   : svf_stmt -> bool  = "caml_phi_stmt_is_function_ret_phi"

  (** {2 SelectStmt-specific} *)

  external get_condition_var  : svf_stmt -> svf_var = "caml_select_stmt_get_condition_var"
  external get_true_value     : svf_stmt -> svf_var = "caml_select_stmt_get_true_value"
  external get_false_value    : svf_stmt -> svf_var = "caml_select_stmt_get_false_value"

  (** {2 CmpStmt-specific} *)

  (** [get_predicate stmt] returns the integer comparison predicate (LLVM ICmpInst::Predicate). *)
  external get_predicate  : svf_stmt -> int = "caml_cmp_stmt_get_predicate"

  (** {2 BinaryOPStmt-specific} *)

  (** [get_opcode stmt] returns the integer binary opcode (LLVM BinaryOperator::BinaryOps). *)
  external get_opcode     : svf_stmt -> int = "caml_binary_op_stmt_get_opcode"

  (** {2 UnaryOPStmt-specific} *)

  (** [get_unary_opcode stmt] returns the integer unary opcode. *)
  external get_unary_opcode : svf_stmt -> int    = "caml_unary_op_stmt_get_opcode"

  (** [get_unary_res stmt] returns the result variable. *)
  external get_unary_res    : svf_stmt -> svf_var = "caml_unary_op_stmt_get_res"

  (** [get_unary_res_id stmt] returns the ID of the result variable. *)
  external get_unary_res_id : svf_stmt -> node_id = "caml_unary_op_stmt_get_res_id"

  (** [get_unary_op_var stmt] returns the operand variable. *)
  external get_unary_op_var : svf_stmt -> svf_var = "caml_unary_op_stmt_get_op_var"

  (** [get_unary_op_var_id stmt] returns the ID of the operand variable. *)
  external get_unary_op_var_id : svf_stmt -> node_id = "caml_unary_op_stmt_get_op_var_id"

  (** {2 BranchStmt-specific} *)

  (** [branch_get_successors stmt] returns the list of [(icfg_node, condition_value)] pairs.
      The condition_value is an integer (-1 for unconditional, 0 for false, 1 for true). *)
  external branch_get_successors  : svf_stmt -> (icfg_node * int) list
                                                     = "caml_branch_stmt_get_successors"

  external branch_get_num_successors : svf_stmt -> int  = "caml_branch_stmt_get_num_successors"
  external is_conditional         : svf_stmt -> bool = "caml_branch_stmt_is_conditional"
  external is_unconditional       : svf_stmt -> bool = "caml_branch_stmt_is_unconditional"
  external get_branch_condition   : svf_stmt -> svf_var option = "caml_branch_stmt_get_condition"

  (** {2 CallPE-specific} *)

  (** [call_pe_get_call_site stmt] returns the call ICFG node. *)
  external call_pe_get_call_site       : svf_stmt -> icfg_node = "caml_call_pe_get_call_site"

  (** [call_pe_get_fun_entry_icfg_node stmt] returns the entry ICFG node of the callee. *)
  external call_pe_get_fun_entry_icfg_node : svf_stmt -> icfg_node
                                                     = "caml_call_pe_get_fun_entry_icfg_node"

  (** {2 RetPE-specific} *)

  (** [ret_pe_get_call_site stmt] returns the call ICFG node. *)
  external ret_pe_get_call_site        : svf_stmt -> icfg_node = "caml_ret_pe_get_call_site"

  (** [ret_pe_get_fun_exit_icfg_node stmt] returns the exit ICFG node of the callee. *)
  external ret_pe_get_fun_exit_icfg_node : svf_stmt -> icfg_node
                                                     = "caml_ret_pe_get_fun_exit_icfg_node"
end

(** {1 SvfVar — IR variables (PAG nodes)} *)
module SvfVar = struct
  (** [get_name var] returns the name of [var]. *)
  external get_name       : svf_var -> string  = "caml_svf_var_get_name"

  (** [get_id var] returns the node ID of [var]. *)
  external get_id         : svf_var -> node_id = "caml_svf_var_get_id"

  (** [is_pointer var] returns [true] if [var] is a pointer variable. *)
  external is_pointer     : svf_var -> bool    = "caml_svf_var_is_pointer"

  (** [get_value_name var] returns the value name. *)
  external get_value_name : svf_var -> string  = "caml_svf_var_get_value_name"

  (** [to_string var] returns a verbose string representation. *)
  external to_string      : svf_var -> string  = "caml_svf_var_to_string"

  external is_isolated    : svf_var -> bool    = "caml_svf_var_is_isolated"

  (** {2 Kind predicates} *)
  external is_val_var     : svf_var -> bool    = "caml_svf_var_is_val_var"
  external is_obj_var     : svf_var -> bool    = "caml_svf_var_is_obj_var"
  external is_gep_val_var : svf_var -> bool    = "caml_svf_var_is_gep_val_var"
  external is_gep_obj_var : svf_var -> bool    = "caml_svf_var_is_gep_obj_var"
  external is_fun_obj_var : svf_var -> bool    = "caml_svf_var_is_fun_obj_var"
  external is_fun_val_var : svf_var -> bool    = "caml_svf_var_is_fun_val_var"
  external is_arg_val_var : svf_var -> bool    = "caml_svf_var_is_arg_val_var"
  external is_ret_val_var : svf_var -> bool    = "caml_svf_var_is_ret_val_var"
  external is_dummy_val_var : svf_var -> bool  = "caml_svf_var_is_dummy_val_var"
  external is_dummy_obj_var : svf_var -> bool  = "caml_svf_var_is_dummy_obj_var"
  external is_const_data_or_agg_data : svf_var -> bool
                                               = "caml_svf_var_is_const_data_or_agg_data"
  external is_const_data_or_agg_data_but_not_null_ptr : svf_var -> bool
                                               = "caml_svf_var_is_const_data_or_agg_data_but_not_null_ptr"
  external ptr_in_uncalled_function : svf_var -> bool
                                               = "caml_svf_var_ptr_in_uncalled_function"

  (** {2 Downcast helpers} *)
  external as_val_var      : svf_var -> svf_var option = "caml_svf_var_as_val_var"
  external as_obj_var      : svf_var -> svf_var option = "caml_svf_var_as_obj_var"
  external as_gep_val_var  : svf_var -> svf_var option = "caml_svf_var_as_gep_val_var"
  external as_gep_obj_var  : svf_var -> svf_var option = "caml_svf_var_as_gep_obj_var"
  external as_fun_obj_var  : svf_var -> svf_var option = "caml_svf_var_as_fun_obj_var"
  external as_fun_val_var  : svf_var -> svf_var option = "caml_svf_var_as_fun_val_var"
  external as_arg_val_var  : svf_var -> svf_var option = "caml_svf_var_as_arg_val_var"
  external as_ret_val_var  : svf_var -> svf_var option = "caml_svf_var_as_ret_val_var"
  external as_dummy_val_var : svf_var -> svf_var option = "caml_svf_var_as_dummy_val_var"
  external as_dummy_obj_var : svf_var -> svf_var option = "caml_svf_var_as_dummy_obj_var"

  (** [get_in_edges var] returns the list of incoming SVF statements. *)
  external get_in_edges   : svf_var -> svf_stmt list = "caml_svf_var_get_in_edges"

  (** [get_out_edges var] returns the list of outgoing SVF statements. *)
  external get_out_edges  : svf_var -> svf_stmt list = "caml_svf_var_get_out_edges"

  (** {2 GepValVar-specific} *)

  external gep_val_get_constant_field_idx : svf_var -> int       = "caml_gep_val_var_get_constant_field_idx"
  external gep_val_get_base_node          : svf_var -> svf_var   = "caml_gep_val_var_get_base_node"
  external gep_val_get_type               : svf_var -> svf_type  = "caml_gep_val_var_get_type"

  (** {2 GepObjVar-specific} *)

  external gep_obj_get_constant_field_idx : svf_var -> int       = "caml_gep_obj_var_get_constant_field_idx"
  external gep_obj_get_base_node          : svf_var -> svf_var   = "caml_gep_obj_var_get_base_node"
  external gep_obj_get_type               : svf_var -> svf_type  = "caml_gep_obj_var_get_type"

  (** {2 ArgValVar-specific} *)

  external get_arg_no                     : svf_var -> int       = "caml_arg_val_var_get_arg_no"
  external is_arg_of_uncalled_function    : svf_var -> bool      = "caml_arg_val_var_is_arg_of_uncalled_function"

  (** {2 BaseObjVar / ObjVar predicates} *)

  external base_obj_get_type              : svf_var -> svf_type  = "caml_base_obj_var_get_type"
  external get_byte_size                  : svf_var -> int       = "caml_base_obj_var_get_byte_size"
  external get_max_field_offset_limit     : svf_var -> int       = "caml_base_obj_var_get_max_field_offset_limit"
  external get_num_of_elements            : svf_var -> int       = "caml_base_obj_var_get_num_of_elements"
  external is_field_insensitive           : svf_var -> bool      = "caml_base_obj_var_is_field_insensitive"
  external is_function_obj                : svf_var -> bool      = "caml_base_obj_var_is_function"
  external is_global_obj                  : svf_var -> bool      = "caml_base_obj_var_is_global_obj"
  external is_heap_obj                    : svf_var -> bool      = "caml_base_obj_var_is_heap"
  external is_stack_obj                   : svf_var -> bool      = "caml_base_obj_var_is_stack"
  external is_struct_obj                  : svf_var -> bool      = "caml_base_obj_var_is_struct"
  external is_array_obj                   : svf_var -> bool      = "caml_base_obj_var_is_array"
  external is_var_struct                  : svf_var -> bool      = "caml_base_obj_var_is_var_struct"
  external is_var_array                   : svf_var -> bool      = "caml_base_obj_var_is_var_array"
  external is_constant_struct             : svf_var -> bool      = "caml_base_obj_var_is_constant_struct"
  external is_constant_array              : svf_var -> bool      = "caml_base_obj_var_is_constant_array"
  external is_constant_byte_size          : svf_var -> bool      = "caml_base_obj_var_is_constant_byte_size"
  external is_black_hole_obj              : svf_var -> bool      = "caml_base_obj_var_is_black_hole_obj"
  external is_static_obj                  : svf_var -> bool      = "caml_base_obj_var_is_static_obj"

  (** {2 ConstIntValVar} *)

  external get_sext_value   : svf_var -> int   = "caml_const_int_val_var_get_sext_value"
  external get_zext_value   : svf_var -> int   = "caml_const_int_val_var_get_zext_value"

  (** {2 ConstIntObjVar — for object-type constant integers} *)

  external get_obj_sext_value : svf_var -> int = "caml_const_int_obj_var_get_sext_value"
  external get_obj_zext_value : svf_var -> int = "caml_const_int_obj_var_get_zext_value"

  (** {2 ConstFPValVar} *)

  external get_fp_value     : svf_var -> float = "caml_const_fp_val_var_get_fp_value"

  (** {2 ConstFPObjVar — for object-type constant floats} *)

  external get_obj_fp_value : svf_var -> float = "caml_const_fp_obj_var_get_fp_value"
end

(** {1 FunObjVar — function object variable with rich API} *)
module FunObjVar = struct
  (** [get_name f] returns the function name. *)
  external get_name          : fun_obj_var -> string       = "caml_fun_obj_var_get_name"

  (** [to_string f] returns a verbose string representation. *)
  external to_string         : fun_obj_var -> string       = "caml_fun_obj_var_to_string"

  external is_declaration    : fun_obj_var -> bool         = "caml_fun_obj_var_is_declaration"
  external is_intrinsic      : fun_obj_var -> bool         = "caml_fun_obj_var_is_intrinsic"
  external is_var_arg        : fun_obj_var -> bool         = "caml_fun_obj_var_is_var_arg"
  external is_uncalled_function : fun_obj_var -> bool      = "caml_fun_obj_var_is_uncalled_function"
  external has_address_taken : fun_obj_var -> bool         = "caml_fun_obj_var_has_address_taken"
  external has_return        : fun_obj_var -> bool         = "caml_fun_obj_var_has_return"

  (** [arg_size f] returns the number of formal parameters. *)
  external arg_size          : fun_obj_var -> int          = "caml_fun_obj_var_arg_size"

  (** [get_arg f i] returns the i-th formal parameter variable, or [None]. *)
  external get_arg           : fun_obj_var -> int -> svf_var option
                                                           = "caml_fun_obj_var_get_arg"

  (** [get_function_type f] returns the function's SVF type. *)
  external get_function_type : fun_obj_var -> svf_type     = "caml_fun_obj_var_get_function_type"

  (** [get_return_type f] returns the return type. *)
  external get_return_type   : fun_obj_var -> svf_type     = "caml_fun_obj_var_get_return_type"

  (** [dominates f bb1 bb2] returns [true] if [bb1] dominates [bb2] in [f]. *)
  external dominates         : fun_obj_var -> svf_basic_block -> svf_basic_block -> bool
                                                           = "caml_fun_obj_var_dominates"

  (** [post_dominate f bb1 bb2] returns [true] if [bb1] post-dominates [bb2] in [f]. *)
  external post_dominate     : fun_obj_var -> svf_basic_block -> svf_basic_block -> bool
                                                           = "caml_fun_obj_var_post_dominate"
end

(** {1 SVFType — SVF type descriptors} *)
module SvfType = struct
  (** [get_kind t] returns the integer kind tag. *)
  external get_kind          : svf_type -> int    = "caml_svf_type_get_kind"

  (** [get_byte_size t] returns the byte size of the type, or 0 if unknown. *)
  external get_byte_size     : svf_type -> int    = "caml_svf_type_get_byte_size"

  external is_pointer_ty     : svf_type -> bool   = "caml_svf_type_is_pointer_ty"
  external is_array_ty       : svf_type -> bool   = "caml_svf_type_is_array_ty"
  external is_struct_ty      : svf_type -> bool   = "caml_svf_type_is_struct_ty"
  external is_single_value_ty : svf_type -> bool  = "caml_svf_type_is_single_value_ty"

  (** [to_string t] returns a textual description of [t]. *)
  external to_string         : svf_type -> string = "caml_svf_type_to_string"

  (** {2 Kind predicates (mirrors Python SVFType.is_*_type)} *)
  external is_integer_type   : svf_type -> bool   = "caml_svf_type_is_integer_type"
  external is_function_type  : svf_type -> bool   = "caml_svf_type_is_function_type"
  external is_struct_type    : svf_type -> bool   = "caml_svf_type_is_struct_type"
  external is_array_type     : svf_type -> bool   = "caml_svf_type_is_array_type"
  external is_pointer_type   : svf_type -> bool   = "caml_svf_type_is_pointer_type"
  external is_other_type     : svf_type -> bool   = "caml_svf_type_is_other_type"

  (** {2 Downcast helpers} *)
  external as_pointer_type   : svf_type -> svf_type option = "caml_svf_type_as_pointer_type"
  external as_integer_type   : svf_type -> svf_type option = "caml_svf_type_as_integer_type"
  external as_function_type  : svf_type -> svf_type option = "caml_svf_type_as_function_type"
  external as_struct_type    : svf_type -> svf_type option = "caml_svf_type_as_struct_type"
  external as_array_type     : svf_type -> svf_type option = "caml_svf_type_as_array_type"
  external as_other_type     : svf_type -> svf_type option = "caml_svf_type_as_other_type"

  (** {2 SVFIntegerType-specific} *)

  (** [is_signed t] returns [true] if this is a signed integer type.
      Requires [is_integer_type t]. *)
  external is_signed : svf_type -> bool   = "caml_svf_integer_type_is_signed"

  (** {2 SVFFunctionType-specific} *)

  (** [fn_get_return_type t] returns the return type of a function type.
      Requires [is_function_type t]. *)
  external fn_get_return_type : svf_type -> svf_type = "caml_svf_function_type_get_return_type"

  (** {2 SVFStructType-specific} *)

  (** [struct_get_name t] returns the struct name.
      Requires [is_struct_type t]. *)
  external struct_get_name : svf_type -> string = "caml_svf_struct_type_get_name"

  (** {2 SVFArrayType-specific} *)

  (** [array_get_type_of_element t] returns the element type.
      Requires [is_array_type t]. *)
  external array_get_type_of_element : svf_type -> svf_type
                                                 = "caml_svf_array_type_get_type_of_element"
end

(** {1 SVFIR (PAG) — top-level graph} *)

(** [get_icfg pag] returns the ICFG associated with [pag]. *)
external get_icfg : svfir -> icfg = "caml_svfir_get_icfg"

(** [get_call_graph pag] returns the call graph associated with [pag]. *)
external get_call_graph : svfir -> call_graph = "caml_svfir_get_call_graph"

(** [get_call_sites pag] returns the list of all call-site ICFG nodes. *)
external get_call_sites : svfir -> icfg_node list = "caml_svfir_get_call_sites"

(** [get_svf_var_num pag] returns the number of SVF variables. *)
external get_svf_var_num : svfir -> int = "caml_svfir_get_svf_var_num"

(** [get_gnode pag id] returns the SVF variable with ID [id]. *)
external get_gnode : svfir -> node_id -> svf_var = "caml_svfir_get_gnode"

(** [get_module_identifier pag] returns the module identifier string. *)
external get_module_identifier : svfir -> string = "caml_svfir_get_module_identifier"

(** [dump pag filename] dumps the PAG to a file. *)
external dump : svfir -> string -> unit = "caml_svfir_dump"

(** [iter pag] returns a list of [(node_id, svf_var)] pairs for all PAG nodes. *)
external iter : svfir -> (node_id * svf_var) list = "caml_svfir_iter"

(** [get_base_object pag id] returns the base object variable for object ID [id].
    Raises [Failure] if not found. *)
external get_base_object : svfir -> node_id -> svf_var = "caml_svfir_get_base_object"

(** [get_fun_obj_var pag name] returns the FunObjVar for function [name],
    or [None] if not found. *)
external get_fun_obj_var : svfir -> string -> fun_obj_var option = "caml_svfir_get_fun_obj_var"

(** [get_fun_ret pag fun_obj] returns the return variable of [fun_obj],
    or [None] if the function has no return. *)
external get_fun_ret : svfir -> fun_obj_var -> svf_var option = "caml_svfir_get_fun_ret"

(** [get_gep_obj_var pag obj_id offset] returns the GEP object variable for
    [obj_id] at field [offset]. *)
external get_gep_obj_var : svfir -> node_id -> int -> svf_var = "caml_svfir_get_gep_obj_var"

(** [get_flattened_elem_idx pag ty offset] returns the flattened element index
    for [ty] at byte offset [offset]. *)
external get_flattened_elem_idx : svfir -> svf_type -> int -> int
                                                       = "caml_svfir_get_flattened_elem_idx"

(** [get_num_of_flatten_elements pag ty] returns the total number of flattened
    elements in [ty]. *)
external get_num_of_flatten_elements : svfir -> svf_type -> int
                                                       = "caml_svfir_get_num_of_flatten_elements"

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

  external is_isolated    : svf_var -> bool    = "caml_svf_var_is_isolated"
  external is_val_var     : svf_var -> bool    = "caml_svf_var_is_val_var"
  external is_obj_var     : svf_var -> bool    = "caml_svf_var_is_obj_var"
  external is_fun_obj_var : svf_var -> bool    = "caml_svf_var_is_fun_obj_var"

  (** [get_in_edges var] returns the list of incoming SVF statements. *)
  external get_in_edges   : svf_var -> svf_stmt list = "caml_svf_var_get_in_edges"

  (** [get_out_edges var] returns the list of outgoing SVF statements. *)
  external get_out_edges  : svf_var -> svf_stmt list = "caml_svf_var_get_out_edges"
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

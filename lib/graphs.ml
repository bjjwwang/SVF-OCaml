(** Graphs — ICFG, CallGraph, SVFG and related graph structures.
    Mirrors SVF-Python/pybind/Graphs.cpp. *)

open Types

(** {1 ICFGNode — nodes in the ICFG} *)
module IcfgNode = struct
  (** [to_string node] returns a human-readable representation of [node]. *)
  external to_string     : icfg_node -> string       = "caml_icfg_node_to_string"

  (** [get_id node] returns the ID of [node]. *)
  external get_id        : icfg_node -> node_id      = "caml_icfg_node_get_id"

  (** [get_svf_stmts node] returns the list of SVF statements at [node]. *)
  external get_svf_stmts : icfg_node -> svf_stmt list = "caml_icfg_node_get_svf_stmts"

  (** [get_out_edges node] returns the outgoing ICFG edges. *)
  external get_out_edges : icfg_node -> icfg_edge list = "caml_icfg_node_get_out_edges"

  (** [get_in_edges node] returns the incoming ICFG edges. *)
  external get_in_edges  : icfg_node -> icfg_edge list = "caml_icfg_node_get_in_edges"

  (** {2 Node-kind predicates} *)

  (** [is_fun_entry node] returns [true] if [node] is a function entry node. *)
  external is_fun_entry  : icfg_node -> bool         = "caml_icfg_node_is_fun_entry"

  (** [is_fun_exit node] returns [true] if [node] is a function exit node. *)
  external is_fun_exit   : icfg_node -> bool         = "caml_icfg_node_is_fun_exit"

  (** [is_call node] returns [true] if [node] is a call site node. *)
  external is_call       : icfg_node -> bool         = "caml_icfg_node_is_call"

  (** [is_ret node] returns [true] if [node] is a return site node. *)
  external is_ret        : icfg_node -> bool         = "caml_icfg_node_is_ret"

  (** [get_fun_name node] returns the name of the function owning [node],
      or [""] for global nodes. *)
  external get_fun_name  : icfg_node -> string       = "caml_icfg_node_get_fun_name"

  (** [get_fun node] returns the function object variable for [node],
      or [None] for global nodes. *)
  external get_fun       : icfg_node -> fun_obj_var option = "caml_icfg_node_get_fun"

  (** [get_bb node] returns the basic block for [node], or [None] for
      special (entry/exit/global) nodes. *)
  external get_bb        : icfg_node -> svf_basic_block option = "caml_icfg_node_get_bb"

  (** [get_source_loc node] returns a string describing the source location. *)
  external get_source_loc : icfg_node -> string      = "caml_icfg_node_get_source_loc"

  (** {2 Downcast helpers — return None if the cast fails} *)

  (** [as_fun_entry node] casts [node] to a FunEntryICFGNode, or [None]. *)
  external as_fun_entry  : icfg_node -> icfg_node option = "caml_icfg_node_as_fun_entry"

  (** [as_fun_exit node] casts [node] to a FunExitICFGNode, or [None]. *)
  external as_fun_exit   : icfg_node -> icfg_node option = "caml_icfg_node_as_fun_exit"

  (** [as_call node] casts [node] to a CallICFGNode, or [None]. *)
  external as_call       : icfg_node -> icfg_node option = "caml_icfg_node_as_call"

  (** [as_ret node] casts [node] to a RetICFGNode, or [None]. *)
  external as_ret        : icfg_node -> icfg_node option = "caml_icfg_node_as_ret"

  (** {2 IntraICFGNode} *)

  (** [is_ret_inst node] returns [true] if this intra-CFG node is a return instruction. *)
  external is_ret_inst   : icfg_node -> bool         = "caml_intra_icfg_node_is_ret_inst"

  (** {2 FunEntryICFGNode-specific} *)

  (** [get_formal_parms node] returns the formal parameter variables.
      Requires [is_fun_entry node]. *)
  external get_formal_parms : icfg_node -> svf_var list = "caml_fun_entry_icfg_node_get_formal_parms"

  (** {2 FunExitICFGNode-specific} *)

  (** [get_formal_ret node] returns the formal return variable.
      Requires [is_fun_exit node]. *)
  external get_formal_ret   : icfg_node -> svf_var option = "caml_fun_exit_icfg_node_get_formal_ret"

  (** {2 CallICFGNode-specific} *)

  (** [get_called_function node] returns the static callee, or [None] for
      indirect calls. Requires [is_call node]. *)
  external get_called_function : icfg_node -> fun_obj_var option
                                                     = "caml_call_icfg_node_get_called_function"

  (** [get_actual_parms node] returns the actual argument variables.
      Requires [is_call node]. *)
  external get_actual_parms : icfg_node -> svf_var list
                                                     = "caml_call_icfg_node_get_actual_parms"

  (** [get_argument node i] returns the i-th actual argument.
      Requires [is_call node]. *)
  external get_argument    : icfg_node -> int -> svf_var option
                                                     = "caml_call_icfg_node_get_argument"

  (** [is_var_arg node] returns [true] if the callee is variadic.
      Requires [is_call node]. *)
  external is_var_arg      : icfg_node -> bool       = "caml_call_icfg_node_is_var_arg"

  (** [is_virtual_call node] returns [true] if this is a virtual call.
      Requires [is_call node]. *)
  external is_virtual_call : icfg_node -> bool       = "caml_call_icfg_node_is_virtual_call"

  (** [get_ret_icfg_node node] returns the corresponding return ICFG node.
      Requires [is_call node]. *)
  external get_ret_icfg_node : icfg_node -> icfg_node = "caml_call_icfg_node_get_ret_icfg_node"

  (** [get_caller_entry node] returns the entry ICFG node of the caller function.
      Requires [is_call node]. *)
  external get_caller_entry  : icfg_node -> icfg_node = "caml_call_icfg_node_get_caller_entry"

  (** {2 RetICFGNode-specific} *)

  (** [get_actual_ret node] returns the actual return variable, or [None].
      Requires [is_ret node]. *)
  external get_actual_ret   : icfg_node -> svf_var option = "caml_ret_icfg_node_get_actual_ret"

  (** [get_call_icfg_node node] returns the matching call ICFG node.
      Requires [is_ret node]. *)
  external get_call_icfg_node : icfg_node -> icfg_node = "caml_ret_icfg_node_get_call_icfg_node"
end

(** {1 ICFGEdge — edges in the ICFG} *)
module IcfgEdge = struct
  (** [to_string edge] returns a string representation of [edge]. *)
  external to_string          : icfg_edge -> string   = "caml_icfg_edge_to_string"

  external is_cfg_edge        : icfg_edge -> bool     = "caml_icfg_edge_is_cfg_edge"
  external is_call_cfg_edge   : icfg_edge -> bool     = "caml_icfg_edge_is_call_cfg_edge"
  external is_ret_cfg_edge    : icfg_edge -> bool     = "caml_icfg_edge_is_ret_cfg_edge"
  external is_intra_cfg_edge  : icfg_edge -> bool     = "caml_icfg_edge_is_intra_cfg_edge"

  (** [get_src_node edge] returns the source ICFG node. *)
  external get_src_node : icfg_edge -> icfg_node = "caml_icfg_edge_get_src_node"

  (** [get_dst_node edge] returns the destination ICFG node. *)
  external get_dst_node : icfg_edge -> icfg_node = "caml_icfg_edge_get_dst_node"

  (** [get_src_id edge] returns the ID of the source node. *)
  external get_src_id   : icfg_edge -> node_id   = "caml_icfg_edge_get_src_id"

  (** [get_dst_id edge] returns the ID of the destination node. *)
  external get_dst_id   : icfg_edge -> node_id   = "caml_icfg_edge_get_dst_id"

  (** {2 Downcast helpers} *)

  (** [as_intra edge] casts to IntraCFGEdge, or [None]. *)
  external as_intra     : icfg_edge -> icfg_edge option = "caml_icfg_edge_as_intra"

  (** [as_call_edge edge] casts to CallCFGEdge, or [None]. *)
  external as_call_edge : icfg_edge -> icfg_edge option = "caml_icfg_edge_as_call"

  (** [as_ret_edge edge] casts to RetCFGEdge, or [None]. *)
  external as_ret_edge  : icfg_edge -> icfg_edge option = "caml_icfg_edge_as_ret"

  (** {2 IntraCFGEdge-specific} *)

  (** [intra_cfg_edge_get_condition edge] returns the branch condition as a
      string, or [None] for unconditional edges. *)
  external intra_cfg_edge_get_condition : icfg_edge -> string option
                                                    = "caml_intra_cfg_edge_get_condition"

  (** {2 CallCFGEdge-specific} *)

  (** [call_cfg_edge_get_call_site edge] returns the call ICFG node. *)
  external call_cfg_edge_get_call_site  : icfg_edge -> icfg_node
                                                    = "caml_call_cfg_edge_get_call_site"

  (** [call_cfg_edge_get_call_pes edge] returns the call PAG edges at this call site. *)
  external call_cfg_edge_get_call_pes   : icfg_edge -> svf_stmt list
                                                    = "caml_call_cfg_edge_get_call_pes"

  (** {2 RetCFGEdge-specific} *)

  (** [ret_cfg_edge_get_call_site edge] returns the call ICFG node for the return edge. *)
  external ret_cfg_edge_get_call_site   : icfg_edge -> icfg_node
                                                    = "caml_ret_cfg_edge_get_call_site"

  (** [ret_cfg_edge_get_ret_pe edge] returns the return PAG edge. *)
  external ret_cfg_edge_get_ret_pe      : icfg_edge -> svf_stmt option
                                                    = "caml_ret_cfg_edge_get_ret_pe"
end

(** {1 Icfg — the Interprocedural Control Flow Graph} *)
module Icfg = struct
  (** [get_nodes icfg] returns all ICFG nodes as a list. *)
  external get_nodes    : icfg -> icfg_node list = "caml_icfg_get_nodes"

  (** [get_gnode icfg id] returns the ICFG node with the given [id].
      Raises [Failure] if the node is not found. *)
  external get_gnode    : icfg -> node_id -> icfg_node = "caml_icfg_get_gnode"

  (** [get_node_num icfg] returns the number of nodes in [icfg]. *)
  external get_node_num : icfg -> int = "caml_icfg_get_node_num"

  (** [get_global_icfg_node icfg] returns the global ICFG node, or [None]. *)
  external get_global_icfg_node   : icfg -> icfg_node option = "caml_icfg_get_global_icfg_node"

  (** [get_fun_entry_icfg_node icfg fun_obj] returns the entry ICFG node for
      the given function, or [None]. *)
  external get_fun_entry_icfg_node : icfg -> fun_obj_var -> icfg_node option
                                                           = "caml_icfg_get_fun_entry_icfg_node"

  (** [dump icfg filename] dumps the ICFG to a dot file. *)
  external dump         : icfg -> string -> unit = "caml_icfg_dump"
end

(** {1 CallGraphNode — nodes in the call graph} *)
module CallGraphNode = struct
  external to_string     : call_graph_node -> string       = "caml_call_graph_node_to_string"
  external get_id        : call_graph_node -> node_id      = "caml_call_graph_node_get_id"
  external get_name      : call_graph_node -> string       = "caml_call_graph_node_get_name"
  external get_out_edges : call_graph_node -> call_graph_edge list
                                                           = "caml_call_graph_node_get_out_edges"
  external get_in_edges  : call_graph_node -> call_graph_edge list
                                                           = "caml_call_graph_node_get_in_edges"

  (** [is_reachable_from_prog_entry node] returns [true] if this function is
      reachable from the program entry point. *)
  external is_reachable_from_prog_entry : call_graph_node -> bool
                                                           = "caml_call_graph_node_is_reachable_from_prog_entry"
end

(** {1 CallGraphEdge — edges in the call graph} *)
module CallGraphEdge = struct
  external get_src : call_graph_edge -> call_graph_node = "caml_call_graph_edge_get_src"
  external get_dst : call_graph_edge -> call_graph_node = "caml_call_graph_edge_get_dst"

  (** [get_call_site_id edge] returns the ID of the call-site ICFG node. *)
  external get_call_site_id  : call_graph_edge -> node_id = "caml_call_graph_edge_get_call_site_id"

  (** [get_src_id edge] returns the ID of the source call-graph node. *)
  external get_src_id        : call_graph_edge -> node_id = "caml_call_graph_edge_get_src_id"

  (** [get_dst_id edge] returns the ID of the destination call-graph node. *)
  external get_dst_id        : call_graph_edge -> node_id = "caml_call_graph_edge_get_dst_id"

  (** [is_direct edge] returns [true] for direct (static) calls. *)
  external is_direct         : call_graph_edge -> bool    = "caml_call_graph_edge_is_direct"

  (** [is_indirect edge] returns [true] for indirect (function-pointer) calls. *)
  external is_indirect       : call_graph_edge -> bool    = "caml_call_graph_edge_is_indirect"

  (** [get_direct_calls edge] returns the list of direct call ICFG nodes. *)
  external get_direct_calls  : call_graph_edge -> icfg_node list
                                                          = "caml_call_graph_edge_get_direct_calls"

  (** [get_indirect_calls edge] returns the list of indirect call ICFG nodes. *)
  external get_indirect_calls : call_graph_edge -> icfg_node list
                                                          = "caml_call_graph_edge_get_indirect_calls"

  (** [to_string edge] returns a string representation. *)
  external to_string         : call_graph_edge -> string  = "caml_call_graph_edge_to_string"
end

(** {1 CallGraph} *)
module CallGraph = struct
  (** [get_nodes cg] returns all call graph nodes. *)
  external get_nodes : call_graph -> call_graph_node list = "caml_call_graph_get_nodes"

  (** [get_gnode cg id] returns the call graph node with the given [id]. *)
  external get_gnode : call_graph -> node_id -> call_graph_node = "caml_call_graph_get_gnode"

  (** [dump cg filename] dumps the call graph to a dot file. *)
  external dump      : call_graph -> string -> unit = "caml_call_graph_dump"

  (** [view cg] opens a viewer for the call graph. *)
  external view      : call_graph -> unit = "caml_call_graph_view"

  (** [get_node_by_name cg name] returns the call graph node for function
      [name], or [None] if not found. *)
  external get_node_by_name : call_graph -> string -> call_graph_node option
                                                    = "caml_call_graph_get_node_by_name"
end

(** {1 ThreadCallGraph — subset of CallGraph for thread-related calls} *)
module ThreadCallGraph = struct
  (** [get_fork_sites cg] returns all fork-site ICFG nodes. *)
  external get_fork_sites : call_graph -> icfg_node list = "caml_thread_call_graph_get_fork_sites"

  (** [get_fork_edges cg] returns all call-graph edges that correspond to
      thread fork calls. *)
  external get_fork_edges : call_graph -> call_graph_edge list
                                                       = "caml_thread_call_graph_get_fork_edges"
end

(** {1 SVFBasicBlock} *)
module SvfBasicBlock = struct
  external get_name : svf_basic_block -> string = "caml_svf_basic_block_get_name"

  (** [get_id bb] returns the numeric ID of [bb]. *)
  external get_id   : svf_basic_block -> int    = "caml_svf_basic_block_get_id"

  (** [get_icfg_node_list bb] returns all ICFG nodes in [bb]. *)
  external get_icfg_node_list : svf_basic_block -> icfg_node list
                                                  = "caml_svf_basic_block_get_icfg_node_list"

  (** [get_successors bb] returns the successor basic blocks. *)
  external get_successors     : svf_basic_block -> svf_basic_block list
                                                  = "caml_svf_basic_block_get_successors"

  (** [get_predecessors bb] returns the predecessor basic blocks. *)
  external get_predecessors   : svf_basic_block -> svf_basic_block list
                                                  = "caml_svf_basic_block_get_predecessors"

  (** [get_num_successors bb] returns the number of successors. *)
  external get_num_successors : svf_basic_block -> int
                                                  = "caml_svf_basic_block_get_num_successors"
end

(** {1 VFGNode — nodes in the VFG/SVFG} *)
module VfgNode = struct
  (** [to_string node] returns a string representation. *)
  external to_string    : vfg_node -> string     = "caml_vfg_node_to_string"

  (** [get_id node] returns the ID. *)
  external get_id       : vfg_node -> node_id    = "caml_vfg_node_get_id"

  (** [get_icfg_node node] returns the associated ICFG node, or [None]. *)
  external get_icfg_node : vfg_node -> icfg_node option = "caml_vfg_node_get_icfg_node"

  (** [get_out_edges node] returns outgoing VFG edges. *)
  external get_out_edges : vfg_node -> vfg_edge list = "caml_vfg_node_get_out_edges"

  (** [get_in_edges node] returns incoming VFG edges. *)
  external get_in_edges  : vfg_node -> vfg_edge list = "caml_vfg_node_get_in_edges"

  (** {2 Kind predicates} *)
  external is_stmt_vfg_node      : vfg_node -> bool = "caml_vfg_node_is_stmt_vfg_node"
  external is_phi_vfg_node       : vfg_node -> bool = "caml_vfg_node_is_phi_vfg_node"
  external is_argument_vfg_node  : vfg_node -> bool = "caml_vfg_node_is_argument_vfg_node"
  external is_cmp_vfg_node       : vfg_node -> bool = "caml_vfg_node_is_cmp_vfg_node"
  external is_binary_op_vfg_node : vfg_node -> bool = "caml_vfg_node_is_binary_op_vfg_node"
  external is_unary_op_vfg_node  : vfg_node -> bool = "caml_vfg_node_is_unary_op_vfg_node"
  external is_branch_vfg_node    : vfg_node -> bool = "caml_vfg_node_is_branch_vfg_node"

  (** [get_value node] returns the PAG variable associated with this VFG node,
      or [None] for nodes without a direct variable. *)
  external get_value    : vfg_node -> svf_var option = "caml_vfg_node_get_value"
end

(** {1 VFGEdge — edges in the VFG/SVFG} *)
module VfgEdge = struct
  (** [to_string edge] returns a string representation. *)
  external to_string    : vfg_edge -> string   = "caml_vfg_edge_to_string"

  (** [get_src_node edge] returns the source VFG node. *)
  external get_src_node : vfg_edge -> vfg_node = "caml_vfg_edge_get_src_node"

  (** [get_dst_node edge] returns the destination VFG node. *)
  external get_dst_node : vfg_edge -> vfg_node = "caml_vfg_edge_get_dst_node"

  (** {2 Kind predicates} *)
  external is_direct_vfg_edge   : vfg_edge -> bool = "caml_vfg_edge_is_direct_vfg_edge"
  external is_indirect_vfg_edge : vfg_edge -> bool = "caml_vfg_edge_is_indirect_vfg_edge"
  external is_call_vfg_edge     : vfg_edge -> bool = "caml_vfg_edge_is_call_vfg_edge"
  external is_ret_vfg_edge      : vfg_edge -> bool = "caml_vfg_edge_is_ret_vfg_edge"
  external is_intra_vfg_edge    : vfg_edge -> bool = "caml_vfg_edge_is_intra_vfg_edge"
  external is_thread_mhp_edge   : vfg_edge -> bool = "caml_vfg_edge_is_thread_mhp_indirect"
end

(** {1 Vfg — Value Flow Graph} *)
module Vfg = struct
  (** [get_nodes vfg] returns all VFG nodes. *)
  external get_nodes         : svfg -> vfg_node list  = "caml_vfg_get_nodes"

  (** [get_gnode vfg id] returns the VFG node with the given [id]. *)
  external get_gnode         : svfg -> node_id -> vfg_node = "caml_vfg_get_gnode"

  (** [dump vfg filename] dumps the VFG to a dot file. *)
  external dump              : svfg -> string -> unit = "caml_vfg_dump"

  (** [view vfg] opens a viewer for the VFG. *)
  external view              : svfg -> unit           = "caml_vfg_view"

  (** [get_total_node_num vfg] returns the total number of nodes. *)
  external get_total_node_num : svfg -> int           = "caml_vfg_get_total_node_num"

  (** [get_total_edge_num vfg] returns the total number of edges. *)
  external get_total_edge_num : svfg -> int           = "caml_vfg_get_total_edge_num"
end

(** {1 Svfg — Sparse Value Flow Graph (superset of VFG)} *)
module Svfg = struct
  (** [get_nodes svfg] returns all SVFG nodes (as vfg_node, which is the base type). *)
  external get_nodes          : svfg -> vfg_node list = "caml_vfg_get_nodes"

  (** [get_gnode svfg id] returns the SVFG node with the given [id]. *)
  external get_gnode          : svfg -> node_id -> vfg_node = "caml_vfg_get_gnode"

  (** [dump svfg filename] dumps the SVFG to a dot file. *)
  external dump               : svfg -> string -> unit = "caml_vfg_dump"

  (** [view svfg] opens a viewer for the SVFG. *)
  external view               : svfg -> unit           = "caml_vfg_view"

  (** [has_def_svfg_node svfg var] returns [true] if [var] has a definition
      node in the SVFG. *)
  external has_def_svfg_node  : svfg -> svf_var -> bool = "caml_svfg_has_def_svfg_node"

  (** [get_def_svfg_node svfg var] returns the definition SVFG node for [var]. *)
  external get_def_svfg_node  : svfg -> svf_var -> vfg_node = "caml_svfg_get_def_svfg_node"

  (** [get_svfg_node svfg stmt] returns the SVFG node for a PAG statement. *)
  external get_svfg_node      : svfg -> svf_stmt -> vfg_node = "caml_svfg_get_svfg_node"

  (** [has_svfg_node svfg stmt] returns [true] if there is an SVFG node for [stmt]. *)
  external has_svfg_node      : svfg -> svf_stmt -> bool = "caml_svfg_has_svfg_node"

  (** [is_fun_entry_svfg_node svfg node] returns [true] if [node] is a function
      entry SVFG node. *)
  external is_fun_entry_svfg_node     : svfg -> vfg_node -> bool
                                                           = "caml_svfg_is_fun_entry_svfg_node"

  (** [is_call_site_ret_svfg_node svfg node] returns [true] if [node] is a
      call-site return SVFG node. *)
  external is_call_site_ret_svfg_node : svfg -> vfg_node -> bool
                                                           = "caml_svfg_is_call_site_ret_svfg_node"
end

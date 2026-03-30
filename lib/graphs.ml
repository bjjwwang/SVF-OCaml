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
end

(** {1 CallGraphEdge — edges in the call graph} *)
module CallGraphEdge = struct
  external get_src : call_graph_edge -> call_graph_node = "caml_call_graph_edge_get_src"
  external get_dst : call_graph_edge -> call_graph_node = "caml_call_graph_edge_get_dst"
end

(** {1 CallGraph} *)
module CallGraph = struct
  (** [get_nodes cg] returns all call graph nodes. *)
  external get_nodes : call_graph -> call_graph_node list = "caml_call_graph_get_nodes"

  (** [get_gnode cg id] returns the call graph node with the given [id]. *)
  external get_gnode : call_graph -> node_id -> call_graph_node = "caml_call_graph_get_gnode"
end

(** {1 SVFBasicBlock} *)
module SvfBasicBlock = struct
  external get_name : svf_basic_block -> string = "caml_svf_basic_block_get_name"
end

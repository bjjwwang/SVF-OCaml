(** Wpa — Pointer Analysis (Whole-Program Analysis).
    Mirrors SVF-Python/pybind/WPA.cpp and MemoryModel.cpp.

    Supported analyses:
    - {!AndersenBase}     — Andersen's analysis base (direct instantiation)
    - {!AndersenWaveDiff} — Andersen wave-difference (default in {!Svf.get_pag})
    - {!Steensgaard}      — Steensgaard's fast unification-based PTA
    - {!PointsTo}         — Points-to set manipulation
    - {!ConstraintNode}   — Constraint graph node
    - {!ConstraintEdge}   — Constraint graph edge
    - {!ConstraintGraph}  — Constraint graph
*)

open Types

(** Alias analysis result *)
type alias_result =
  | NoAlias      (** The two nodes definitely do not alias *)
  | MayAlias     (** The two nodes may alias *)
  | MustAlias    (** The two nodes must alias *)
  | PartialAlias (** Partial overlap *)

(** {1 PointsTo — points-to set} *)
module PointsTo = struct
  (** [create ()] returns a new empty points-to set. *)
  external create    : unit -> points_to      = "caml_points_to_create"

  (** [set pts id] adds [id] to [pts]. *)
  external set       : points_to -> node_id -> unit = "caml_points_to_set"

  (** [to_list pts] returns the elements of [pts] as a list of node IDs. *)
  external to_list   : points_to -> node_id list    = "caml_points_to_to_list"

  (** [to_string pts] returns a space-separated string of node IDs. *)
  external to_string : points_to -> string          = "caml_points_to_to_string"

  (** [count pts] returns the number of elements in [pts]. *)
  external count     : points_to -> int             = "caml_points_to_count"
end

(** {1 ConstraintNode — node in the constraint graph} *)
module ConstraintNode = struct
  (** [get_id node] returns the ID of the node. *)
  external get_id         : constraint_node -> node_id = "caml_constraint_node_get_id"

  (** [get_in_edges node] returns all incoming constraint edges. *)
  external get_in_edges   : constraint_node -> constraint_edge list
                                                       = "caml_constraint_node_get_in_edges"

  (** [get_out_edges node] returns all outgoing constraint edges. *)
  external get_out_edges  : constraint_node -> constraint_edge list
                                                       = "caml_constraint_node_get_out_edges"

  (** {2 Filtered edge accessors} *)

  (** [get_direct_in_edges node] returns incoming Direct edges. *)
  external get_direct_in_edges   : constraint_node -> constraint_edge list
                                                       = "caml_constraint_node_get_direct_in_edges"

  (** [get_direct_out_edges node] returns outgoing Direct edges. *)
  external get_direct_out_edges  : constraint_node -> constraint_edge list
                                                       = "caml_constraint_node_get_direct_out_edges"

  (** [get_copy_in_edges node] returns incoming Copy edges. *)
  external get_copy_in_edges     : constraint_node -> constraint_edge list
                                                       = "caml_constraint_node_get_copy_in_edges"

  (** [get_copy_out_edges node] returns outgoing Copy edges. *)
  external get_copy_out_edges    : constraint_node -> constraint_edge list
                                                       = "caml_constraint_node_get_copy_out_edges"

  (** [get_gep_in_edges node] returns incoming Gep edges. *)
  external get_gep_in_edges      : constraint_node -> constraint_edge list
                                                       = "caml_constraint_node_get_gep_in_edges"

  (** [get_gep_out_edges node] returns outgoing Gep edges. *)
  external get_gep_out_edges     : constraint_node -> constraint_edge list
                                                       = "caml_constraint_node_get_gep_out_edges"

  (** [get_load_in_edges node] returns incoming Load edges. *)
  external get_load_in_edges     : constraint_node -> constraint_edge list
                                                       = "caml_constraint_node_get_load_in_edges"

  (** [get_load_out_edges node] returns outgoing Load edges. *)
  external get_load_out_edges    : constraint_node -> constraint_edge list
                                                       = "caml_constraint_node_get_load_out_edges"

  (** [get_store_in_edges node] returns incoming Store edges. *)
  external get_store_in_edges    : constraint_node -> constraint_edge list
                                                       = "caml_constraint_node_get_store_in_edges"

  (** [get_store_out_edges node] returns outgoing Store edges. *)
  external get_store_out_edges   : constraint_node -> constraint_edge list
                                                       = "caml_constraint_node_get_store_out_edges"

  (** [get_addr_in_edges node] returns incoming Addr edges. *)
  external get_addr_in_edges     : constraint_node -> constraint_edge list
                                                       = "caml_constraint_node_get_addr_in_edges"

  (** [get_addr_out_edges node] returns outgoing Addr edges. *)
  external get_addr_out_edges    : constraint_node -> constraint_edge list
                                                       = "caml_constraint_node_get_addr_out_edges"
end

(** {1 ConstraintEdge — edge in the constraint graph} *)
module ConstraintEdge = struct
  (** [get_src_node edge] returns the source constraint node. *)
  external get_src_node  : constraint_edge -> constraint_node = "caml_constraint_edge_get_src_node"

  (** [get_dst_node edge] returns the destination constraint node. *)
  external get_dst_node  : constraint_edge -> constraint_node = "caml_constraint_edge_get_dst_node"

  (** [get_src_id edge] returns the ID of the source node. *)
  external get_src_id    : constraint_edge -> node_id = "caml_constraint_edge_get_src_id"

  (** [get_dst_id edge] returns the ID of the destination node. *)
  external get_dst_id    : constraint_edge -> node_id = "caml_constraint_edge_get_dst_id"

  (** {2 Kind predicates} *)
  external is_addr_edge       : constraint_edge -> bool = "caml_constraint_edge_is_addr"
  external is_copy_edge       : constraint_edge -> bool = "caml_constraint_edge_is_copy"
  external is_store_edge      : constraint_edge -> bool = "caml_constraint_edge_is_store"
  external is_load_edge       : constraint_edge -> bool = "caml_constraint_edge_is_load"
  external is_gep_edge        : constraint_edge -> bool = "caml_constraint_edge_is_gep"
  external is_normal_gep_edge : constraint_edge -> bool = "caml_constraint_edge_is_normal_gep"
  external is_variant_gep_edge : constraint_edge -> bool = "caml_constraint_edge_is_variant_gep"

  (** {2 Downcast helpers} *)
  external as_addr_edge       : constraint_edge -> constraint_edge option = "caml_constraint_edge_as_addr"
  external as_copy_edge       : constraint_edge -> constraint_edge option = "caml_constraint_edge_as_copy"
  external as_store_edge      : constraint_edge -> constraint_edge option = "caml_constraint_edge_as_store"
  external as_load_edge       : constraint_edge -> constraint_edge option = "caml_constraint_edge_as_load"
  external as_normal_gep_edge : constraint_edge -> constraint_edge option = "caml_constraint_edge_as_normal_gep"
  external as_variant_gep_edge : constraint_edge -> constraint_edge option = "caml_constraint_edge_as_variant_gep"
end

(** {1 ConstraintGraph — the Andersen constraint graph} *)
module ConstraintGraph = struct
  (** [get_nodes cg] returns all constraint nodes. *)
  external get_nodes   : constraint_graph -> constraint_node list = "caml_constraint_graph_get_nodes"

  (** [get_gnode cg id] returns the constraint node with the given [id].
      Raises [Failure] if not found. *)
  external get_gnode   : constraint_graph -> node_id -> constraint_node
                                                                  = "caml_constraint_graph_get_gnode"

  (** [dump cg filename] dumps the constraint graph to a dot file. *)
  external dump        : constraint_graph -> string -> unit       = "caml_constraint_graph_dump"

  (** [add_copy_edge cg src dst] adds a copy edge from [src] to [dst].
      Returns [true] if the edge was newly added. *)
  external add_copy_edge : constraint_graph -> node_id -> node_id -> bool
                                                                  = "caml_constraint_graph_add_copy_edge"
end

(** {1 AndersenBase — base Andersen pointer analysis} *)
module AndersenBase = struct
  (** [create pag] creates an Andersen analysis instance for [pag].
      Note: this creates a PublicAndersen (subclass) instance. *)
  external create              : svfir -> andersen_base = "caml_andersen_base_create"

  (** [analyze pa] runs the analysis. *)
  external analyze             : andersen_base -> unit  = "caml_andersen_base_analyze"

  (** [get_constraint_graph pa] returns the constraint graph. *)
  external get_constraint_graph : andersen_base -> constraint_graph
                                                        = "caml_andersen_base_get_constraint_graph"

  (** [get_pts pa id] returns the points-to set for variable [id].
      The returned set is a copy. *)
  external get_pts             : andersen_base -> node_id -> points_to
                                                        = "caml_andersen_base_get_pts"

  (** [add_pts pa id ptd] adds [ptd] to the points-to set of [id].
      Returns [true] if the set changed. *)
  external add_pts             : andersen_base -> node_id -> node_id -> bool
                                                        = "caml_andersen_base_add_pts"

  (** [union_pts pa id ptd] unions the points-to set of [id] with that of [ptd]. *)
  external union_pts           : andersen_base -> node_id -> node_id -> bool
                                                        = "caml_andersen_base_union_pts"

  (** [alias pa id1 id2] checks whether [id1] and [id2] may alias. *)
  external alias_int           : andersen_base -> node_id -> node_id -> int
                                                        = "caml_andersen_base_alias"

  let alias pa id1 id2 =
    match alias_int pa id1 id2 with
    | 0 -> NoAlias
    | 1 -> MayAlias
    | 2 -> MustAlias
    | 3 -> PartialAlias
    | _ -> MayAlias

  external is_worklist_empty   : andersen_base -> bool = "caml_andersen_base_is_worklist_empty"
  external push_worklist       : andersen_base -> node_id -> unit
                                                        = "caml_andersen_base_push_worklist"
  external pop_worklist        : andersen_base -> node_id = "caml_andersen_base_pop_worklist"

  (** [initialize pa] runs the initialization phase of the analysis. *)
  external initialize          : andersen_base -> unit  = "caml_andersen_base_initialize"

  (** [finalize pa] runs the finalization phase. *)
  external finalize            : andersen_base -> unit  = "caml_andersen_base_finalize"

  (** [init_worklist pa] initializes the constraint solving worklist. *)
  external init_worklist       : andersen_base -> unit  = "caml_andersen_base_init_worklist"

  (** [update_call_graph pa] processes indirect call edges discovered during
      solving and updates the call graph. Returns [true] if the call graph changed. *)
  external update_call_graph   : andersen_base -> bool
                                                        = "caml_andersen_base_update_call_graph"

  (** [is_in_cycle pa id] returns [true] if node [id] is in a PTA SCC cycle. *)
  external is_in_cycle         : andersen_base -> node_id -> bool
                                                        = "caml_andersen_base_is_in_cycle"

  (** [scc_rep_node pa id] returns the SCC representative node for [id]. *)
  external scc_rep_node        : andersen_base -> node_id -> node_id
                                                        = "caml_andersen_base_scc_rep_node"
end

(** {1 AndersenWaveDiff — wave-propagation Andersen PTA} *)
module AndersenWaveDiff = struct
  external create  : svfir -> andersen_wave_diff = "caml_andersen_wave_diff_create"
  external analyze : andersen_wave_diff -> unit  = "caml_andersen_wave_diff_analyze"
end

(** {1 Steensgaard — unification-based PTA} *)
module Steensgaard = struct
  external create  : svfir -> steensgaard = "caml_steensgaard_create"
  external analyze : steensgaard -> unit  = "caml_steensgaard_analyze"
end

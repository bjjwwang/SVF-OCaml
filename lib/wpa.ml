(** Wpa — Pointer Analysis (Whole-Program Analysis).
    Mirrors SVF-Python/pybind/WPA.cpp and MemoryModel.cpp.

    Supported analyses:
    - {!AndersenBase}     — Andersen's analysis base (direct instantiation)
    - {!AndersenWaveDiff} — Andersen wave-difference (default in {!Svf.get_pag})
    - {!Steensgaard}      — Steensgaard's fast unification-based PTA
    - {!PointsTo}         — Points-to set manipulation
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

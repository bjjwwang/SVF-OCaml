(** MemoryModel — memory model bindings.
    Mirrors SVF-Python/pybind/MemoryModel.cpp.

    The PointsTo type and implementation live in {!Wpa}. *)

(** [version ()] returns a version string. *)
external version : unit -> string = "caml_memory_model_version"

(** [points_to_create ()] creates an empty points-to set. *)
let points_to_create = Wpa.PointsTo.create

(** [points_to_set pts id] adds [id] to [pts]. *)
let points_to_set = Wpa.PointsTo.set

(** [points_to_to_list pts] converts [pts] to a list of node IDs. *)
let points_to_to_list = Wpa.PointsTo.to_list

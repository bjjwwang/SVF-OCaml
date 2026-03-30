(** Ae — Abstract Execution / Abstract Interpretation engine.
    Mirrors SVF-Python/pybind/AE.cpp (core subset). *)

open Types

(** {1 AbstractState} *)
module AbstractState = struct
  external to_string : abstract_state -> string = "caml_abstract_state_to_string"
end

(** {1 IntervalValue — integer interval [lb, ub]} *)
module IntervalValue = struct
  (** [create lb ub] creates the interval [[lb, ub]]. *)
  external create    : int -> int -> interval_value = "caml_interval_value_create"

  (** [lb iv] returns the lower bound of [iv]. *)
  external lb        : interval_value -> int        = "caml_interval_value_lb"

  (** [ub iv] returns the upper bound of [iv]. *)
  external ub        : interval_value -> int        = "caml_interval_value_ub"

  external is_bottom : interval_value -> bool       = "caml_interval_value_is_bottom"
  external is_top    : interval_value -> bool       = "caml_interval_value_is_top"
  external to_string : interval_value -> string     = "caml_interval_value_to_string"
end

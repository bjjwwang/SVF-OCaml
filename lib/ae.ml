(** Ae — Abstract Execution / Abstract Interpretation engine.
    Mirrors SVF-Python/pybind/AE.cpp (core subset). *)

open Types

(** {1 BoundedInt — an integer extended with +inf and -inf} *)
module BoundedInt = struct
  (** [create n] creates a bounded integer with finite value [n].
      The second argument [is_inf] must be [false] for finite values. *)
  external create_raw     : int -> bool -> bounded_int = "caml_bounded_int_create"

  (** [create n] creates a bounded integer with finite value [n]. *)
  let create n = create_raw n false

  (** [plus_infinity ()] returns +∞. *)
  external plus_infinity  : unit -> bounded_int = "caml_bounded_int_plus_infinity"

  (** [minus_infinity ()] returns -∞. *)
  external minus_infinity : unit -> bounded_int = "caml_bounded_int_minus_infinity"

  (** [get_numeral b] returns the finite value of [b].
      Raises [Failure] if [b] is infinite. *)
  external get_numeral    : bounded_int -> int  = "caml_bounded_int_get_numeral"

  external is_infinity       : bounded_int -> bool = "caml_bounded_int_is_infinity"
  external is_plus_infinity  : bounded_int -> bool = "caml_bounded_int_is_plus_infinity"
  external is_minus_infinity : bounded_int -> bool = "caml_bounded_int_is_minus_infinity"
  external is_zero           : bounded_int -> bool = "caml_bounded_int_is_zero"

  (** [leq a b] returns [true] if [a] ≤ [b]. *)
  external leq   : bounded_int -> bounded_int -> bool = "caml_bounded_int_leq"

  (** [geq a b] returns [true] if [a] ≥ [b]. *)
  external geq   : bounded_int -> bounded_int -> bool = "caml_bounded_int_geq"

  (** [equal a b] returns [true] if [a] = [b]. *)
  external equal : bounded_int -> bounded_int -> bool = "caml_bounded_int_equal"
end

(** {1 IntervalValue — integer interval [[lb, ub]]} *)
module IntervalValue = struct
  (** [create lb ub] creates the interval [[lb, ub]]. *)
  external create    : int -> int -> interval_value = "caml_interval_value_create"

  (** [top ()] returns the top interval (all integers). *)
  external top       : unit -> interval_value       = "caml_interval_value_top"

  (** [bottom ()] returns the bottom interval (empty). *)
  external bottom    : unit -> interval_value       = "caml_interval_value_bottom"

  (** [lb iv] returns the lower bound as an integer.
      Only valid when [not (is_bottom iv)] and lb is finite. *)
  external lb        : interval_value -> int        = "caml_interval_value_lb"

  (** [ub iv] returns the upper bound as an integer.
      Only valid when [not (is_bottom iv)] and ub is finite. *)
  external ub        : interval_value -> int        = "caml_interval_value_ub"

  (** [lb_bounded iv] returns the lower bound as a [bounded_int]. *)
  external lb_bounded : interval_value -> bounded_int = "caml_interval_value_lb_bounded"

  (** [ub_bounded iv] returns the upper bound as a [bounded_int]. *)
  external ub_bounded : interval_value -> bounded_int = "caml_interval_value_ub_bounded"

  external is_bottom  : interval_value -> bool      = "caml_interval_value_is_bottom"
  external is_top     : interval_value -> bool      = "caml_interval_value_is_top"
  external is_zero    : interval_value -> bool      = "caml_interval_value_is_zero"
  external is_infinity : interval_value -> bool     = "caml_interval_value_is_infinity"
  external is_numeral : interval_value -> bool      = "caml_interval_value_is_numeral"

  (** [get_numeral iv] returns the single finite value when [is_numeral iv].
      Raises [Failure] otherwise. *)
  external get_numeral : interval_value -> int      = "caml_interval_value_get_numeral"

  external to_string  : interval_value -> string    = "caml_interval_value_to_string"

  (** [equals a b] returns [true] if [a] and [b] represent the same interval. *)
  external equals     : interval_value -> interval_value -> bool
                                                    = "caml_interval_value_equals"

  (** [clone iv] returns a fresh copy of [iv]. *)
  external clone      : interval_value -> interval_value = "caml_interval_value_clone"

  (** {2 Lattice operations (mutate in place)} *)

  (** [join_with dst src] sets [dst] to join([dst], [src]). *)
  external join_with   : interval_value -> interval_value -> unit
                                                    = "caml_interval_value_join_with"

  (** [meet_with dst src] sets [dst] to meet([dst], [src]). *)
  external meet_with   : interval_value -> interval_value -> unit
                                                    = "caml_interval_value_meet_with"

  (** [widen_with dst src] sets [dst] to widen([dst], [src]). *)
  external widen_with  : interval_value -> interval_value -> unit
                                                    = "caml_interval_value_widen_with"

  (** [narrow_with dst src] sets [dst] to narrow([dst], [src]). *)
  external narrow_with : interval_value -> interval_value -> unit
                                                    = "caml_interval_value_narrow_with"

  (** [set_to_bottom iv] sets [iv] to bottom. *)
  external set_to_bottom : interval_value -> unit   = "caml_interval_value_set_to_bottom"

  (** [set_to_top iv] sets [iv] to top. *)
  external set_to_top    : interval_value -> unit   = "caml_interval_value_set_to_top"

  (** {2 Arithmetic (return new owned intervals)} *)

  external add    : interval_value -> interval_value -> interval_value = "caml_interval_value_add"
  external sub    : interval_value -> interval_value -> interval_value = "caml_interval_value_sub"
  external mul    : interval_value -> interval_value -> interval_value = "caml_interval_value_mul"
  external div    : interval_value -> interval_value -> interval_value = "caml_interval_value_div"
  external rem    : interval_value -> interval_value -> interval_value = "caml_interval_value_mod"
  external band   : interval_value -> interval_value -> interval_value = "caml_interval_value_and"
  external bor    : interval_value -> interval_value -> interval_value = "caml_interval_value_or"
  external bxor   : interval_value -> interval_value -> interval_value = "caml_interval_value_xor"
  external lshift : interval_value -> interval_value -> interval_value = "caml_interval_value_lshift"
  external rshift : interval_value -> interval_value -> interval_value = "caml_interval_value_rshift"
end

(** {1 AddressValue — a set of abstract memory addresses} *)
module AddressValue = struct
  (** [create ()] creates an empty address value. *)
  external create       : unit -> address_value   = "caml_address_value_create"

  (** [insert av addr] adds abstract address [addr] to [av]. *)
  external insert       : address_value -> int -> unit = "caml_address_value_insert"

  (** [contains av addr] returns [true] if [addr] is in [av]. *)
  external contains     : address_value -> int -> bool = "caml_address_value_contains"

  external is_bottom    : address_value -> bool   = "caml_address_value_is_bottom"

  (** [get_vals av] returns the list of abstract addresses. *)
  external get_vals     : address_value -> int list = "caml_address_value_get_vals"

  (** [join_with dst src] sets [dst] to join([dst], [src]). *)
  external join_with    : address_value -> address_value -> unit
                                                  = "caml_address_value_join_with"

  (** [meet_with dst src] sets [dst] to meet([dst], [src]). *)
  external meet_with    : address_value -> address_value -> unit
                                                  = "caml_address_value_meet_with"

  (** [has_intersect a b] returns [true] if [a] and [b] share at least one address. *)
  external has_intersect : address_value -> address_value -> bool
                                                  = "caml_address_value_has_intersect"

  (** [equals a b] returns [true] if [a] = [b]. *)
  external equals       : address_value -> address_value -> bool
                                                  = "caml_address_value_equals"

  (** [clone av] returns a fresh copy. *)
  external clone        : address_value -> address_value = "caml_address_value_clone"

  (** [to_string av] returns a string representation. *)
  external to_string    : address_value -> string = "caml_address_value_to_string"

  (** [get_virtual_mem_address id] converts a numeric object ID to the
      corresponding virtual memory address integer. *)
  external get_virtual_mem_address : int -> int   = "caml_address_value_get_virtual_mem_address"

  (** [is_virtual_mem_address addr] returns [true] if [addr] encodes a virtual
      memory address (i.e. has the virtual-address bit set). *)
  external is_virtual_mem_address  : int -> bool  = "caml_address_value_is_virtual_mem_address"
end

(** {1 AbstractValue — a tagged union of IntervalValue | AddressValue} *)
module AbstractValue = struct
  (** [create_interval iv] creates an AbstractValue containing interval [iv]. *)
  external create_interval : interval_value -> abstract_value = "caml_abstract_value_create_interval"

  (** [create_addr av] creates an AbstractValue containing address set [av]. *)
  external create_addr     : address_value  -> abstract_value = "caml_abstract_value_create_addr"

  external is_interval     : abstract_value -> bool           = "caml_abstract_value_is_interval"
  external is_addr         : abstract_value -> bool           = "caml_abstract_value_is_addr"

  (** [get_interval v] returns the interval component.
      Requires [is_interval v]. *)
  external get_interval    : abstract_value -> interval_value = "caml_abstract_value_get_interval"

  (** [get_addrs v] returns the address-set component.
      Requires [is_addr v]. *)
  external get_addrs       : abstract_value -> address_value  = "caml_abstract_value_get_addrs"

  (** [equals a b] returns [true] if [a] = [b]. *)
  external equals          : abstract_value -> abstract_value -> bool
                                                              = "caml_abstract_value_equals"

  (** [clone v] returns a fresh copy. *)
  external clone           : abstract_value -> abstract_value = "caml_abstract_value_clone"

  (** [to_string v] returns a string representation. *)
  external to_string       : abstract_value -> string         = "caml_abstract_value_to_string"

  (** [join_with dst src] sets [dst] to join([dst], [src]). *)
  external join_with       : abstract_value -> abstract_value -> unit
                                                              = "caml_abstract_value_join_with"

  (** [meet_with dst src] sets [dst] to meet([dst], [src]). *)
  external meet_with       : abstract_value -> abstract_value -> unit
                                                              = "caml_abstract_value_meet_with"

  (** [widen_with dst src] sets [dst] to widen([dst], [src]). *)
  external widen_with      : abstract_value -> abstract_value -> unit
                                                              = "caml_abstract_value_widen_with"

  (** [narrow_with dst src] sets [dst] to narrow([dst], [src]). *)
  external narrow_with     : abstract_value -> abstract_value -> unit
                                                              = "caml_abstract_value_narrow_with"
end

(** {1 AbstractState — a map from variable IDs to abstract values} *)
module AbstractState = struct
  (** [to_string as_] returns a string representation. *)
  external to_string          : abstract_state -> string = "caml_abstract_state_to_string"

  (** [get_var as_ id] returns the abstract value for variable [id],
      or [None] if not present. *)
  external get_var            : abstract_state -> node_id -> abstract_value option
                                                           = "caml_abstract_state_get_var"

  (** [set_var as_ id v] sets variable [id] to abstract value [v]. *)
  external set_var            : abstract_state -> node_id -> abstract_value -> unit
                                                           = "caml_abstract_state_set_var"

  (** [store as_ addr v] writes abstract value [v] to abstract address [addr]. *)
  external store              : abstract_state -> int -> abstract_value -> unit
                                                           = "caml_abstract_state_store"

  (** [load as_ addr] reads the abstract value at abstract address [addr].
      Returns bottom if not present. *)
  external load               : abstract_state -> int -> abstract_value
                                                           = "caml_abstract_state_load"

  (** [equals a b] returns [true] if states [a] and [b] are equal. *)
  external equals             : abstract_state -> abstract_state -> bool
                                                           = "caml_abstract_state_equals"

  (** [join_with dst src] sets [dst] to join([dst], [src]). *)
  external join_with          : abstract_state -> abstract_state -> unit
                                                           = "caml_abstract_state_join_with"

  (** [meet_with dst src] sets [dst] to meet([dst], [src]). *)
  external meet_with          : abstract_state -> abstract_state -> unit
                                                           = "caml_abstract_state_meet_with"

  (** [get_id_from_addr as_ addr] returns the variable ID for abstract address [addr]. *)
  external get_id_from_addr   : abstract_state -> int -> node_id
                                                           = "caml_abstract_state_get_id_from_addr"

  (** [in_var_to_val_table as_ id] returns [true] if [id] is in the variable-to-value map. *)
  external in_var_to_val_table  : abstract_state -> node_id -> bool
                                                           = "caml_abstract_state_in_var_to_val_table"

  (** [in_var_to_addrs_table as_ id] returns [true] if [id] is in the variable-to-addresses map. *)
  external in_var_to_addrs_table : abstract_state -> node_id -> bool
                                                           = "caml_abstract_state_in_var_to_addrs_table"

  (** [in_addr_to_addrs_table as_ addr] returns [true] if [addr] is in the addr-to-addresses map. *)
  external in_addr_to_addrs_table : abstract_state -> int -> bool
                                                           = "caml_abstract_state_in_addr_to_addrs_table"

  (** [load_value as_ id] returns the interval value for variable [id]. *)
  external load_value         : abstract_state -> node_id -> interval_value
                                                           = "caml_abstract_state_load_value"

  (** [store_value as_ id iv] stores interval value [iv] for variable [id]. *)
  external store_value        : abstract_state -> node_id -> interval_value -> unit
                                                           = "caml_abstract_state_store_value"

  (** [hash as_] returns a hash of the state. *)
  external hash               : abstract_state -> int     = "caml_abstract_state_hash"

  (** [clear as_] clears all entries in the state. *)
  external clear              : abstract_state -> unit    = "caml_abstract_state_clear"

  (** [print as_] prints the state to stdout. *)
  external print              : abstract_state -> unit    = "caml_abstract_state_print"

  (** [add_to_freed_addrs as_ addr] marks [addr] as freed. *)
  external add_to_freed_addrs : abstract_state -> int -> unit
                                                           = "caml_abstract_state_add_to_freed_addrs"

  (** [is_freed_mem as_ addr] returns [true] if [addr] has been freed. *)
  external is_freed_mem       : abstract_state -> int -> bool
                                                           = "caml_abstract_state_is_freed_mem"

  (** [is_null_mem addr] (static) returns [true] if [addr] is the null address. *)
  external is_null_mem        : int -> bool               = "caml_abstract_state_is_null_mem"

  (** [is_black_hole_obj_addr addr] (static) returns [true] if [addr] is the black-hole address. *)
  external is_black_hole_obj_addr : int -> bool           = "caml_abstract_state_is_black_hole_obj_addr"

  (** [is_virtual_mem_address addr] returns [true] if [addr] encodes a virtual memory location. *)
  external is_virtual_mem_address : abstract_state -> int -> bool
                                                           = "caml_abstract_state_is_virtual_mem_address"

  (** [get_virtual_mem_address as_ id] returns the virtual address for object ID [id]. *)
  external get_virtual_mem_address : abstract_state -> node_id -> int
                                                           = "caml_abstract_state_get_virtual_mem_address"

  (** [get_gep_obj_addrs as_ var_id iv] returns the address value obtained by
      GEP-indexing the object at [var_id] with interval [iv]. *)
  external get_gep_obj_addrs  : abstract_state -> node_id -> interval_value -> address_value
                                                           = "caml_abstract_state_get_gep_obj_addrs"

  (** [get_element_index as_ gep_stmt] returns the element index interval for [gep_stmt].
      Raises [Failure] if [gep_stmt] is not a GepStmt. *)
  external get_element_index  : abstract_state -> svf_stmt -> interval_value
                                                           = "caml_abstract_state_get_element_index"

  (** [get_byte_offset as_ gep_stmt] returns the byte-offset interval for [gep_stmt].
      Raises [Failure] if [gep_stmt] is not a GepStmt. *)
  external get_byte_offset    : abstract_state -> svf_stmt -> interval_value
                                                           = "caml_abstract_state_get_byte_offset"

  (** [get_pointee_element as_ id] returns the SVF type of the pointee for object [id]. *)
  external get_pointee_element : abstract_state -> node_id -> svf_type
                                                           = "caml_abstract_state_get_pointee_element"

  (** [get_alloca_inst_byte_size as_ addr_stmt] returns the allocation byte size
      for [addr_stmt]. Raises [Failure] if [addr_stmt] is not an AddrStmt. *)
  external get_alloca_inst_byte_size : abstract_state -> svf_stmt -> int
                                                           = "caml_abstract_state_get_alloca_inst_byte_size"

  (** [init_obj_var as_ var] initialises the object variable [var] in the state. *)
  external init_obj_var       : abstract_state -> svf_var -> unit
                                                           = "caml_abstract_state_init_obj_var"
end

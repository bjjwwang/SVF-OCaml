(** Mta — Multi-Thread Analysis.
    Mirrors SVF-Python/pybind/MTA.cpp.

    Usage:
    {[
      let mta = Mta.Mta.create () in
      Mta.Mta.run_on_module mta pag;
      let pa   = Svf.get_pta () in
      let tct  = Mta.Tct.create pa in
      let mhp  = Mta.Mhp.create tct in
      Mta.Mhp.analyze mhp;
      let race = Mta.Mhp.may_happen_in_parallel mhp node1 node2
    ]}
*)

open Types

(** {1 MTA — Multi-Thread Analysis driver} *)
module Mta = struct
  external create        : unit -> mta            = "caml_mta_create"
  external run_on_module : mta -> svfir -> unit   = "caml_mta_run_on_module"
  external detect        : mta -> unit            = "caml_mta_detect"
end

(** {1 TCT — Thread Creation Tree} *)
module Tct = struct
  (** [create pa] builds the TCT from a pointer analysis instance [pa]. *)
  external create               : andersen_base -> tct    = "caml_tct_create"
  external get_thread_call_graph : tct -> call_graph      = "caml_tct_get_thread_call_graph"
  external dump                 : tct -> unit             = "caml_tct_dump"
end

(** {1 MHP — May-Happen-in-Parallel analysis} *)
module Mhp = struct
  (** [create tct] builds MHP from the Thread Creation Tree [tct]. *)
  external create                  : tct -> mhp          = "caml_mhp_create"
  external analyze                 : mhp -> unit         = "caml_mhp_analyze"

  (** [may_happen_in_parallel mhp n1 n2] returns [true] if ICFG nodes
      [n1] and [n2] may execute in parallel. *)
  external may_happen_in_parallel  : mhp -> icfg_node -> icfg_node -> bool
                                                         = "caml_mhp_may_happen_in_parallel"
end

(** {1 LockAnalysis — lock-set analysis} *)
module LockAnalysis = struct
  external create       : tct -> lock_analysis    = "caml_lock_analysis_create"
  external analyze      : lock_analysis -> unit   = "caml_lock_analysis_analyze"

  (** [is_protected la n1 n2] returns [true] if [n1] and [n2] are both
      protected by a common lock. *)
  external is_protected : lock_analysis -> icfg_node -> icfg_node -> bool
                                                   = "caml_lock_analysis_is_protected"
end

(** Svf — SVF lifecycle management for SVF-OCaml.
    Mirrors the PySVF singleton class from SVF-Python.

    The typical usage pattern is:
    {[
      (* 1. Build SVFIR (PAG) and run Andersen pointer analysis *)
      let pag = Svf.get_pag "/path/to/bitcode.bc" in

      (* 2. Get the ICFG from the PAG *)
      let icfg = Svfir.get_icfg pag in

      (* 3. Iterate ICFG nodes *)
      let nodes = Graphs.Icfg.get_nodes icfg in
      List.iter (fun n ->
        print_endline (Graphs.IcfgNode.to_string n)) nodes;

      (* 4. Release resources when done *)
      Svf.release_pag ()
    ]}
*)

open Types

(** Analysis type for pointer analysis *)
type ptaty =
  | AndersenWaveDiff  (** Andersen wave-diff pointer analysis (default) *)
  | Steensgaard       (** Steensgaard's fast unification-based pointer analysis *)

(** {1 Flexible build / run API} *)

(** [get_svfir path] builds the SVFIR (PAG) from the LLVM bitcode file at [path].
    Does not run pointer analysis. *)
external get_svfir : string -> svfir = "caml_get_svfir"

(** [release_pag ()] releases all current analysis state:
    SVFIR, ICFG, PTA, and SVFG. *)
external release_pag : unit -> unit = "caml_release_pag"

(** [run_pta_andersen ()] runs AndersenWaveDiff pointer analysis on the
    current SVFIR.  Raises [Failure] if no SVFIR has been built. *)
external run_pta_andersen : unit -> unit = "caml_run_pta_andersen"

(** [run_pta_steensgaard ()] runs Steensgaard pointer analysis. *)
external run_pta_steensgaard : unit -> unit = "caml_run_pta_steensgaard"

(** [run_pta pta_type] runs the specified pointer analysis. *)
let run_pta = function
  | AndersenWaveDiff -> run_pta_andersen ()
  | Steensgaard      -> run_pta_steensgaard ()

(** {1 Easy all-in-one API} *)

(** [get_pag path] builds the SVFIR from [path] and runs AndersenWaveDiff
    pointer analysis.  Returns the SVFIR (PAG).
    This is the most common entry point, equivalent to Python's [pysvf.get_pag()]. *)
external get_pag : string -> svfir = "caml_get_pag"

(** [build_svf_module options] parses SVF command-line [options] (including the
    bitcode file path) and builds the SVFIR + runs Andersen PTA, optionally also
    building the SVFG if ["-svfg"] is in [options]. *)
external build_svf_module : string list -> unit = "caml_build_svf_module"

(** {1 Getters for the current singleton state} *)

(** [get_current_pag ()] returns the current SVFIR.
    Raises [Failure] if not built. *)
external get_current_pag : unit -> svfir = "caml_get_current_pag"

(** [get_pta ()] returns the current pointer analysis instance.
    Raises [Failure] if PTA has not been run. *)
external get_pta : unit -> andersen_base = "caml_get_pta"

(** [get_icfg ()] returns the current ICFG. *)
external get_icfg : unit -> icfg = "caml_get_icfg_global"

(** [get_call_graph ()] returns the current call graph. *)
external get_call_graph : unit -> call_graph = "caml_get_call_graph_global"

(** [get_module_name ()] returns the path of the last analyzed bitcode file. *)
external get_module_name : unit -> string = "caml_get_module_name"

(** [is_ext_call fun_obj] returns [true] if the given function is an external
    (library) call. *)
external is_ext_call : fun_obj_var -> bool = "caml_is_ext_call"

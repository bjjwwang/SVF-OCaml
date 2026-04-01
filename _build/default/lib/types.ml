(** Types shared across all SVF-OCaml modules.
    All types are opaque (abstract): they are custom OCaml blocks
    wrapping raw C++ pointers. *)

(** SVFIR — Program Assignment Graph (PAG) *)
type svfir

(** ICFG — Interprocedural Control Flow Graph *)
type icfg

(** A node in the ICFG *)
type icfg_node

(** An edge in the ICFG *)
type icfg_edge

(** An SVF IR statement (edge in the PAG) *)
type svf_stmt

(** An SVF IR variable (node in the PAG) *)
type svf_var

(** An SVF type descriptor *)
type svf_type

(** A basic block *)
type svf_basic_block

(** Call graph *)
type call_graph

(** Call graph node *)
type call_graph_node

(** Call graph edge *)
type call_graph_edge

(** Call graph SCC (SCCDetection over CallGraph) *)
type call_graph_scc

(** SVFG — Sparse Value Flow Graph *)
type svfg

(** VFG — Value Flow Graph *)
type vfg

(** VFG node *)
type vfg_node

(** SVFG node *)
type svfg_node

(** VFG edge / SVFG edge *)
type vfg_edge

(** Andersen-family pointer analysis (base) *)
type andersen_base

(** AndersenWaveDiff pointer analysis *)
type andersen_wave_diff

(** Steensgaard pointer analysis *)
type steensgaard

(** A points-to set *)
type points_to

(** Constraint graph (used by Andersen analysis) *)
type constraint_graph

(** Constraint graph node *)
type constraint_node

(** Constraint graph edge *)
type constraint_edge

(** MTA — Multi-Thread Analysis *)
type mta

(** MHP — May-Happen-in-Parallel analysis *)
type mhp

(** Lock set analysis *)
type lock_analysis

(** TCT — Thread Creation Tree *)
type tct

(** Abstract state (for abstract interpretation) *)
type abstract_state

(** Abstract value (for abstract interpretation) — owned, heap-allocated *)
type abstract_value

(** Interval value (for abstract interpretation) — owned, heap-allocated *)
type interval_value

(** Address value (for abstract interpretation) — owned, heap-allocated *)
type address_value

(** Bounded integer — owned, heap-allocated *)
type bounded_int

(** Function object variable *)
type fun_obj_var

(** Type aliases *)
type node_id = int
type edge_id = int

(** demo.ml — SVF-OCaml tutorial and demonstration.

    This file demonstrates the main SVF-OCaml APIs, covering:
    1. Building SVFIR (PAG) from a bitcode file
    2. Walking the ICFG (nodes and edges)
    3. Inspecting SVF statements (type checks and content)
    4. Running pointer analysis and querying points-to information
    5. Traversing the call graph
    6. Creating and using a PointsTo set
    7. Multi-thread analysis (MTA)

    Compile and run:
      dune exec ./demo/demo.exe -- path/to/bitcode.bc

    Or load interactively with utop:
      #require "svf-ocaml";;
      open Svf_ocaml;;
*)

open Svf_ocaml

(* ============================================================
   Helper utilities
   ============================================================ *)

let separator title =
  Printf.printf "\n%s\n%s\n" title (String.make (String.length title) '=')

(* ============================================================
   1. Build SVFIR from a bitcode file
   ============================================================ *)
let demo_build_svfir bitcode_path =
  separator "1. Build SVFIR (PAG) + Andersen PTA";
  let pag = Svf.get_pag bitcode_path in
  Printf.printf "Module : %s\n" (Svfir.get_module_identifier pag);
  Printf.printf "#vars  : %d\n" (Svfir.get_svf_var_num pag);
  pag

(* ============================================================
   2. Walk the ICFG
   ============================================================ *)
let demo_icfg pag =
  separator "2. ICFG nodes";
  let icfg = Svfir.get_icfg pag in
  let nodes = Graphs.Icfg.get_nodes icfg in
  Printf.printf "Total ICFG nodes: %d\n" (List.length nodes);

  (* Print the first 5 nodes as a sample *)
  let sample = if List.length nodes > 5 then
    let rec take n = function [] -> [] | x :: xs -> if n = 0 then [] else x :: take (n-1) xs in
    take 5 nodes
  else nodes in

  List.iter (fun node ->
    Printf.printf "  [%d] %s%s\n"
      (Graphs.IcfgNode.get_id node)
      (if Graphs.IcfgNode.is_fun_entry node then "[FunEntry] " else "")
      (Graphs.IcfgNode.to_string node)
  ) sample;
  Printf.printf "  ... (showing first %d of %d nodes)\n"
    (List.length sample) (List.length nodes);
  icfg

(* ============================================================
   3. Inspect SVF statements
   ============================================================ *)
let demo_statements icfg =
  separator "3. SVF Statements";
  let nodes = Graphs.Icfg.get_nodes icfg in
  let count = ref 0 in
  List.iter (fun node ->
    let stmts = Graphs.IcfgNode.get_svf_stmts node in
    List.iter (fun stmt ->
      if !count < 10 then begin
        Printf.printf "  Stmt [%d->%d]: %s\n"
          (Svfir.SvfStmt.get_src_id stmt)
          (Svfir.SvfStmt.get_dst_id stmt)
          (Svfir.SvfStmt.to_string stmt);
        Printf.printf "    addr=%b copy=%b store=%b load=%b cmp=%b binary=%b\n"
          (Svfir.SvfStmt.is_addr_stmt stmt)
          (Svfir.SvfStmt.is_copy_stmt stmt)
          (Svfir.SvfStmt.is_store_stmt stmt)
          (Svfir.SvfStmt.is_load_stmt stmt)
          (Svfir.SvfStmt.is_cmp_stmt stmt)
          (Svfir.SvfStmt.is_binary_op_stmt stmt);
        incr count
      end
    ) stmts
  ) nodes;
  Printf.printf "  (showed first %d statements)\n" !count

(* ============================================================
   4. Points-to information
   ============================================================ *)
let demo_pta pag =
  separator "4. Pointer Analysis — points-to sets";
  let pta = Svf.get_pta () in
  (* Print points-to for the first 5 PAG nodes *)
  let count = ref 0 in
  let pairs = Svfir.iter pag in
  List.iter (fun (id, _var) ->
    if !count < 5 then begin
      let pts = Wpa.AndersenBase.get_pts pta id in
      let pts_list = Wpa.PointsTo.to_list pts in
      if pts_list <> [] then begin
        Printf.printf "  pts(%d) = {%s}\n" id
          (String.concat ", " (List.map string_of_int pts_list));
        incr count
      end
    end
  ) pairs;

  (* Also demonstrate manual PointsTo set creation *)
  separator "4b. Manual PointsTo set";
  let pts = Wpa.PointsTo.create () in
  Wpa.PointsTo.set pts 1;
  Wpa.PointsTo.set pts 3;
  Wpa.PointsTo.set pts 7;
  Printf.printf "  Manual pts = {%s}\n" (Wpa.PointsTo.to_string pts)

(* ============================================================
   5. Call graph traversal
   ============================================================ *)
let demo_call_graph () =
  separator "5. Call Graph";
  let cg = Svf.get_call_graph () in
  let nodes = Graphs.CallGraph.get_nodes cg in
  Printf.printf "Total call graph nodes: %d\n" (List.length nodes);
  List.iteri (fun i node ->
    if i < 5 then
      Printf.printf "  [%d] %s\n"
        (Graphs.CallGraphNode.get_id node)
        (Graphs.CallGraphNode.get_name node)
  ) nodes;
  if List.length nodes > 5 then
    Printf.printf "  ... (%d total)\n" (List.length nodes)

(* ============================================================
   6. ICFG edges
   ============================================================ *)
let demo_edges icfg =
  separator "6. ICFG Edges (sample)";
  let nodes = Graphs.Icfg.get_nodes icfg in
  let count = ref 0 in
  List.iter (fun node ->
    let out_edges = Graphs.IcfgNode.get_out_edges node in
    List.iter (fun edge ->
      if !count < 5 then begin
        Printf.printf "  %s\n" (Graphs.IcfgEdge.to_string edge);
        Printf.printf "    intra=%b call=%b ret=%b\n"
          (Graphs.IcfgEdge.is_intra_cfg_edge edge)
          (Graphs.IcfgEdge.is_call_cfg_edge edge)
          (Graphs.IcfgEdge.is_ret_cfg_edge edge);
        incr count
      end
    ) out_edges
  ) nodes;
  Printf.printf "  (showed first %d edges)\n" !count

(* ============================================================
   Main entry point
   ============================================================ *)
let () =
  let bitcode_path =
    if Array.length Sys.argv > 1 then Sys.argv.(1)
    else begin
      Printf.eprintf
        "Usage: %s <bitcode.bc>\n\
         Tip: use a .bc file compiled with clang -O0 -emit-llvm -c\n"
        Sys.argv.(0);
      exit 1
    end
  in

  Printf.printf "SVF-OCaml Demo\n";
  Printf.printf "Bitcode: %s\n" bitcode_path;

  let pag = demo_build_svfir bitcode_path in
  let icfg = demo_icfg pag in
  demo_statements icfg;
  demo_pta pag;
  demo_call_graph ();
  demo_edges icfg;

  separator "Done";
  Svf.release_pag ()

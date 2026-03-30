(** test2.ml — SVF-OCaml test for statement-level queries.
    Mirrors SVF-Python/test_cases/test2.py:

      pag = pysvf.get_pag("BASIC_array_2d_0-0.c.bc")
      icfg = pag.get_icfg()
      nodes = icfg.get_nodes()
      node_16 = icfg.get_gnode(16)
      print("Node 16:", node_16.to_string())
      for stmt in node_16.get_svf_stmts():
          print("Stmt::", stmt.to_string())
          print("Is CMP: " + str(stmt.is_cmp_stmt()))
          print("Is Addr: " + str(stmt.is_addr_stmt()))
*)

open Svf_ocaml

let () =
  let bc = "./BASIC_array_2d_0-0.c.bc" in
  let pag = Svf.get_pag bc in

  let icfg = Svfir.get_icfg pag in
  let nodes = Graphs.Icfg.get_nodes icfg in

  (* Sanity check: we have nodes *)
  assert (List.length nodes > 0);

  (* Get node with ID 16 *)
  let node_16 = Graphs.Icfg.get_gnode icfg 16 in
  Printf.printf "Node 16: %s\n" (Graphs.IcfgNode.to_string node_16);

  let stmts = Graphs.IcfgNode.get_svf_stmts node_16 in
  List.iter (fun stmt ->
    Printf.printf "Stmt:: %s\n" (Svfir.SvfStmt.to_string stmt);
    Printf.printf "Is CMP: %b\n"  (Svfir.SvfStmt.is_cmp_stmt stmt);
    Printf.printf "Is Addr: %b\n" (Svfir.SvfStmt.is_addr_stmt stmt)
  ) stmts;

  (* Verify statement type queries work on a broader set of nodes *)
  let addr_count = ref 0 in
  let cmp_count  = ref 0 in
  List.iter (fun node ->
    List.iter (fun stmt ->
      if Svfir.SvfStmt.is_addr_stmt stmt then incr addr_count;
      if Svfir.SvfStmt.is_cmp_stmt  stmt then incr cmp_count
    ) (Graphs.IcfgNode.get_svf_stmts node)
  ) nodes;
  Printf.printf "Total Addr stmts: %d\n" !addr_count;
  Printf.printf "Total CMP  stmts: %d\n" !cmp_count;

  Svf.release_pag ();
  print_endline "test2.ml: PASSED"

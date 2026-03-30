(** test.ml — Basic SVF-OCaml integration test.
    Mirrors SVF-Python/test_cases/test.py:

      pag = pysvf.get_pag("./BASIC_array_2d_0-0.c.bc")
      icfg = pag.get_icfg()
      nodes = icfg.get_nodes()
      for node in nodes:
          print(node.to_string())
*)

open Svf_ocaml

let () =
  let bc = "./BASIC_array_2d_0-0.c.bc" in
  let pag = Svf.get_pag bc in

  let icfg = Svfir.get_icfg pag in
  let nodes = Graphs.Icfg.get_nodes icfg in

  (* Verify that we got some nodes *)
  assert (List.length nodes > 0);

  (* Print all nodes (equivalent of Python test output) *)
  List.iter (fun node ->
    Printf.printf "%s\n" (Graphs.IcfgNode.to_string node)
  ) nodes;

  Svf.release_pag ();
  print_endline "test.ml: PASSED"

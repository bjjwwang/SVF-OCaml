(lang dune 3.0)
(name svf-ocaml)
(generate_opam_files true)

(package
 (name svf-ocaml)
 (synopsis "OCaml bindings for the SVF static analysis framework")
 (description
  "SVF-OCaml provides OCaml bindings for SVF (Static Value-Flow Analysis), \
   a static analysis framework for LLVM-based programs. It exposes SVF's \
   interprocedural control flow graph (ICFG), pointer analysis, value flow \
   graphs, and more.")
 (depends
  (ocaml (>= 4.14))
  dune
  (alcotest :with-test)))

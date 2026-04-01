# SVF-OCaml

OCaml bindings for the [SVF](https://github.com/SVF-tools/SVF) static analysis framework, built on top of LLVM. SVF-OCaml is the OCaml counterpart of [SVF-Python](https://github.com/SVF-tools/SVF-Python) and exposes the same core analysis APIs.

## Overview

SVF-OCaml uses the standard OCaml C FFI (`external` declarations + C++ stub files) to bridge SVF's C++ API into OCaml. Each SVF analysis component maps to one OCaml module and one C++ stub file, mirroring the SVF-Python `pybind/` layout:

| OCaml module | C++ stub file | SVF component |
|---|---|---|
| `Svf` | `svf_stubs.cpp` | Lifecycle management (build PAG, run PTA) |
| `Svfir` | `svfir_stubs.cpp` | SVFIR / PAG, SVFStmt, SVFVar |
| `Graphs` | `graphs_stubs.cpp` | ICFG, CallGraph, basic blocks |
| `Wpa` | `wpa_stubs.cpp` | Pointer analysis (Andersen, Steensgaard, PointsTo) |
| `Mta` | `mta_stubs.cpp` | Multi-thread analysis (MTA, MHP, LockAnalysis, TCT) |
| `Ae` | `ae_stubs.cpp` | Abstract execution (AbstractState, IntervalValue) |
| `MemoryModel` | `memory_model_stubs.cpp` | Memory model / PointsTo |
| `Saber`, `Dda`, `Cfl`, `Mssa`, `Util` | *(stubs)* | Planned — not yet implemented |

## Prerequisites

- OCaml ≥ 4.14 and [opam](https://opam.ocaml.org/)
- [dune](https://dune.build/) build system
- [Node.js / npm](https://nodejs.org/) — used to install `svf-lib`
- C++17 compiler (gcc ≥ 9 or clang ≥ 11)
- CMake ≥ 3.16

SVF library files (LLVM 18, Z3) are fetched automatically via the `svf-lib` npm package.

## Quick Start

```bash
# 1. Clone
git clone https://github.com/your-org/SVF-OCaml.git
cd SVF-OCaml

# 2. Build (installs svf-lib via npm automatically)
./build.sh

# 3. Run tests
cd test_cases
SVF_DIR=... LLVM_DIR=... Z3_DIR=... CXX_LINK_FLAGS=... dune exec ./test.exe
SVF_DIR=... LLVM_DIR=... Z3_DIR=... CXX_LINK_FLAGS=... dune exec ./test2.exe

# 4. Run the demo with a bitcode file
cd ..
SVF_DIR=... LLVM_DIR=... Z3_DIR=... CXX_LINK_FLAGS=... \
  dune exec ./demo/demo.exe -- path/to/program.bc
```

## Build from Source

```bash
# Install OCaml dependencies
opam install dune alcotest

# Install SVF libraries (platform-specific paths)
npm install -g svf-lib
export SVF_LIB_DIR=$(npm root -g)/svf-lib

# macOS
export SVF_DIR=$SVF_LIB_DIR/SVF-osx
export LLVM_DIR=$(brew --prefix llvm@18)
export Z3_DIR=$(brew --prefix z3)
export CXX_LINK_FLAGS=-lc++

# Linux (x86_64)
# export SVF_DIR=$SVF_LIB_DIR/SVF-linux-x86_64
# export LLVM_DIR=$SVF_LIB_DIR/llvm-18.1.0.obj
# export Z3_DIR=$SVF_LIB_DIR/z3.obj
# export CXX_LINK_FLAGS=-lstdc++

# Build the library
SVF_DIR=$SVF_DIR LLVM_DIR=$LLVM_DIR Z3_DIR=$Z3_DIR CXX_LINK_FLAGS=$CXX_LINK_FLAGS dune build
```

### Setting up the runtime library path

Before running the built executables, ensure the SVF/LLVM/Z3 shared libraries are on the dynamic linker path:

**Linux:**
```bash
export LD_LIBRARY_PATH=$SVF_DIR/lib:$LLVM_DIR/lib:$Z3_DIR/lib:$LD_LIBRARY_PATH
```

**macOS:**
```bash
export DYLD_LIBRARY_PATH=$SVF_DIR/lib:$LLVM_DIR/lib:$Z3_DIR/lib:$DYLD_LIBRARY_PATH
```

## API Usage

### Basic workflow

```ocaml
open Svf_ocaml

(* Build SVFIR (PAG) and run Andersen pointer analysis *)
let pag = Svf.get_pag "/path/to/bitcode.bc"

(* Get the ICFG *)
let icfg = Svfir.get_icfg pag

(* Iterate over all ICFG nodes *)
let nodes = Graphs.Icfg.get_nodes icfg
let () = List.iter (fun node ->
  Printf.printf "%s\n" (Graphs.IcfgNode.to_string node)
) nodes

(* Inspect statements at a specific node *)
let node = Graphs.Icfg.get_gnode icfg 16
let stmts = Graphs.IcfgNode.get_svf_stmts node
let () = List.iter (fun stmt ->
  Printf.printf "Is CMP: %b  Is Addr: %b  %s\n"
    (Svfir.SvfStmt.is_cmp_stmt stmt)
    (Svfir.SvfStmt.is_addr_stmt stmt)
    (Svfir.SvfStmt.to_string stmt)
) stmts

(* Query points-to information *)
let pta = Svf.get_pta ()
let pts = Wpa.AndersenBase.get_pts pta 42
let () = Printf.printf "pts(42) = {%s}\n" (Wpa.PointsTo.to_string pts)

(* Release resources *)
let () = Svf.release_pag ()
```

### Pointer analysis

```ocaml
(* Default: Andersen wave-diff (via get_pag) *)
let pag = Svf.get_pag "program.bc"

(* Or run manually *)
let pag = Svf.get_svfir "program.bc"
let ()  = Svf.run_pta Svf.Steensgaard

(* Direct Andersen instance *)
let ander = Wpa.AndersenBase.create pag
let ()    = Wpa.AndersenBase.analyze ander
let pts   = Wpa.AndersenBase.get_pts ander 10
```

### Multi-thread analysis

```ocaml
let pag = Svf.get_pag "threaded_program.bc"
let mta = Mta.Mta.create ()
let ()  = Mta.Mta.run_on_module mta pag
let pta = Svf.get_pta ()
let tct = Mta.Tct.create pta
let mhp = Mta.Mhp.create tct
let ()  = Mta.Mhp.analyze mhp
let node1 = Graphs.Icfg.get_gnode icfg 10
let node2 = Graphs.Icfg.get_gnode icfg 20
let par = Mta.Mhp.may_happen_in_parallel mhp node1 node2
```

## Module Reference

### `Svf` — Lifecycle management

| Function | Description |
|---|---|
| `get_pag path` | Build SVFIR + run Andersen PTA from bitcode path |
| `get_svfir path` | Build SVFIR only (no PTA) |
| `run_pta pta_type` | Run `AndersenWaveDiff` or `Steensgaard` |
| `release_pag ()` | Release all analysis state |
| `get_current_pag ()` | Get current SVFIR |
| `get_pta ()` | Get current PTA instance |
| `get_icfg ()` | Get current ICFG |
| `get_call_graph ()` | Get current call graph |
| `build_svf_module opts` | Build from SVF-style command-line options |

### `Svfir` — PAG / SVFIR

| Function | Description |
|---|---|
| `get_icfg pag` | Get ICFG from PAG |
| `get_call_graph pag` | Get call graph from PAG |
| `get_svf_var_num pag` | Number of SVF variables |
| `get_gnode pag id` | Get SVF variable by ID |
| `SvfStmt.is_*_stmt stmt` | Type-check a statement |
| `SvfStmt.to_string stmt` | String representation |
| `SvfVar.get_name var` | Variable name |

### `Graphs` — ICFG, CallGraph

| Function | Description |
|---|---|
| `Icfg.get_nodes icfg` | All ICFG nodes |
| `Icfg.get_gnode icfg id` | ICFG node by ID |
| `IcfgNode.to_string node` | String representation |
| `IcfgNode.get_svf_stmts node` | Statements at this node |
| `IcfgNode.is_fun_entry node` | Is function entry? |
| `IcfgNode.get_out_edges node` | Successor edges |
| `CallGraph.get_nodes cg` | All call graph nodes |

### `Wpa` — Pointer Analysis

| Function | Description |
|---|---|
| `AndersenBase.create pag` | Create Andersen analysis |
| `AndersenBase.analyze pa` | Run the analysis |
| `AndersenBase.get_pts pa id` | Points-to set for variable `id` |
| `AndersenBase.alias pa id1 id2` | Alias query |
| `PointsTo.create ()` | New empty points-to set |
| `PointsTo.set pts id` | Add `id` to set |
| `PointsTo.to_list pts` | Get elements as list |

## Testing

Tests are in `test_cases/` and use the `BASIC_array_2d_0-0.c.bc` bitcode file:

```bash
cd test_cases
SVF_DIR=... LLVM_DIR=... Z3_DIR=... dune exec ./test.exe
SVF_DIR=... LLVM_DIR=... Z3_DIR=... dune exec ./test2.exe
```

The CI workflow (`.github/workflows/ci.yml`) runs these tests on Ubuntu 24.04 and macOS 14 on every push to `main`.

## TODO — Publishing / Release

When the project reaches a stable API, the recommended release approach for the OCaml ecosystem is:

1. **opam package** — publish to the [opam repository](https://opam.ocaml.org/) so users can install via `opam install svf-ocaml`. The opam file is auto-generated by dune from `dune-project`.

2. **Bundled native libraries** — similar to SVF-Python's wheel approach, bundle the SVF/LLVM/Z3 `.so`/`.dylib` files into an opam release. This can be done by:
   - Having a `conf-svf-lib` opam package that sets `SVF_DIR`/`LLVM_DIR`/`Z3_DIR` from the npm-installed `svf-lib`.
   - OR distributing pre-built `.a` (static) libraries for each platform and linking them statically.

3. **Version pinning** — pin to specific SVF/LLVM versions. The current build uses SVF from the `svf-lib` npm package (LLVM 18).

4. **Platform matrix** — build and test on `linux-x86_64`, `linux-aarch64`, `macos-x86_64`, `macos-aarch64`, same as SVF-Python.

5. **GitHub Releases** — tag releases in the repository and attach platform-specific pre-built artifacts.

6. **CI for releases** — add a `release.yml` workflow (see SVF-Python's `.github/workflows/release.yml`) that automatically publishes to the opam repository on tag.

## License

GNU Affero General Public License v3.0 — same as SVF.

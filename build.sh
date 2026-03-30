#!/bin/bash
# build.sh — Build SVF-OCaml from source.
#
# Usage:
#   ./build.sh
#
# Environment variables (optional — auto-detected via npm svf-lib):
#   SVF_DIR   — path to the platform SVF dir (must contain include/ and lib/)
#   LLVM_DIR  — path to LLVM installation
#   Z3_DIR    — path to Z3 installation
#
# Example (manual):
#   SVF_DIR=/opt/svf-lib/SVF-osx LLVM_DIR=/opt/homebrew/opt/llvm@18 Z3_DIR=/opt/homebrew/opt/z3 ./build.sh

set -e

# ----------------------------------------------------------------
# Step 1: Locate SVF libraries via npm svf-lib
# ----------------------------------------------------------------
if [ -z "$SVF_DIR" ] || [ ! -d "$SVF_DIR" ]; then
    echo "SVF_DIR not set or missing — installing via npm svf-lib..."

    if ! command -v npm &> /dev/null; then
        echo "npm not found. Please install Node.js or set SVF_DIR manually."
        exit 1
    fi

    npm install -g svf-lib || {
        echo "Failed to install svf-lib. Please set SVF_DIR, LLVM_DIR, Z3_DIR manually."
        exit 1
    }

    SVF_LIB_DIR=$(npm root -g)/svf-lib
    sysOS=$(uname -s)
    arch=$(uname -m)

    if [[ "$sysOS" == "Darwin" ]]; then
        export SVF_DIR="${SVF_LIB_DIR}/SVF-osx"
        export LLVM_DIR="$(brew --prefix llvm@18 2>/dev/null || echo /opt/homebrew/opt/llvm@18)"
        export Z3_DIR="$(brew --prefix z3 2>/dev/null || echo /opt/homebrew/opt/z3)"
        export CXX_LINK_FLAGS="-lc++"
    else
        export SVF_DIR="${SVF_LIB_DIR}/SVF-linux-${arch}"
        export LLVM_DIR="${SVF_LIB_DIR}/llvm-18.1.0.obj"
        export Z3_DIR="${SVF_LIB_DIR}/z3.obj"
        export CXX_LINK_FLAGS="-lstdc++"
    fi

    echo "SVF_DIR  = $SVF_DIR"
    echo "LLVM_DIR = $LLVM_DIR"
    echo "Z3_DIR   = $Z3_DIR"
else
    echo "Using existing SVF_DIR=$SVF_DIR"
fi

if [ -z "$SVF_DIR" ] || [ -z "$LLVM_DIR" ] || [ -z "$Z3_DIR" ]; then
    echo "Please define SVF_DIR, LLVM_DIR, and Z3_DIR."
    exit 1
fi

# ----------------------------------------------------------------
# Step 2: Ensure OCaml / opam is available
# ----------------------------------------------------------------
if ! command -v opam &> /dev/null; then
    echo "opam not found. Please install OCaml and opam."
    echo "  Linux:  bash -c \"\$(curl -fsSL https://opam.ocaml.org/install.sh)\""
    echo "  macOS:  brew install opam"
    exit 1
fi

if ! opam env &> /dev/null; then
    opam init --no-setup -y
fi

eval "$(opam env)"

echo "Installing OCaml dependencies..."
opam install -y dune alcotest || true

# ----------------------------------------------------------------
# Step 3: Build
# ----------------------------------------------------------------
echo ""
echo "Building SVF-OCaml..."
SVF_DIR="${SVF_DIR}" LLVM_DIR="${LLVM_DIR}" Z3_DIR="${Z3_DIR}" \
    dune build

echo ""
echo "Build succeeded."
echo ""
echo "To run the tests:"
echo "  SVF_DIR='$SVF_DIR' LLVM_DIR='$LLVM_DIR' Z3_DIR='$Z3_DIR' \\"
echo "    dune exec test_cases/test.exe -- test_cases/BASIC_array_2d_0-0.c.bc"
echo "  SVF_DIR='$SVF_DIR' LLVM_DIR='$LLVM_DIR' Z3_DIR='$Z3_DIR' \\"
echo "    dune exec test_cases/test2.exe -- test_cases/BASIC_array_2d_0-0.c.bc"

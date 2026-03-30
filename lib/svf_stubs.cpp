/**
 * svf_stubs.cpp
 *
 * OCaml C FFI stubs for SVF lifecycle management.
 * Mirrors the PySVF singleton class from SVF-Python/pybind/svf_pybind.cpp.
 */

#include "svf_ocaml_types.h"

#include "SVF-LLVM/SVFIRBuilder.h"
#include "SVF-LLVM/LLVMUtil.h"
#include "Graphs/ICFG.h"
#include "WPA/Andersen.h"
#include "WPA/Steensgaard.h"
#include "SVFIR/SVFVariables.h"
#include "Util/Options.h"
#include "Util/CommandLine.h"
#include "Util/SVFUtil.h"
#include "MemoryModel/PointerAnalysis.h"

#include <filesystem>
#include <memory>
#include <stdexcept>

using namespace SVF;

/* =========================================================================
 * Singleton state (mirrors PySVF static members)
 * ========================================================================= */
static SVFIR*                        g_svfir        = nullptr;
static ICFG*                         g_icfg         = nullptr;
static SVFGBuilder*                  g_svfg_builder = nullptr;
static SVFG*                         g_svfg         = nullptr;
static std::string                   g_last_module  = "";
static std::shared_ptr<AndersenBase> g_pta          = nullptr;

/* =========================================================================
 * Internal C++ helpers
 * ========================================================================= */
static void impl_release_svfir() {
    LLVMModuleSet::releaseLLVMModuleSet();
    SVFIR::releaseSVFIR();
    g_svfir = nullptr;
    g_icfg  = nullptr;
    NodeIDAllocator::unset();
}

static void impl_release_pta() {
    g_pta = nullptr;
}

static void impl_release_all() {
    impl_release_svfir();
    impl_release_pta();
    if (g_svfg_builder) {
        delete g_svfg_builder;
        g_svfg_builder = nullptr;
        g_svfg = nullptr;
    }
}

static SVFIR* impl_build_svfir(const std::string& path) {
    if (!std::filesystem::exists(std::filesystem::path(path)))
        throw std::runtime_error("Bitcode file does not exist: " + path);
    impl_release_svfir();

    std::vector<std::string> module_names = {path};
    Options::UsePreCompFieldSensitive.setValue(false);
    Options::ModelConsts.setValue(true);
    Options::ModelArrays.setValue(true);

    LLVMModuleSet::buildSVFModule(module_names);
    SVFIRBuilder builder;
    builder.build();
    g_svfir = builder.getPAG();
    g_icfg  = g_svfir->getICFG();
    return g_svfir;
}

static void impl_run_pta(bool use_andersen) {
    if (!g_svfir)
        throw std::runtime_error("SVFIR not built. Call get_pag first.");
    impl_release_pta();

    if (use_andersen)
        g_pta = std::make_shared<AndersenWaveDiff>(g_svfir);
    else
        g_pta = std::make_shared<Steensgaard>(g_svfir);

    g_pta->analyze();
    PointerAnalysis::CallGraphSCC* scc = g_pta->getCallGraphSCC();
    scc->find();
    CallGraph* cg = g_pta->getCallGraph();
    g_icfg->updateCallGraph(cg);
    SVFIRBuilder updater;
    updater.updateCallGraph(cg);
}

/* =========================================================================
 * CAMLprim callbacks
 * ========================================================================= */

extern "C" CAMLprim value caml_get_svfir(value v_path) {
    CAMLparam1(v_path);
    try { impl_build_svfir(std::string(String_val(v_path))); }
    catch (const std::exception& e) { caml_failwith(e.what()); }
    CAMLreturn(wrap_svfir(g_svfir));
}

extern "C" CAMLprim value caml_release_pag(value v_unit) {
    CAMLparam1(v_unit);
    impl_release_all();
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_run_pta_andersen(value v_unit) {
    CAMLparam1(v_unit);
    try { impl_run_pta(true); }
    catch (const std::exception& e) { caml_failwith(e.what()); }
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_run_pta_steensgaard(value v_unit) {
    CAMLparam1(v_unit);
    try { impl_run_pta(false); }
    catch (const std::exception& e) { caml_failwith(e.what()); }
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_get_pag(value v_path) {
    CAMLparam1(v_path);
    try {
        std::string path = std::string(String_val(v_path));
        impl_build_svfir(path);
        impl_run_pta(true);
        g_last_module = path;
    } catch (const std::exception& e) { caml_failwith(e.what()); }
    CAMLreturn(wrap_svfir(g_svfir));
}

extern "C" CAMLprim value caml_get_current_pag(value v_unit) {
    CAMLparam1(v_unit);
    if (!g_svfir) caml_failwith("No PAG built yet. Call get_pag first.");
    CAMLreturn(wrap_svfir(g_svfir));
}

extern "C" CAMLprim value caml_get_pta(value v_unit) {
    CAMLparam1(v_unit);
    if (!g_pta) caml_failwith("No PTA run yet. Call get_pag first.");
    CAMLreturn(wrap_andersen_base(g_pta.get()));
}

extern "C" CAMLprim value caml_get_icfg_global(value v_unit) {
    CAMLparam1(v_unit);
    if (!g_icfg) caml_failwith("No ICFG available. Call get_pag first.");
    CAMLreturn(wrap_icfg(g_icfg));
}

extern "C" CAMLprim value caml_get_call_graph_global(value v_unit) {
    CAMLparam1(v_unit);
    if (!g_pta) caml_failwith("No PTA run yet. Call get_pag first.");
    CAMLreturn(wrap_call_graph(g_pta->getCallGraph()));
}

extern "C" CAMLprim value caml_get_module_name(value v_unit) {
    CAMLparam1(v_unit);
    CAMLreturn(caml_copy_string(g_last_module.c_str()));
}

extern "C" CAMLprim value caml_build_svf_module(value v_options) {
    CAMLparam1(v_options);
    try {
        std::vector<std::string> options;
        value cur = v_options;
        while (cur != Val_int(0)) {
            options.push_back(std::string(String_val(Field(cur, 0))));
            cur = Field(cur, 1);
        }
        if (options.empty()) throw std::runtime_error("No options provided.");

        std::vector<char*> argv;
        std::string prog = "";
        argv.push_back(&prog[0]);
        for (auto& s : options) argv.push_back(&s[0]);

        bool build_svfg = false;
        for (auto& s : options)
            if (s == "-svfg") { build_svfg = true; break; }

        std::vector<std::string> module_names = OptionBase::parseOptions(
            (int)argv.size(), argv.data(), "SVFOCaml",
            "[options] <input-bitcode...>");

        if (module_names.empty())
            throw std::runtime_error("No bitcode file in options.");

        impl_build_svfir(module_names[0]);
        impl_run_pta(true);

        if (build_svfg) {
            if (g_svfg_builder) { delete g_svfg_builder; g_svfg_builder = nullptr; g_svfg = nullptr; }
            g_svfg_builder = new SVFGBuilder(g_svfir);
            g_svfg = g_svfg_builder->buildFullSVFG(g_pta.get());
        }
        g_last_module = module_names[0];
    } catch (const std::exception& e) { caml_failwith(e.what()); }
    CAMLreturn(Val_unit);
}

extern "C" CAMLprim value caml_is_ext_call(value v_fun) {
    CAMLparam1(v_fun);
    FunObjVar* fun = unwrap_fun_obj_var(v_fun);
    CAMLreturn(Val_bool(SVFUtil::isExtCall(fun)));
}

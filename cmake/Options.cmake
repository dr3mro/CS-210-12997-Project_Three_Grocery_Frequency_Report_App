# =============================================================================
# Options.cmake - every user-facing switch, in one place
#
# Set them on the command line (-DGROCERY_SANITIZER=address), in a preset's
# cacheVariables, or in your own CMakeUserPresets.json. The modules only read
# these; none of them declares options of its own.
#
# Two more switches live in PreProject.cmake because they must exist before
# project(): GROCERY_USE_LLVM_CLANG and GROCERY_BUILD_TESTS. So does
# GROCERY_USE_VCPKG: the toolchain file is read during project().
# =============================================================================
include_guard(DIRECTORY)

# ---- what to build -------------------------------------------------------------
option(GROCERY_LIBRARY_MODE
       "Build an installable library package (with examples/) instead of an app" OFF)
option(GROCERY_BUILD_FUZZERS
       "Build the libFuzzer targets in fuzz/ (needs clang; the fuzz preset sets this)" OFF)

# ---- compiler checks -----------------------------------------------------------
# Warnings as errors: CMake's own CMAKE_COMPILE_WARNING_AS_ERROR (the ci
# preset sets it). `cmake --compile-no-warning-as-error` turns it off again.

set(GROCERY_SANITIZER "" CACHE STRING
    "Sanitizer to enable: address (includes UBSan), thread or memory; empty for none")
set_property(CACHE GROCERY_SANITIZER PROPERTY STRINGS "" address thread memory)

option(GROCERY_ENABLE_HARDENING
       "Add security hardening flags to optimised (Release-like) builds" ON)
option(GROCERY_ENABLE_LTO
       "Link-time optimisation for optimised builds of the app (not in library mode)" ON)
option(GROCERY_ENABLE_COVERAGE
       "Instrument for LLVM source-based coverage and add the coverage target (clang only)" OFF)

# ---- tooling -------------------------------------------------------------------
option(GROCERY_USE_CCACHE "Use ccache when it is installed" ON)
option(GROCERY_TIDY_WERROR "Make the tidy target fail on clang-tidy findings (CI)" OFF)
set(GROCERY_FUZZ_SECONDS "10" CACHE STRING
    "Seconds each fuzz target runs in the fuzz-run target")

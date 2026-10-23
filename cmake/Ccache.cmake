# =============================================================================
# Ccache.cmake - cache compiler results with ccache, when it is installed
#
# Speeds up rebuilds (switching branches, clean builds) by reusing earlier
# compiler output. Affects targets created after this point.
#
# Reads: GROCERY_USE_CCACHE (Options.cmake); turn it OFF if ccache misbehaves.
# =============================================================================
include_guard(DIRECTORY)

if(NOT GROCERY_USE_CCACHE)
    return()
endif()

find_program(GROCERY_CCACHE_PROGRAM ccache)
if(GROCERY_CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${GROCERY_CCACHE_PROGRAM}")
endif()

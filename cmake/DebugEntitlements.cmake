# =============================================================================
# DebugEntitlements.cmake - let macOS debugging tools attach to Debug builds
#
# Apple's leaks, heap, vmmap and Instruments can only inspect a process that
# carries the com.apple.security.get-task-allow entitlement; without it leaks
# says the process "is not debuggable". After every link of the app in a Debug
# configuration, this re-signs it ad hoc (codesign -s -) with that entitlement.
# Release-type builds are left alone: the entitlement lets other processes read
# the program's memory, which a shipped binary must not allow.
#
# Must be included after the app target exists.
#
# Reads: GROCERY_LIBRARY_MODE (Options.cmake)
# =============================================================================
include_guard(DIRECTORY)

if(NOT APPLE OR GROCERY_LIBRARY_MODE OR NOT TARGET ${PROJECT_NAME})
    return()
endif()

find_program(GROCERY_CODESIGN codesign)
if(NOT GROCERY_CODESIGN)
    message(STATUS "codesign not found: Debug builds will not get get-task-allow")
    return()
endif()

set(_grocery_entitlements "${PROJECT_BINARY_DIR}/debug.entitlements")
file(WRITE "${_grocery_entitlements}"
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
    "<plist version=\"1.0\">\n<dict>\n"
    "    <key>com.apple.security.get-task-allow</key>\n    <true/>\n"
    "</dict>\n</plist>\n")

# One generator expression per configuration so multi-config generators
# (Xcode) also sign only Debug; other configurations run a no-op.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "$<IF:$<CONFIG:Debug>,${GROCERY_CODESIGN};-s;-;-f;--entitlements;${_grocery_entitlements};$<TARGET_FILE:${PROJECT_NAME}>,${CMAKE_COMMAND};-E;true>"
    COMMAND_EXPAND_LISTS
    VERBATIM
    COMMENT "Signing ${PROJECT_NAME} with get-task-allow (Debug only)")

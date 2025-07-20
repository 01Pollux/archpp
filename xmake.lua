set_languages("c++23")

add_rules("mode.releasedbg", "mode.release")
add_extrafiles(".clang-format")
set_runtimes("MT")

includes("project/options.lua")
includes("project/xmake.lua")
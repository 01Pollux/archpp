includes("packages/xmake.lua")

includes("archpp/xmake.lua")

if is_config("with-tests", true) then
    includes("tests/xmake.lua")
end

target("app")
    set_kind("binary")
    add_files("app/**.cpp")
    add_deps("archpp", {public = true})
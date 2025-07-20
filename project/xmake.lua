includes("packages")
includes("archpp")

if is_config("with-tests", true) then
    includes("tests")
end

target("app")
    set_kind("binary")
    add_files("app/**.cpp")
    add_deps("archpp", {public = true})
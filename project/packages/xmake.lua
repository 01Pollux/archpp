includes("llvmlib.lua")
add_requires("llvmlib 20.1.8")

if has_config("test-framework", "boost") then
    add_requires("boost[cmake,regex,test] 1.88.0")
end
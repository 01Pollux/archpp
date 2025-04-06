local header_files = {
    {"include/archpp/*.hpp", {public = true}},
    {"include/archpp/core/*.hpp", {public = true}},
    {"include/archpp/filters/*.hpp", {public = true}},
    {"include/archpp/formatters/*.hpp", {public = true}},
    {"include/archpp/inputs/*.hpp", {public = true}},
    {"include/archpp/outcomes/*.hpp", {public = true}},
    {"include/archpp/rule/*.hpp", {public = true}},
    {"include/archpp/runners/*.hpp", {public = true}},
    {"include/archpp/test/*.hpp", {public = true}},

    {"src/rule/*.hpp", nil},
}

local source_files = {
    {"src/filters/*.cpp"},
    {"src/inputs/*.cpp"},
    {"src/rule/*.cpp"},
    {"src/runners/*.cpp"},
}

local extra_compiler_flags = {
    "cl::/wd4291", -- Disable C++ exception specification
    "cl::/bigobj", -- Enable support for large object files
}

local function setup_project_test_framework()
    local switch = {
        ["boost"] = function ()
            add_headerfiles("include/archpp/test/adapters/TestRunner.Boost.hpp", {public = true})
        end,
        ["none"] = function()
        end
    }
    switch[get_config("test-framework") or "none"]()
end

target("archpp")
    set_kind("static")
    add_packages("llvmlib", {public = true})

    add_includedirs("include", {public = true})
    for _, header in ipairs(header_files) do
        add_headerfiles(header[1], header[2])
    end
    for _, source in ipairs(source_files) do
        add_files(source[1])
    end

    if is_plat("windows") then
        add_links("ntdll", "version", {public = true})
        add_syslinks("advapi32", "ole32", "shell32", {public = true})
    end
    for _, flag in ipairs(extra_compiler_flags) do
        add_cxxflags(flag, {public = true})
    end
    
    setup_project_test_framework()
target_end()

local project_dir_define = path.join(os.projectdir(), "project", "archpp")
if not os.isdir(project_dir_define) then
    print("[Warning] Project directory does not exist: " .. project_dir_define)
end
project_dir_define = project_dir_define:gsub("\\", "/")

--

local function setup_test_project()
    local switch = {
        ["boost"] = function ()
            add_packages("boost", {public = false})
            add_defines("ARCHPP_USE_BOOST_TEST")
        end,
        ["none"] = function()
        end
    }
    switch[get_config("test-framework") or "none"]()
end

--

local paths = {
    "archpp"
}

for _, cur_path in ipairs(paths) do
    
    local all_sources = path.join(cur_path, "*.cpp")
    for _, file in ipairs(os.files(all_sources)) do
        local name = path.basename(file, ".cpp")
        target(name)
            set_group("tests/" .. cur_path)
            set_kind("binary")

            add_deps("archpp", {public = true})
            setup_test_project()

            add_defines("ARCHPP_PROJECT_DIR=\"" .. project_dir_define .. "\"")
            add_files(file)

            add_filegroups("", {rootdir = cur_path})
        target_end()
    end

end

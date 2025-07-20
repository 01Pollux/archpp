package("llvmlib")
    set_kind("library")
    set_homepage("https://llvm.org/")
    set_description("The LLVM Compiler Infrastructure")
    add_configs("shared",            {description = "Build shared library.", default = false, type = "boolean", readonly = true})

    if is_plat("windows") then
        if is_arch("arm.*") then
            set_urls("https://github.com/llvm/llvm-project/releases/download/llvmorg-$(version)/clang+llvm-$(version)-aarch64-pc-windows-msvc.tar.xz")
            add_versions("20.1.8", "0df3e81e8fe26370dd2b60b9e009d81cd130d3fdc41b257434aa663c5d9f0c13")
        else
            set_urls("https://github.com/llvm/llvm-project/releases/download/llvmorg-$(version)/clang+llvm-$(version)-x86_64-pc-windows-msvc.tar.xz")
            add_versions("20.1.8", "f229769f11d6a6edc8ada599c0cda964b7dee6ab1a08c6cf9dd7f513e85b107f")
        end
    else
        set_urls("https://github.com/llvm/llvm-project/releases/download/llvmorg-$(version)/llvm-project-$(version).src.tar.xz")
        add_versions("20.1.8", "d2060f7a2259d95d121e6a2ceaa948b7d724c497e6e0b752e39086eaaf1675c6")
    end

    on_install("windows", "msys", "linux", function (package)
        print("Copying LLVM library...")
        os.cp("*", package:installdir())

        package:addenv("PATH", "bin", {public = true})
        package:add("includedirs", "include", {public = true})
        package:add("linkdirs", "lib", {public = true})
        
        local get_base_links = import("components.base").get_links
        local get_clang_links = import("components.clang").get_links
        
        local function link_libs(links)
            for _, link in ipairs(links) do
                package:add("links", link, {public = true})
            end
        end

        link_libs(get_base_links(package))
        link_libs(get_clang_links(package))
    end)

    on_test(function (package)
        assert(package:has_cxxincludes("clang/Basic/SourceManager.h", {configs = {languages = "c++23"}}))
    end)

option("submit")
option_end()

local is_submit = has_config("submit")

add_rules("mode.debug", "mode.release")

if is_os("windows") then
    set_plat("mingw")
    set_toolchains("llvm")
elseif is_os("linux") then
    set_plat("linux")
    set_toolchains("gcc")
end

set_languages("c++17")
set_warnings("allextra")

if not is_submit then
    set_policy("build.sanitizer.address", true)
    set_policy("build.sanitizer.undefined", true)

    add_requires("catch2")
end

target("fla-core")
    set_kind("static")
    add_includedirs("fla-project", { public = true })
    add_files("fla-project/**/*.cpp")

target("fla")
    set_kind("binary")
    add_deps("fla-core")
    add_files("fla-project/*.cpp")
    set_rundir(os.projectdir())

if not is_submit then
    includes("tests")
end

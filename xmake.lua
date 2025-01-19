set_project("simple-fla")
set_version("0.1.0")

option("submit")
option_end()
option("ci")
option_end()

add_rules("mode.debug", "mode.release")

if has_config("ci") then
    set_languages("c++latest")
else
    set_languages("c++17")
end
set_warnings("allextra")

local is_submit = has_config("submit")

if not is_submit then
    if is_mode("debug") then
        set_policy("build.sanitizer.address", true)
        set_policy("build.sanitizer.undefined", true)
    end

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

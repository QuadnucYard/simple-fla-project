add_rules("mode.debug", "mode.release")

set_plat("mingw")
set_toolchains("llvm")
set_languages("c++17")
set_warnings("all")

set_policy("build.sanitizer.address", true)
set_policy("build.sanitizer.undefined", true)

add_requires("catch2")

target("fla-core")
    set_kind("static")
    add_includedirs("fla-project", { public = true })
	add_files("fla-project/**/*.cpp")

target("fla")
    set_kind("binary")
	add_deps("fla-core")
	add_files("fla-project/*.cpp")
	set_rundir(os.projectdir())

for _, file in ipairs(os.files("tests/test_*.cpp")) do
    local name = path.basename(file)
    target(name)
        set_kind("binary")
        set_default(false)
        add_deps("fla-core")
		add_packages("catch2")
        add_files("tests/" .. name .. ".cpp")
        add_tests("default")
        set_rundir("$(projectdir)")
end

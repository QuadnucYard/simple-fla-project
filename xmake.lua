add_rules("mode.debug", "mode.release")

set_plat("mingw")
set_toolchains("llvm")
set_languages("c++17")
set_warnings("all")

set_policy("build.sanitizer.address", true)
set_policy("build.sanitizer.undefined", true)

target("fla")
    set_kind("binary")
	add_includedirs("fla-project")
	add_files("fla-project/*.cpp", "fla-project/**/*.cpp")
	set_rundir(os.projectdir())

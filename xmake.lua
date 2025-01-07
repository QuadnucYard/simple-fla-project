add_rules("mode.debug", "mode.release")

set_plat("mingw")
set_toolchains("llvm")
set_languages("c++17")
set_warnings("all")
add_cxxflags("-fsanitize=address,undefined")
add_ldflags("-fsanitize=address")

target("fla")
    set_kind("binary")
	add_includedirs("fla-project")
	add_files("fla-project/*.cpp", "fla-project/**/*.cpp")
	set_rundir(os.projectdir())

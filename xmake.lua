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
    -- unit tests
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

    -- unit tests with CLI
    target("test_cases")
        set_kind("binary")
        set_default(false)
        add_deps("fla-core")
        add_files("fla-project/*.cpp")
        set_rundir(os.projectdir())

        on_load(function (target)
            import("scripts.parse_testcases")
            local testcases = parse_testcases(io.readfile("tests/fixtures/testcases.txt"))
            for i, case in ipairs(testcases) do
                target:add("tests", string.format("case-%02d", i), {
                    runargs = {path.join("tests/fixtures/assets", case.source), case.input},
                    pass_outputs = case.output,
                    trim_output = true,
                })
            end
        end)
end

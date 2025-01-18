-- unit tests
for _, file in ipairs(os.files("test_*.cpp")) do
    local name = path.basename(file)
    target(name)
        set_kind("binary")
        set_default(false)
        add_deps("fla-core")
        add_packages("catch2")
        add_files(name .. ".cpp")
        add_tests("default")
        set_rundir("$(projectdir)")
end

-- unit tests with CLI
target("test_cases")
    set_kind("binary")
    set_default(false)
    add_deps("fla-core")
    add_files("$(projectdir)/fla-project/*.cpp")
    set_rundir(os.projectdir())
    on_load(function (target)
        import("parse_testcases")
        local testcases = parse_testcases(io.readfile("tests/fixtures/testcases.txt"))
        for i, case in ipairs(testcases) do
            target:add("tests", string.format("case-%02d", i), {
                runargs = {path.join("tests/fixtures/assets", case.source), case.input},
                pass_outputs = case.output,
                trim_output = true,
                run_timeout = 1000,
            })
        end
    end)

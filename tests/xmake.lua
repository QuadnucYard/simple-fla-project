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
        local cases_path = "tests/fixtures/core/testcases.txt"
        local testcases = {}
        if os.exists(cases_path) then
            testcases = parse_testcases(io.readfile(cases_path))
        end
        for i, case in ipairs(testcases) do
            target:add("tests", string.format("case-%02d", i), {
                runargs = {path.join("tests/fixtures/core/assets", case.source), case.input},
                pass_outputs = case.output,
                trim_output = true,
                run_timeout = 1000,
            })
        end
    end)

target("test_cli")
    set_kind("binary")
    set_default(false)
    add_deps("fla-core")
    add_files("$(projectdir)/fla-project/*.cpp")
    set_rundir(os.projectdir())

    local snap_folder = "tests/fixtures/cli/snap"

    on_load(function (target)
        if not os.exists(snap_folder) then
            os.mkdir(snap_folder)
        end
        import("fixtures.cli.cases")
        for _, case in ipairs(cases()) do
            target:add("tests", case.name, case)
        end
    end)

    on_test(function (target, opt)
        local runargs = {"r", "test_cli"}
        local snap_text = ""
        if opt.input_file ~= nil then
            local input_file = "tests/fixtures/cli/assets/"..opt.input_file
            table.insert(runargs, input_file)
            snap_text = snap_text..("input_file: %s\n"):format(input_file)
        end
        snap_text = snap_text.."info:\n"
        if opt.input ~= nil then
            table.insert(runargs, opt.input)
            snap_text = snap_text..("  input: %s\n"):format(opt.input)
        end
        if opt.args ~= nil then
            table.join2(runargs, opt.args)
            snap_text = snap_text..("  args: [%s]\n"):format(table.concat(opt.args, ", "))
        end

        local outdata, errdata = "", ""
        try {
            function ()
                outdata, errdata = os.iorunv("xmake", runargs)
            end,
            catch {
                function (errors)
                    errdata = errors
                end
            },
        }

        -- local outdata, errdata = os.iorunv("xmake", runargs)
        --[[ local snap = {
            input_file = opt.input_file,
            info = { args = opt.args },
            snapshot_kind = "text",
        } ]]
        snap_text = ("---\n%s---\n----- stdout -----\n%s\n----- stderr -----\n%s\n"):format(snap_text, outdata, errdata)

        local snap_path = path.join(snap_folder, opt.name..".snap")
        if os.exists(snap_path) then
            local old_snap_text = io.readfile(snap_path)
            if old_snap_text == snap_text then
                return true
            end
        end
        io.writefile(snap_path, snap_text)
        print("snapshot changed: "..snap_path)
        print(snap_text)
        return false

        -- failed
        -- return false, errors
    end)

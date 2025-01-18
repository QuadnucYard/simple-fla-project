function main()
    return {
        {
            name = "help",
            args = { "-h" },
        },
        {
            name = "missing-path",
        },
        {
            name = "unexpected-arg",
            args = { "-a" },
        },
        --
        {
            name = "pda-erroneous-1",
            input_file = "erroneous-1.pda",
        },
        {
            name       = "pda-undeclared",
            input_file = "undeclared.pda",
        },
        {
            name       = "pda-undeclared-cv",
            input_file = "undeclared.pda",
            args       = { "-c", "-v" },
        },
        {
            name       = "pda-anbn-c",
            input_file = "anbn.pda",
            input      = "aaabb",
            args       = { "-c" },
        },
        {
            name       = "pda-anbn-cv",
            input_file = "anbn.pda",
            input      = "aaabb",
            args       = { "-c", "-v" },
        },
        {
            name       = "pda-anbn-cdv",
            input_file = "anbn.pda",
            input      = "aaabb",
            args       = { "-c", "-d", "-v" },
        },
        {
            name       = "pda-anbn",
            input_file = "anbn.pda",
            input      = "aaabb",
        },
        {
            name       = "pda-anbn-v",
            input_file = "anbn.pda",
            input      = "aaabb",
            args       = { "-v" },
        },
        {
            name       = "pda-anbn-vr",
            input_file = "anbn.pda",
            input      = "aaabb",
            args       = { "-v", "-r" },
        },
        {
            name       = "pda-anbn-lv",
            input_file = "anbn.pda",
            input      = "aaabbbb",
            args       = { "-l", "5", "-v" },
        },
        --
        {
            name = "tm-erroneous-1",
            input_file = "erroneous-1.tm",
        },
        {
            name       = "tm-erroneous-1-v",
            input_file = "erroneous-1.tm",
            args       = { "-v" },
        },
        {
            name       = "tm-erroneous-2-c",
            input_file = "erroneous-2.tm",
            args       = { "-c" },
        },
        {
            name       = "tm-erroneous-2-cv",
            input_file = "erroneous-2.tm",
            args       = { "-c", "-v" },
        },
        {
            name       = "tm-missing-final",
            input_file = "missing-final.tm",
        },
        {
            name       = "tm-missing-final-cv",
            input_file = "missing-final.tm",
            args       = { "-c", "-v" },
        },
        {
            name       = "tm-undeclared",
            input_file = "undeclared.tm",
        },
        {
            name       = "tm-undeclared-cv",
            input_file = "undeclared.tm",
            args       = { "-c", "-v" },
        },
        {
            name       = "tm-palindrome-c",
            input_file = "palindrome.tm",
            input      = "1001001",
            args       = { "-c" },
        },
        {
            name       = "tm-palindrome-cv",
            input_file = "palindrome.tm",
            input      = "1001001",
            args       = { "-c", "-v" },
        },
        {
            name       = "tm-palindrome-cdv",
            input_file = "palindrome.tm",
            input      = "1001001",
            args       = { "-c", "-d", "-v" },
        },
        {
            name       = "tm-palindrome",
            input_file = "palindrome.tm",
            input      = "1001001",
        },
        {
            name       = "tm-palindrome-v",
            input_file = "palindrome.tm",
            input      = "1001001",
            args       = { "-v" },
        },
        {
            name       = "tm-palindrome-vr",
            input_file = "palindrome.tm",
            input      = "1001001",
            args       = { "-v", "-r" },
        },
        {
            name       = "tm-palindrome-lv",
            input_file = "palindrome.tm",
            input      = "1001001",
            args       = { "-l", "10", "-v" },
        }
    }
end

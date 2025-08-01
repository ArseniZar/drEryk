#include "UNotePadCli.h"
#include "Command.h"
#include "utils.h"

UNotePadCli::UNotePadCli() {};

int UNotePadCli::run(int &argc, char **argv)
{
    if (argc < 3)
    {
        std::cerr << "Error: command not specified. Use --help\n";
        return 1;
    }

    std::string commandArg = argv[2];
    Command cmd = parseCommand(commandArg);

    if (cmd == Command::Unknown)
    {
        std::cerr << "Unknown command: " << commandArg << "\n";
        return 1;
    }

    std::string line;
    std::string input;

    while (std::getline(std::cin, line))
    {
        input += line + "\n";
    }

    switch (cmd)
    {
    case Command::Palindrome:
    {
        bool result = util::isPalindrome(input);
        std::cout << (result ? "Yes" : "No") << std::endl;
        break;
    }
    case Command::ReplaceAll:
    {
        if (argc < 5)
        {
            std::cerr << "Error: --rep command requires arguments <from> <to> [caseSensitive: T/F] [wholeWordsOnly: T/F]\n";
            return 1;
        }

        std::string from = argv[3];
        std::string to = argv[4];

        bool caseSensitive = false;
        bool wholeWordsOnly = false;

        if (argc > 5)
        {
            if (!parseTF(argv[5], caseSensitive))
            {
                std::cerr << "Error: argument '" << argv[5] << "' for caseSensitive must be T or F\n";
                return 1;
            }
        }

        if (argc > 6)
        {
            if (!parseTF(argv[6], wholeWordsOnly))
            {
                std::cerr << "Error: argument '" << argv[6] << "' for wholeWordsOnly must be T or F\n";
                return 1;
            }
        }

        std::string result = util::replaceAll(input, from, to, caseSensitive, wholeWordsOnly);
        std::cout << result << std::endl;
        break;
    }

    case Command::CountVisibleChars:
    {
        size_t count = util::countChars(input);
        std::cout << "Visible characters: " << count << std::endl;
        break;
    }

    case Command::Reverse:
    {
        std::string result = util::reverse(input);
        std::cout << result << std::endl;
        break;
    }

    case Command::SortLines:
    {
        bool ascending = true;

        if (argc > 3)
        {
            if (!parseTF(argv[3], ascending))
            {
                std::cerr << "Error: argument '" << argv[3] << "' for sorting must be T or F\n";
                return 1;
            }
        }

        std::string result = util::sortLines(input, ascending);
        std::cout << result << std::endl;
        break;
    }

    default:
        std::cerr << "Command not implemented.\n";
        return 1;
    }

    return 0;
}

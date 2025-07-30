#include "Command.h"


const std::unordered_map<std::string, std::pair<Command, std::string>> commandMap = {
    {"--cli",  {Command::Cli, "Run the program in command-line interface (CLI) mode. Use with specific commands to process input."}},
    {"--pal",  {Command::Palindrome, "Check if the given input string is a palindrome (reads the same forwards and backwards). Input is read from standard input."}},
    {"--rep",  {Command::ReplaceAll, "Replace all occurrences of a substring within the input string. Requires additional arguments: <from> <to>. Optional flags: case sensitivity and whole word matching (T/F)."}},
    {"--cnt",  {Command::CountVisibleChars, "Count the number of visible (non-whitespace) characters in the input string. Input is read from standard input."}},
    {"--rev",  {Command::Reverse, "Reverse the entire input string read from standard input and output the result."}},
    {"--sort", {Command::SortLines, "Sort lines in the input text alphabetically. Optional parameter: T for ascending (default), F for descending order."}},
    {"--help", {Command::Help, "Display this help information showing available commands and their usage."}}
};

Command parseCommand(const std::string& input)
{
    auto it = commandMap.find(input);
    if (it != commandMap.end())
        return it->second.first;
    return Command::Unknown;
}

void printAllCommands()
{
    std::cout << "Available commands:\n";
    for (const auto& [key, value] : commandMap)
    {
        std::cout << "  " << key << " — " << value.second << "\n";
    }
}

bool parseTF(const std::string& str, bool& out)
{
    if (str == "T" || str == "t") {
        out = true;
        return true;
    }
    else if (str == "F" || str == "f") {
        out = false;
        return true;
    }
    return false; 
}

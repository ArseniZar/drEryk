#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <unordered_map>
#include <iostream>

enum class Command
{
    Cli,
    Palindrome,
    ReplaceAll,
    CountVisibleChars,
    Reverse,
    SortLines,
    Help,
    Unknown
};

extern const std::unordered_map<std::string, std::pair<Command, std::string>> commandMap;

Command parseCommand(const std::string& input);
void printAllCommands();
bool parseTF(const std::string& str, bool& out);

#endif

#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace util
{
    bool isPalindrome(const std::string &text);
    std::string replaceAll(const std::string &text, const std::string &from, const std::string &to, bool caseSensitive, bool wholeWordsOnly);
    size_t countVisibleChars(const std::string &text);
    std::string reverse(const std::string &text);
    std::string sortLines(const std::string& input, bool ascending);

    std::string toLower(const std::string &str);
    bool isWholeWord(const std::string& str, size_t pos, size_t len);
    std::string adjustCaseCharByChar(const std::string& source, const std::string& replacement);
}

#endif
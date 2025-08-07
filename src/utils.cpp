#include "utils.h"
#include <algorithm>
#include <vector>

namespace util
{
    bool isPalindrome(const std::string &text)
    {
        int left = 0;
        int right = (int)text.length() - 1;

        while (left < right)
        {
            if (!std::isalnum(static_cast<unsigned char>(text[left])))
            {
                left++;
                continue;
            }

            if (!std::isalnum(static_cast<unsigned char>(text[right])))
            {
                right--;
                continue;
            }

            if (std::tolower(static_cast<unsigned char>(text[left])) != std::tolower(static_cast<unsigned char>(text[right])))
            {
                return false;
            }

            left++;
            right--;
        }

        return true;
    }

    std::string replaceAll(const std::string &text, const std::string &from, const std::string &to, bool caseSensitive, bool wholeWordsOnly)
    {

        if (from.empty())
            return text;

        std::string result = text;
        std::string search = caseSensitive ? result : toLower(result);
        std::string pattern = caseSensitive ? from : toLower(from);

        size_t pos = 0;
        while ((pos = search.find(pattern, pos)) != std::string::npos)
        {
            if (!wholeWordsOnly || isWholeWord(search, pos, pattern.length()))
            {
                std::string replacement = to;
                if (!caseSensitive)
                {
                    std::string originalWord = result.substr(pos, pattern.length());
                    replacement = adjustCaseCharByChar(originalWord, to);
                }
                result.replace(pos, pattern.length(), replacement);
                search.replace(pos, pattern.length(), std::string(replacement.length(), '*'));
                pos += replacement.length();
            }
            else
            {
                pos += 1;
            }
        }

        return result;
    }

    std::string reverse(const std::string &text)
    {
        std::string result = text;
        std::reverse(result.begin(), result.end());
        return result;
    }

    size_t countChars(const std::string &text)
    {
        return text.size();
    }

    std::string sortLines(const std::string &input, bool ascending)
    {
        std::vector<std::string> lines;
        size_t start = 0;

        while (true)
        {
            size_t pos = input.find('\n', start);
            if (pos == std::string::npos)
            {
                lines.push_back(input.substr(start));
                break;
            }
            lines.push_back(input.substr(start, pos - start));
            start = pos + 1;
        }

        std::sort(lines.begin(), lines.end(),
                  [ascending](const std::string &a, const std::string &b)
                  {
                      if (ascending)
                      {
                          return a.length() < b.length();
                      }
                      else
                      {
                          return a.length() > b.length();
                      }
                  });

        std::string result;
        for (size_t i = 0; i < lines.size(); ++i)
        {
            result += lines[i];
            if (i + 1 < lines.size())
            {
                result += '\n';
            }
        }

        return result;
    }

    std::string toLower(const std::string &str)
    {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });
        return result;
    }

    bool isWholeWord(const std::string &str, size_t pos, size_t len)
    {
        bool atStart = (pos == 0) || !std::isalnum(static_cast<unsigned char>(str[pos - 1]));
        bool atEnd = (pos + len >= str.length()) || !std::isalnum(static_cast<unsigned char>(str[pos + len]));

        return atStart && atEnd;
    }

    std::string adjustCaseCharByChar(const std::string &source, const std::string &replacement)
    {
        std::string result = replacement;
        for (size_t i = 0; i < result.size() && i < source.size(); ++i)
        {
            if (std::isupper(static_cast<unsigned char>(source[i])))
                result[i] = std::toupper(static_cast<unsigned char>(result[i]));
            else
                result[i] = std::tolower(static_cast<unsigned char>(result[i]));
        }
        return result;
    }
}
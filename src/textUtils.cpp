#include "textUtils.h"
#include <algorithm>

bool CompareLength(const wxString &s1, const wxString &s2) {
    return s1.Length() < s2.Length();
}

bool CheckTextForPalindrome(const wxString &text) {
    wxString cleanedText;
    for (wxChar c : text) {
        if (wxIsalnum(c)) cleanedText += wxTolower(c);
    }
    wxString reversedText;
    reversedText.reserve(cleanedText.Length());
    for (int i = cleanedText.Length() - 1; i >= 0; --i) {
        reversedText += cleanedText[i];
    }
    return cleanedText == reversedText;
}

wxString ProcessReplaceFooBar(const wxString &text) {
    wxString newText = text;
    newText.Replace("foo", "bar");
    return newText;
}

wxString ProcessReverseText(const wxString &text) {
    wxString reversedText;
    reversedText.reserve(text.Length());
    for (int i = text.Length() - 1; i >= 0; --i) {
        reversedText += text[i];
    }
    return reversedText;
}

wxString ProcessSortLines(const wxString &text) {
    wxString originalText = text;

    // Determine line ending character(s)
    wxString lineEnding = "\n";
    if (originalText.Contains("\r\n")) {
        lineEnding = "\r\n";
    } else if (originalText.Contains("\r") && !originalText.Contains("\n")) {
        lineEnding = "\r";
    }

    // Split text into lines
    wxArrayString linesArray = wxStringTokenize(originalText, lineEnding, wxTOKEN_RET_EMPTY_ALL);

    // Added logic to handle the last empty line if the original text did not end with a newline.
    bool originalEndsWithNewline = originalText.EndsWith("\n") || originalText.EndsWith("\r\n");
    if (!linesArray.IsEmpty() && linesArray.Last().IsEmpty() && !originalEndsWithNewline)
    {
        linesArray.RemoveAt(linesArray.GetCount() - 1);
    }

    // Sort lines by length
    std::sort(linesArray.begin(), linesArray.end(), CompareLength);

    // Reconstruct the sorted text
    wxString sortedText;
    for (size_t i = 0; i < linesArray.GetCount(); ++i) {
        sortedText += linesArray[i];
        // Add line separator between lines,
        // and at the end if the original text ended with a separator.
        if (i < linesArray.GetCount() - 1 || (i == linesArray.GetCount() - 1 && originalEndsWithNewline)) {
            sortedText += lineEnding;
        }
    }
    return sortedText;
}
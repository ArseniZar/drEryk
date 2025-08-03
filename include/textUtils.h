#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

#include <string>
#include <algorithm>

#include <wx/event.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/tokenzr.h>

void OnCheckPalindrome(wxCommandEvent &evt);
void OnReplaceFooBar(wxCommandEvent &evt);
void OnReverseText(wxCommandEvent &evt);
void OnSortLines(wxCommandEvent &evt);

bool CheckTextForPalindrome(const wxString &text);
wxString ProcessReplaceFooBar(const wxString &text);
wxString ProcessReverseText(const wxString &text);
wxString ProcessSortLines(const wxString &text);
bool CompareLength(const wxString &a, const wxString &b);

#endif
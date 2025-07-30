/**
 * @file main.h
 * @author Usitha Indeewara (https://github.com/usithadev)
 * @brief Declarations of main App class and main window.
 * @version 0.1
 * @date 2022-09-24
 *
 * @copyright Copyright (c) 2022 Usitha Indeewara. All Rights Reserved.
 */

#ifndef MAIN_H
#define MAIN_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/stc/stc.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/clipbrd.h>
#include <wx/tokenzr.h>
#include <wx/cmdline.h>
#include <wx/textfile.h> 
#include <wx/arrstr.h>  
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/datetime.h> 
#include <iostream>
#include <algorithm>
#include <vector>
#include "about.h"

/**
 * @brief The main App class
 */
class App : public wxApp
{
public:
    /**
     * @brief The application entry point
     *
     * @return true
     */
    virtual bool OnInit();
};

wxDECLARE_APP(App);

enum
{
    ID_MyMessage = wxID_HIGHEST + 1,
    ID_SaveAsCustom = wxID_HIGHEST + 2,
    ID_CopyToClipboard = wxID_HIGHEST + 3,
    ID_PasteFromClipboard,
    ID_CheckPalindrome,
    ID_ReplaceFooBar,
    In_ReverseText,
    ID_SortLines
};

/**
 * @brief The main window object.
 *
 */
class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title);

private:
    // Объявляем члены класса, но НЕ инициализируем их здесь.
    // Инициализация будет происходить в конструкторе MainFrame в main.cpp.
    wxPanel *panel;
    wxStyledTextCtrl *editor;

    // Event methods for menu items
    // Используем wxCommandEvent напрямую вместо 'cmd'
    void OnExit(wxCommandEvent &evt);
    void OnAbout(wxCommandEvent &evt);
    void OnSaveAs(wxCommandEvent &WXUNUSED(evt));
    void OnUndo(wxCommandEvent &evt);
    void OnRedo(wxCommandEvent &evt);
    void OnCopy(wxCommandEvent &evt);
    void OnCut(wxCommandEvent &evt);
    void OnPaste(wxCommandEvent &evt);
    void OnMyMessage(wxCommandEvent &evt);
    void OnSaveAsCustom(wxCommandEvent &evt);
    void OnOpenFile(wxCommandEvent &evt);
    void OnCopyToClipboard(wxCommandEvent &evt);
    void OnPasteFromClipboard(wxCommandEvent &evt);
    void OnEditorChanged(const wxStyledTextEvent &evt);
    void OnCheckPalindrome(wxCommandEvent &evt);
    void OnReplaceFooBar(wxCommandEvent &evt);
    void OnReverseText(wxCommandEvent &evt);
    void OnSortLines(wxCommandEvent &evt);
};

// Прототипы функций обработки текста (глобальные)
bool CheckTextForPalindrome(const wxString &text);
wxString ProcessReplaceFooBar(const wxString &text);
wxString ProcessReverseText(const wxString &text);
wxString ProcessSortLines(const wxString &text);
bool CompareLength(const wxString &a, const wxString &b);

#endif // MAIN_H
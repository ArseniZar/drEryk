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
#include <wx/filename.h>   // Добавлено для wxFileName
#include <wx/stdpaths.h>   // Добавлено для wxStandardPaths
#include <wx/clipbrd.h>
#include "about.h"

/**
 * @brief The main App class
 */
class App : public wxApp {
public:
    /**
     * @brief The application entry point
     *
     * @return true
     */
    virtual bool OnInit();
};

wxDECLARE_APP(App);

enum {
    ID_MyMessage = wxID_HIGHEST + 1,
    ID_SaveAsCustom = wxID_HIGHEST + 2,
    ID_CopyToClipboard = wxID_HIGHEST + 3,
    ID_PasteFromClipboard 
};

/**
 * @brief The main window object.
 *
 */
class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    using cmd = wxCommandEvent;
    wxPanel *panel = new wxPanel(this, wxID_ANY); // The main panel
    wxStyledTextCtrl *editor = new wxStyledTextCtrl(panel, wxID_ANY, wxDefaultPosition); // The text editor

    // Event methods for menu items
    void OnExit(cmd& evt);
    void OnAbout(cmd& evt);
    void OnSaveAs(cmd& WXUNUSED(evt));
    void OnUndo(cmd& evt);
    void OnRedo(cmd& evt);
    void OnCopy(cmd& evt);
    void OnCut(cmd& evt);
    void OnPaste(cmd& evt);
    void OnMyMessage(cmd& evt);
    void OnSaveAsCustom(cmd& evt);
    void OnOpenFile(cmd& evt); 
    void OnCopyToClipboard (cmd& evt);
    void OnPasteFromClipboard (cmd& evt);
    void OnEditorChanged(const wxStyledTextEvent& evt);
};

#endif // MAIN_H
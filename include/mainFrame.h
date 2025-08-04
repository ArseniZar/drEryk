#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include "about.h"
#include "textUtils.h"

#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/textfile.h>
#include <wx/clipbrd.h>
#include <wx/filename.h>
#include <wx/artprov.h>
#include <wx/image.h> 

enum
{
    ID_MyMessage = wxID_HIGHEST + 1,
    ID_SaveAsCustom,
    ID_CopyToClipboard,
    ID_PasteFromClipboard,
    ID_CheckPalindrome,
    ID_ReplaceFooBar,
    ID_ReverseText,
    ID_SortLines
};

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title);

private:
    wxPanel *panel;
    wxStyledTextCtrl *editor;

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

#endif // MAIN_FRAME_H
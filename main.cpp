/**
 * @file main.cpp
 * @author Usitha Indeewara (https://github.com/usithadev)
 * @brief The forward declarations of classes and functions in main.h file.
 * @version 0.1
 * @date 2022-09-24
 *
 * @todo License the source files, Add license notice to tehe about message, Make Save, New, Open commands functional
 *
 * @copyright Copyright (c) 2022 Usitha Indeewara. All Rights Reserved.
 *
 */

#include "main.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrame *frm = new MainFrame("UNotePad");
    frm->Show(true);

    return true;
}

MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title)
{
#if WIN32
    this->SetIcon(wxICON(appicon)); // Set the application icon
#endif                              // WIN32

    //
    // Widgets
    //

    wxBoxSizer *main = new wxBoxSizer(wxVERTICAL); // The main sizer contains all the widgets

    wxBoxSizer *panelsizer = new wxBoxSizer(wxVERTICAL); // The sizer of the panel

    panelsizer->Add(editor, 1, wxALL | wxEXPAND, 0);
    editor->SetMinSize(wxSize(300, 300));

    panel->SetSizerAndFit(panelsizer);

    main->Add(panel, 1, wxALL | wxEXPAND, 0);

    SetSizerAndFit(main);

    //
    // Menu bar and status bar
    //
    wxMenu *file = new wxMenu;
    file->Append(wxID_NEW);
    file->Append(wxID_OPEN);
    file->AppendSeparator();
    file->Append(wxID_SAVE);
    file->Append(wxID_SAVEAS);
    file->AppendSeparator();
    file->Append(wxID_EXIT);

    wxMenu *help = new wxMenu;
    help->Append(wxID_ABOUT);

    wxMenu *edit = new wxMenu;
    edit->Append(wxID_UNDO);
    edit->Append(wxID_REDO);
    edit->AppendSeparator();
    edit->Append(wxID_COPY);
    edit->Append(wxID_CUT);
    edit->Append(wxID_PASTE);

    wxMenu *info = new wxMenu;
    info->Append(wxID_INFO);

    wxMenuBar *menubar = new wxMenuBar;
    menubar->Append(file, "&File");
    menubar->Append(edit, "&Edit");
    menubar->Append(help, "&Help");
    menubar->Append(info, "&Info");

    SetMenuBar(menubar);

    CreateStatusBar();
    SetStatusText("Welcome to the lightweight text editor: UNotePad");

    //
    // Event bindings
    //
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    // Bind(wxEVT_MENU, &MainFrame::OnSaveAs, this, wxID_SAVEAS);
    Bind(wxEVT_MENU, &MainFrame::OnSaveAsCustom, this, wxID_SAVEAS);
    Bind(wxEVT_MENU, &MainFrame::OnUndo, this, wxID_UNDO);
    Bind(wxEVT_MENU, &MainFrame::OnRedo, this, wxID_REDO);
    Bind(wxEVT_MENU, &MainFrame::OnCut, this, wxID_CUT);
    Bind(wxEVT_MENU, &MainFrame::OnCopy, this, wxID_COPY);
    Bind(wxEVT_MENU, &MainFrame::OnPaste, this, wxID_PASTE);
    Bind(wxEVT_MENU, &MainFrame::onInfo, this, wxID_INFO);
}

void MainFrame::OnExit(cmd &evt)
{
    Close(true);
}

void MainFrame::OnAbout(cmd &evt)
{
    AboutFrame *about = new AboutFrame(this, "About");
    about->Show(true);
}

void MainFrame::OnSaveAs(cmd &WXUNUSED(evt))
{

    // The text of the editor
    wxString str = this->editor->GetText();

    // Getting the path
    wxFileDialog saveFileAs(this, "Save as", "", "", "Plain text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileAs.ShowModal() == wxID_CANCEL)
    {
        return;
    }
    auto path = saveFileAs.GetPath();

    // Write the str to the file
    wxFileOutputStream output(path);
    if (!output.IsOk())
    {
        wxMessageBox("Unable to save the file");
        return;
    }

    wxFile *file = new wxFile(path, wxFile::write);
    if (file->IsOpened())
    {
        file->Write(str);
    }
    file->Close();
}

void SaveStringToFile(const fs::path &filepath, const string &content)
{
    ofstream file;
    file.exceptions(ofstream::failbit | ofstream::badbit);

    file.open(filepath, ios::binary);
    file << content;
    file.close();
}

void MainFrame::OnSaveAsCustom(cmd &evt)
{

    wxString str = this->editor->GetText();
    string text = string(str.mb_str(wxConvUTF8));

    wxFileDialog saveFileAs(this, "Save as", "", "", "Plain text files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileAs.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    wxString p = saveFileAs.GetPath();
    fs::path path = fs::u8path(string(p.mb_str(wxConvUTF8)));

    try
    {
        if (fs::exists(path))
        {
            if (fs::is_directory(path))
            {
                wxMessageBox("Write error: the specified path is a directory.", "Error", wxOK | wxICON_ERROR);
                return;
            }
        }
        else
        {
            fs::path parent = path.parent_path();
            if (!fs::exists(parent) || !fs::is_directory(parent))
            {
                wxMessageBox("Write error: parent directory does not exist.", "Error", wxOK | wxICON_ERROR);
                return;
            }
        }

        SaveStringToFile(path, text);
        wxMessageBox("File saved successfully.", "Success", wxOK | wxICON_INFORMATION);
    }
    catch (const ofstream::failure &)
    {
        wxMessageBox("Error writing file: cannot open or write to file. Check permissions and disk space.", "Error", wxOK | wxICON_ERROR);
    }
    catch (const fs::filesystem_error &)
    {
        wxMessageBox("Filesystem error. Check the path and permissions.", "Error", wxOK | wxICON_ERROR);
    }
    catch (const exception &)
    {
        wxMessageBox("An error occurred while saving the file.", "Error", wxOK | wxICON_ERROR);
    }
    catch (...)
    {
        wxMessageBox("Unknown error occurred while saving the file.", "Error", wxOK | wxICON_ERROR);
    }
}

void MainFrame::OnUndo(cmd &evt)
{
    this->editor->Undo();
}

void MainFrame::OnRedo(cmd &evt)
{
    this->editor->Redo();
}

void MainFrame::OnCut(cmd &evt)
{
    this->editor->Cut();
}

void MainFrame::OnCopy(cmd &evt)
{
    this->editor->Copy();
}

void MainFrame::OnPaste(cmd &evt)
{
    this->editor->Paste();
}

void MainFrame::onInfo(cmd &evt)
{
    wxMessageBox("Task 1",
                 "Info", wxOK | wxICON_INFORMATION);
}

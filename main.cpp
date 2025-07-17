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
    wxBoxSizer *main = new wxBoxSizer(wxHORIZONTAL);

    wxPanel *editorPanel = new wxPanel(this, wxID_ANY);
    wxPanel *buttonsPanel = new wxPanel(this, wxID_ANY);

    editor = new wxStyledTextCtrl(editorPanel, wxID_ANY, wxDefaultPosition);
    wxBoxSizer *panelsizer = new wxBoxSizer(wxVERTICAL);
    panelsizer->Add(editor, 1, wxALL | wxEXPAND, 0);
    editor->SetMinSize(wxSize(600, 600));
    editorPanel->SetSizerAndFit(panelsizer);

    wxBitmap bmpCopy(std::string(ICONS_PATH) + "icons8-copy-48.png", wxBITMAP_TYPE_PNG);
    wxImage imgCopy = bmpCopy.ConvertToImage().Scale(20, 20, wxIMAGE_QUALITY_HIGH);
    wxBitmap bmpScalerCopy(imgCopy);
    wxBitmapButton *bmpBtn1 = new wxBitmapButton(buttonsPanel, ID_COPY_ALL_FIELDS, bmpScalerCopy, wxDefaultPosition, wxSize(40, 40));

    wxBitmap bmpPaste(std::string(ICONS_PATH) + "icons8-paste-48.png", wxBITMAP_TYPE_PNG);
    wxImage imgPaste = bmpPaste.ConvertToImage().Scale(20, 20, wxIMAGE_QUALITY_HIGH);
    wxBitmap imgScalerPaste(imgPaste);
    wxBitmapButton *bmpBtn2 = new wxBitmapButton(buttonsPanel, ID_PASTE_ALL_FIELDS, imgScalerPaste, wxDefaultPosition, wxSize(40, 40));

    wxBoxSizer *panelButtonSizer = new wxBoxSizer(wxVERTICAL);
    panelButtonSizer->Add(bmpBtn1, 0, wxALL, 5);
    panelButtonSizer->Add(bmpBtn2, 0, wxALL, 5);
    buttonsPanel->SetSizerAndFit(panelButtonSizer);

    main->Add(editorPanel, 1, wxALL | wxEXPAND, 0);
    main->Add(buttonsPanel, 0, wxALL | wxEXPAND, 0);

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

    wxMenu *tools = new wxMenu;
    tools->Append(ID_TOOLS_CHECK_PALINDROME, "Check Palindrome");
    tools->Append(ID_TOOLS_REPLACE_FOO_BAR, "Replace foo on bar");

    wxMenuBar *menubar = new wxMenuBar;
    menubar->Append(file, "&File");
    menubar->Append(edit, "&Edit");
    menubar->Append(help, "&Help");
    menubar->Append(info, "&Info");
    menubar->Append(tools, "&Tools");

    SetMenuBar(menubar);

    CreateStatusBar();
    SetStatusText("Welcome to the lightweight text editor: UNotePad");

    //
    // Event bindings
    //
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnSaveAs, this, wxID_SAVEAS);
    Bind(wxEVT_MENU, &MainFrame::OnSaveAsCustom, this, wxID_SAVE);
    Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MainFrame::OnUndo, this, wxID_UNDO);
    Bind(wxEVT_MENU, &MainFrame::OnRedo, this, wxID_REDO);
    Bind(wxEVT_MENU, &MainFrame::OnCut, this, wxID_CUT);
    Bind(wxEVT_MENU, &MainFrame::OnCopy, this, wxID_COPY);
    Bind(wxEVT_MENU, &MainFrame::OnPaste, this, wxID_PASTE);
    Bind(wxEVT_MENU, &MainFrame::onInfo, this, wxID_INFO);

    Bind(wxEVT_MENU, &MainFrame::onCheckPalindrome, this, ID_TOOLS_CHECK_PALINDROME);
    Bind(wxEVT_MENU, &MainFrame::onReplaceFooOnBar, this, ID_TOOLS_REPLACE_FOO_BAR);

    Bind(wxEVT_BUTTON, &MainFrame::OnCopyCustom, this, ID_COPY_ALL_FIELDS);
    Bind(wxEVT_BUTTON, &MainFrame::OnPaste, this, ID_PASTE_ALL_FIELDS);
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

std::string ReadFileToString(const fs::path &path)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path, std::ios::binary);

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

void MainFrame::OnOpen(cmd &evt)
{
    wxFileDialog openFileDialog(this, "Open file", "", "", "Plain text files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxString p = openFileDialog.GetPath();
    fs::path path = fs::u8path(std::string(p.mb_str(wxConvUTF8)));

    try
    {
        if (!fs::exists(path))
        {
            wxMessageBox("Read error: File does not exist.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        if (fs::is_directory(path))
        {
            wxMessageBox("Read error: The specified path is a directory, not a file.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        std::string content = ReadFileToString(path);
        wxString wxContent = wxString::FromUTF8(content.c_str());
        this->editor->SetText(wxContent);
    }
    catch (const std::ifstream::failure &)
    {
        wxMessageBox("File read error: cannot open or read the file.", "Error", wxOK | wxICON_ERROR);
    }
    catch (const fs::filesystem_error &e)
    {
        wxMessageBox("Filesystem error: " + wxString(e.what()), "Error", wxOK | wxICON_ERROR);
    }
    catch (const std::exception &e)
    {
        wxMessageBox("Error: " + wxString(e.what()), "Error", wxOK | wxICON_ERROR);
    }
    catch (...)
    {
        wxMessageBox("Unknown error occurred while reading the file.", "Error", wxOK | wxICON_ERROR);
    }
}

void SaveStringToFile(const fs::path &filepath, const std::string &content)
{
    std::ofstream file;
    file.exceptions(std::ofstream::failbit | std::ofstream::badbit);

    file.open(filepath, std::ios::binary);
    file << content;
    file.close();
}

void MainFrame::OnSaveAsCustom(cmd &evt)
{
    wxString str = this->editor->GetText();
    std::string text = std::string(str.mb_str(wxConvUTF8));

    wxFileDialog saveFileAs(this, "Save as", "", "", "Plain text files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileAs.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    wxString p = saveFileAs.GetPath();
    fs::path path = fs::u8path(std::string(p.mb_str(wxConvUTF8)));

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
    catch (const std::ofstream::failure &)
    {
        wxMessageBox("Error writing file: cannot open or write to file. Check permissions and disk space.", "Error", wxOK | wxICON_ERROR);
    }
    catch (const fs::filesystem_error &)
    {
        wxMessageBox("Filesystem error. Check the path and permissions.", "Error", wxOK | wxICON_ERROR);
    }
    catch (const std::exception &)
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

void MainFrame::OnCopyCustom(cmd &evt)
{
    wxString text = this->editor->GetText();
    if (wxTheClipboard->Open())
    {
        wxTheClipboard->SetData(new wxTextDataObject(text));
        wxTheClipboard->Close();
    }
    else
    {
        wxMessageBox("Failed to open the clipboard.", "Error", wxOK | wxICON_ERROR);
    }
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

void MainFrame::onCheckPalindrome(cmd &evt)
{
    wxString str = this->editor->GetText();
    if (IsPalindrome(str))
    {
        wxMessageBox("Text is Palindrome",
                     "Info", wxOK | wxICON_INFORMATION);
        return;
    }

    wxMessageBox("Text is not  Palindrome",
                 "Info", wxOK | wxICON_INFORMATION);
}

void MainFrame::onReplaceFooOnBar(cmd &evt)
{
    wxString str = this->editor->GetText();
    str.Replace("foo", "bar", true);
    this->editor->SetText(str);
}

bool IsPalindrome(const wxString &str)
{
    wxString s = str;
    s.MakeLower();
    s.Replace(" ", "");

    int len = s.Length();
    for (int i = 0; i < len / 2; ++i)
    {
        if (s[i] != s[len - 1 - i])
            return false;
    }
    return true;
}
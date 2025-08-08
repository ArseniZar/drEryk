/**
 * @file UNotePadWindow.cpp
 * @author Usitha Indeewara (https://github.com/usithadev)
 * @brief The forward declarations of classes and functions in UNotePadWindow.h file.
 * @version 0.1
 * @date 2022-09-24
 *
 * @todo License the source files, Add license notice to tehe about message, Make Save, New, Open commands functional
 *
 * @copyright Copyright (c) 2022 Usitha Indeewara. All Rights Reserved.
 *
 */

#include "UNotePadWindow.h"
bool UNotePadWindow::OnInit()
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
    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *leftPanelSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *rightPanelSizer = new wxBoxSizer(wxVERTICAL);

    wxPanel *replacePanel = CreateReplacePanel();
    wxPanel *editorPanel = CreateEditorPanel();
    wxPanel *buttonsPanel = CreateButtonsPanel();

    leftPanelSizer->Add(replacePanel, 0, wxALL | wxEXPAND, 5);
    leftPanelSizer->Add(editorPanel, 1, wxALL | wxEXPAND, 5);

    rightPanelSizer->Add(buttonsPanel, 0, wxALL | wxEXPAND, 5);

    mainSizer->Add(leftPanelSizer, 1, wxEXPAND);
    mainSizer->Add(rightPanelSizer, 0, wxEXPAND);

    SetSizerAndFit(mainSizer);

    CreateMenuBar();
    CreateStatusBar();
    BindEvents();
}

wxPanel *MainFrame::CreateEditorPanel()
{
    wxPanel *editorPanel = new wxPanel(this, wxID_ANY);
    wxBoxSizer *editorSizer = new wxBoxSizer(wxVERTICAL);

    editor = new wxStyledTextCtrl(editorPanel, wxID_ANY, wxDefaultPosition);
    editor->SetMinSize(wxSize(600, 600));

    editorSizer->Add(editor, 1, wxALL | wxEXPAND, 0);
    editorPanel->SetSizerAndFit(editorSizer);

    return editorPanel;
}

wxPanel *MainFrame::CreateButtonsPanel()
{
    wxPanel *buttonsPanel = new wxPanel(this, wxID_ANY);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);

    buttonSizer->Add(CreateBitmapButton(buttonsPanel, ID_COPY_ALL_FIELDS, "icons8-copy-48.png"), 0, wxALL, 5);
    buttonSizer->Add(CreateBitmapButton(buttonsPanel, ID_PASTE_ALL_FIELDS, "icons8-paste-48.png"), 0, wxALL, 5);

    buttonSizer->Add(CreateBitmapButton(buttonsPanel, ID_TOOLS_REPLACE_FOO_BAR, "icons8-move-up-row-50.png"), 0, wxALL, 5);
    buttonSizer->Add(CreateBitmapButton(buttonsPanel, ID_TOOLS_REVERS_TEXT, "icons8-reverse-50.png"), 0, wxALL, 5);
    buttonSizer->Add(CreateBitmapButton(buttonsPanel, ID_TOOLS_SORT_LINES_BY_TEXT, "icons8-sort-50.png"), 0, wxALL, 5);
    buttonSizer->Add(CreateTextButton(buttonsPanel, ID_TOOLS_CHECK_PALINDROME, "isPal"), 0, wxALL, 5);

    buttonsPanel->SetSizerAndFit(buttonSizer);

    return buttonsPanel;
}

wxPanel *MainFrame::CreateReplacePanel()
{
    wxPanel *panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    hbox->Add(new wxStaticText(panel, wxID_ANY, "Search:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    searchCtrl = new wxTextCtrl(panel, wxID_ANY);
    hbox->Add(searchCtrl, 1, wxRIGHT, 10);

    hbox->Add(new wxStaticText(panel, wxID_ANY, "Replace:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    replaceCtrl = new wxTextCtrl(panel, wxID_ANY);
    hbox->Add(replaceCtrl, 1, wxRIGHT, 10);

    useRegexCheckBox = new wxCheckBox(panel, wxID_ANY, "Use regex");
    hbox->Add(useRegexCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

    wxButton *goButton = new wxButton(panel, ID_REPLACE_GO_BUTTON, "Go");
    hbox->Add(goButton, 0);

    panel->SetSizerAndFit(hbox);
    return panel;
}

wxBitmapButton *MainFrame::CreateBitmapButton(wxWindow *parent, int id, const std::string &filename)
{
    wxBitmap bmp(std::string(ICONS_PATH) + filename, wxBITMAP_TYPE_PNG);
    wxImage img = bmp.ConvertToImage().Scale(20, 20, wxIMAGE_QUALITY_HIGH);
    wxBitmap scaledBmp(img);

    return new wxBitmapButton(parent, id, scaledBmp, wxDefaultPosition, wxSize(40, 40));
}

wxButton *MainFrame::CreateTextButton(wxWindow *parent, int id, const wxString &label)
{
    return new wxButton(parent, id, label, wxDefaultPosition, wxSize(40, 40));
}

void MainFrame::CreateMenuBar()
{
    wxMenu *file = new wxMenu;
    file->Append(wxID_NEW);
    file->Append(wxID_OPEN);
    file->AppendSeparator();
    file->Append(wxID_SAVE);
    file->Append(wxID_SAVEAS);
    file->AppendSeparator();
    file->Append(wxID_EXIT);

    wxMenu *edit = new wxMenu;
    edit->Append(wxID_UNDO);
    edit->Append(wxID_REDO);
    edit->AppendSeparator();
    edit->Append(wxID_COPY);
    edit->Append(wxID_CUT);
    edit->Append(wxID_PASTE);

    wxMenu *help = new wxMenu;
    help->Append(wxID_ABOUT);

    wxMenu *info = new wxMenu;
    info->Append(wxID_INFO);

    wxMenu *tools = new wxMenu;
    tools->Append(ID_TOOLS_CHECK_PALINDROME, "Check Palindrome");
    tools->Append(ID_TOOLS_REPLACE_FOO_BAR, "Replace foo on bar");
    tools->Append(ID_TOOLS_REVERS_TEXT, "Reverse Text");
    tools->Append(ID_TOOLS_SORT_LINES_BY_TEXT, "Sort Lines by Length");

    wxMenuBar *menubar = new wxMenuBar;
    menubar->Append(file, "&File");
    menubar->Append(edit, "&Edit");
    menubar->Append(help, "&Help");
    menubar->Append(info, "&Info");
    menubar->Append(tools, "&Tools");

    SetMenuBar(menubar);
}

void MainFrame::BindEvents()
{
    // File/Menu events
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MainFrame::OnSaveAs, this, wxID_SAVEAS);
    Bind(wxEVT_MENU, &MainFrame::OnSaveAsCustom, this, wxID_SAVE);

    // Edit/Menu events
    Bind(wxEVT_MENU, &MainFrame::OnUndo, this, wxID_UNDO);
    Bind(wxEVT_MENU, &MainFrame::OnRedo, this, wxID_REDO);
    Bind(wxEVT_MENU, &MainFrame::OnCut, this, wxID_CUT);
    Bind(wxEVT_MENU, &MainFrame::OnCopy, this, wxID_COPY);
    Bind(wxEVT_MENU, &MainFrame::OnPaste, this, wxID_PASTE);

    // Help/Info
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnInfo, this, wxID_INFO);

    // Copy/Pass
    Bind(wxEVT_BUTTON, &MainFrame::OnCopyCustom, this, ID_COPY_ALL_FIELDS);
    Bind(wxEVT_BUTTON, &MainFrame::OnPaste, this, ID_PASTE_ALL_FIELDS);

    // Tools
    Bind(wxEVT_MENU, &MainFrame::OnCheckPalindrome, this, ID_TOOLS_CHECK_PALINDROME);
    Bind(wxEVT_MENU, &MainFrame::OnReplaceFooOnBar, this, ID_TOOLS_REPLACE_FOO_BAR);
    Bind(wxEVT_MENU, &MainFrame::OnReverseText, this, ID_TOOLS_REVERS_TEXT);
    Bind(wxEVT_MENU, &MainFrame::OnSortLinesByLength, this, ID_TOOLS_SORT_LINES_BY_TEXT);
    Bind(wxEVT_BUTTON, &MainFrame::OnCheckPalindrome, this, ID_TOOLS_CHECK_PALINDROME);
    Bind(wxEVT_BUTTON, &MainFrame::OnReplaceFooOnBar, this, ID_TOOLS_REPLACE_FOO_BAR);
    Bind(wxEVT_BUTTON, &MainFrame::OnReverseText, this, ID_TOOLS_REVERS_TEXT);
    Bind(wxEVT_BUTTON, &MainFrame::OnSortLinesByLength, this, ID_TOOLS_SORT_LINES_BY_TEXT);

    // Replace
    Bind(wxEVT_BUTTON, &MainFrame::OnReplaceAll, this, ID_REPLACE_GO_BUTTON);

    Bind(wxEVT_STC_CHANGE, &MainFrame::OnTextChanged, this, editor->GetId());
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

std::string ReadFileToString(const std::filesystem::path &path)
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
    std::filesystem::path path = std::filesystem::u8path(std::string(p.mb_str(wxConvUTF8)));

    try
    {
        if (!std::filesystem::exists(path))
        {
            wxMessageBox("Read error: File does not exist.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        if (std::filesystem::is_directory(path))
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
    catch (const std::filesystem::filesystem_error &e)
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

void SaveStringToFile(const std::filesystem::path &filepath, const std::string &content)
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
    std::filesystem::path path = std::filesystem::u8path(std::string(p.mb_str(wxConvUTF8)));

    try
    {
        if (std::filesystem::exists(path))
        {
            if (std::filesystem::is_directory(path))
            {
                wxMessageBox("Write error: the specified path is a directory.", "Error", wxOK | wxICON_ERROR);
                return;
            }
        }
        else
        {
            std::filesystem::path parent = path.parent_path();
            if (!std::filesystem::exists(parent) || !std::filesystem::is_directory(parent))
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
    catch (const std::filesystem::filesystem_error &)
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

void MainFrame::OnInfo(cmd &evt)
{
    wxMessageBox("Task 1",
                 "Info", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnCheckPalindrome(cmd &evt)
{
    wxString str = this->editor->GetText();
    wxString wxText = this->editor->GetText();
    std::string stdText = std::string(wxText.mb_str(wxConvUTF8));
    if (util::isPalindrome(stdText))
    {
        wxMessageBox("Text is Palindrome",
                     "Info", wxOK | wxICON_INFORMATION);
        return;
    }

    wxMessageBox("Text is not  Palindrome",
                 "Info", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnReplaceFooOnBar(cmd &evt)
{
    wxString wxText = this->editor->GetText();
    std::string stdText = std::string(wxText.mb_str(wxConvUTF8));
    std::string replaceText = util::replaceAll(stdText, "foo", "bar", true, false);
    this->editor->SetText(replaceText);
}

void MainFrame::OnTextChanged(cmd &evt)
{
    wxString wxText = this->editor->GetText();
    std::string stdText = std::string(wxText.mb_str(wxConvUTF8));
    size_t length = util::countChars(stdText);
    wxString message = wxString::Format("Characters: %zu", length);
    SetStatusText(message);
}

void MainFrame ::OnReverseText(cmd &evt)
{
    wxString wxText = this->editor->GetText();
    std::string stdText = std::string(wxText.mb_str(wxConvUTF8));
    std::string reverseText = util::reverse(stdText);
    this->editor->SetText(reverseText);
}

void MainFrame::OnSortLinesByLength(cmd &evt)
{
    wxString wxText = this->editor->GetText();
    std::string stdText = std::string(wxText.mb_str(wxConvUTF8));
    std::string sortedText = util::sortLines(stdText, true);
    wxString wxSortedText(sortedText);

    this->editor->SetText(wxSortedText);
}

void MainFrame::OnReplaceAll(wxCommandEvent &evt)
{
    wxString wxTextEditor = this->editor->GetText();
    wxString wxTextFrom = this->searchCtrl->GetValue();
    wxString wxTextTo = this->replaceCtrl->GetValue();
    bool useRegex = useRegexCheckBox->IsChecked();

    std::string textEditor = std::string(wxTextEditor.mb_str(wxConvUTF8));
    std::string textFrom = std::string(wxTextFrom.mb_str(wxConvUTF8));
    std::string textTo = std::string(wxTextTo.mb_str(wxConvUTF8));

    std::string resultText;

    if (useRegex)
    {
        try
        {
            resultText = util::regexReplaceAll(textEditor, textFrom, textTo, true);
            this->editor->SetText(resultText);
        }
        catch (const std::exception &e)
        {
            wxMessageBox("Invalid regular expression. Please check your pattern and try again.",
                         "Regex Error", wxOK | wxICON_ERROR);
        }
    }
    else
    {
        resultText = util::replaceAll(textEditor, textFrom, textTo, true, false);
        this->editor->SetText(resultText);
    }
}

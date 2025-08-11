#include "mainFrame.h"

MainFrame::MainFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxPoint(50, 50), wxSize(800, 600))
{
    panel = new wxPanel(this, wxID_ANY);
    editor = new wxStyledTextCtrl(panel, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxHSCROLL | wxVSCROLL);

#if WIN32
    this->SetIcon(wxICON(appicon));
#endif

    // Widgets
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *panelsizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer *replaceSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText *searchLabel = new wxStaticText(panel, wxID_ANY, "Search:");
    searchCtrl = new wxTextCtrl(panel, ID_SearchEdit, "", wxDefaultPosition, wxSize(100, -1));
    wxStaticText *replaceLabel = new wxStaticText(panel, wxID_ANY, "Replace:");
    replaceCtrl = new wxTextCtrl(panel, ID_ReplaceEdit, "", wxDefaultPosition, wxSize(100, -1));
    useRegexCheckBox = new wxCheckBox(panel, ID_UseRegexCheckBox, "Use Regex");
    wxButton *goButton = new wxButton(panel, ID_GoButton, "Go");


    replaceSizer->Add(searchLabel, 0, wxALL | wxALIGN_LEFT, 2);
    replaceSizer->Add(searchCtrl, 0, wxALL | wxALIGN_LEFT, 2);
    replaceSizer->Add(replaceLabel, 0, wxALL | wxALIGN_LEFT, 2);
    replaceSizer->Add(replaceCtrl, 0, wxALL | wxALIGN_LEFT, 2);
    replaceSizer->Add(useRegexCheckBox, 0, wxALL | wxALIGN_LEFT, 2);
    replaceSizer->Add(goButton, 0, wxALL | wxALIGN_LEFT, 2);

    wxBoxSizer *verticalButtonSizer = new wxBoxSizer(wxVERTICAL);

    wxBitmap replaceBmp(wxImage("UNotePad.app/Contents/Resources/icon/icons8-move-up-row-50.png").Rescale(32, 32));
    wxBitmap reverseBmp(wxImage("UNotePad.app/Contents/Resources/icon/icons8-reverse-50.png").Rescale(32, 32));
    wxBitmap sortLinesBmp(wxImage("UNotePad.app/Contents/Resources/icon/icons8-sort-50.png").Rescale(32, 32));
    wxButton *palindromeButton = new wxButton(panel, ID_CheckPalindrome, "IsPal");
    wxBitmapButton *replaceButton = new wxBitmapButton(panel, ID_ReplaceFooBar, replaceBmp, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
    wxBitmapButton *reverseButton = new wxBitmapButton(panel, ID_ReverseText, reverseBmp, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
    wxBitmapButton *sortLinesButton = new wxBitmapButton(panel, ID_SortLines, sortLinesBmp, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);

    verticalButtonSizer->Add(replaceSizer, 0, wxALL, 5);
    verticalButtonSizer->Add(palindromeButton, 0, wxALL | wxALIGN_RIGHT, 5);
    verticalButtonSizer->Add(replaceButton, 0, wxALL | wxALIGN_RIGHT, 5);
    verticalButtonSizer->Add(reverseButton, 0, wxALL | wxALIGN_RIGHT, 5);
    verticalButtonSizer->Add(sortLinesButton, 0, wxALL | wxALIGN_RIGHT, 5);

    panelsizer->Add(editor, 1, wxEXPAND | wxALL, 0);
    panelsizer->Add(verticalButtonSizer, 0, wxALL | wxALIGN_RIGHT, 5);
    editor->SetMinSize(wxSize(300, 300));

    panel->SetSizerAndFit(panelsizer);

    mainSizer->Add(panel, 1, wxALL | wxEXPAND, 0);
    SetSizerAndFit(mainSizer);

    // Create menu
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(wxID_NEW);
    fileMenu->Append(wxID_OPEN);
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_SAVE);
    fileMenu->Append(wxID_SAVEAS);
    fileMenu->Append(ID_SaveAsCustom, "Save as custom \tCtrl+Shift+S", "Save as custom pro");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT);

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT);
    helpMenu->Append(ID_MyMessage, "Message \tCtrl+M", "Show message");

    wxMenu *editMenu = new wxMenu;
    editMenu->Append(wxID_UNDO);
    editMenu->Append(wxID_REDO);
    editMenu->AppendSeparator();
    editMenu->Append(wxID_COPY);
    editMenu->Append(wxID_CUT);
    editMenu->Append(wxID_PASTE);
    editMenu->AppendSeparator();
    editMenu->Append(ID_CopyToClipboard, "Copy To Clipboard\tCtrl+Alt+C", "Copy all text from editor to clipboard");
    editMenu->Append(ID_PasteFromClipboard, "Paste From Clipboard\tCtrl+Alt+V", "Paste all text from editor to clipboard");
    editMenu->AppendSeparator();
    editMenu->Append(ID_CheckPalindrome, "Check &Palindrome\tCtrl+P", "Check if the current text is a palindrome.");
    editMenu->AppendSeparator();
    editMenu->Append(ID_ReplaceFooBar, "Replace F&oo with Bar", "Replace all occurrences of 'foo' with 'bar'.");
    editMenu->Append(ID_ReverseText, "&Reverse Text", "Reverse the entire text in the editor.");
    editMenu->AppendSeparator();
    editMenu->Append(ID_SortLines, "Sort L&ines by Length", "Sort all lines in the editor by their length.");

    wxMenuBar *menubar = new wxMenuBar;
    menubar->Append(fileMenu, "&File");
    menubar->Append(editMenu, "&Edit");
    menubar->Append(helpMenu, "&Help");

    SetMenuBar(menubar);

    CreateStatusBar();
    SetStatusText("Welcome to the lightweight text editor: UNotePad");

    // Event bindings
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnSaveAs, this, wxID_SAVEAS);
    Bind(wxEVT_MENU, &MainFrame::OnUndo, this, wxID_UNDO);
    Bind(wxEVT_MENU, &MainFrame::OnRedo, this, wxID_REDO);
    Bind(wxEVT_MENU, &MainFrame::OnCut, this, wxID_CUT);
    Bind(wxEVT_MENU, &MainFrame::OnCopy, this, wxID_COPY);
    Bind(wxEVT_MENU, &MainFrame::OnPaste, this, wxID_PASTE);
    Bind(wxEVT_MENU, &MainFrame::OnMyMessage, this, ID_MyMessage);
    Bind(wxEVT_MENU, &MainFrame::OnSaveAsCustom, this, ID_SaveAsCustom);
    Bind(wxEVT_MENU, &MainFrame::OnOpenFile, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MainFrame::OnCopyToClipboard, this, ID_CopyToClipboard);
    Bind(wxEVT_MENU, &MainFrame::OnPasteFromClipboard, this, ID_PasteFromClipboard);
    Bind(wxEVT_STC_CHANGE, &MainFrame::OnEditorChanged, this, editor->GetId());
    Bind(wxEVT_MENU, &MainFrame::OnCheckPalindrome, this, ID_CheckPalindrome);
    Bind(wxEVT_MENU, &MainFrame::OnReplaceFooBar, this, ID_ReplaceFooBar);
    Bind(wxEVT_MENU, &MainFrame::OnReverseText, this, ID_ReverseText);
    Bind(wxEVT_MENU, &MainFrame::OnSortLines, this, ID_SortLines);

    Bind(wxEVT_MENU, &MainFrame::OnCheckPalindrome, this, ID_CheckPalindrome);
    Bind(wxEVT_BUTTON, &MainFrame::OnCheckPalindrome, this, ID_CheckPalindrome);

    Bind(wxEVT_MENU, &MainFrame::OnReplaceFooBar, this, ID_ReplaceFooBar);
    Bind(wxEVT_BUTTON, &MainFrame::OnReplaceFooBar, this, ID_ReplaceFooBar);

    Bind(wxEVT_MENU, &MainFrame::OnReverseText, this, ID_ReverseText);
    Bind(wxEVT_BUTTON, &MainFrame::OnReverseText, this, ID_ReverseText);

    Bind(wxEVT_MENU, &MainFrame::OnSortLines, this, ID_SortLines);
    Bind(wxEVT_BUTTON, &MainFrame::OnSortLines, this, ID_SortLines);

    Bind(wxEVT_BUTTON, &MainFrame::OnGoButton, this, ID_GoButton);

    OnEditorChanged(wxStyledTextEvent());
}

// GUI Event methods
void MainFrame::OnExit(wxCommandEvent &evt)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent &evt)
{
    AboutFrame *about = new AboutFrame(this, "About");
    about->Show(true);
}

void MainFrame::OnSaveAs(wxCommandEvent &WXUNUSED(evt))
{
    wxString str = this->editor->GetText();

    wxFileDialog saveFileAs(this, "Save as", "", "", "Plain text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileAs.ShowModal() == wxID_CANCEL)
    {
        return;
    }
    auto path = saveFileAs.GetPath();

    wxTextFile file;
    if (file.Create(path))
    {
        file.Clear();
        wxArrayString lines = wxStringTokenize(str, "\n", wxTOKEN_RET_EMPTY_ALL);
        for (const wxString &line : lines)
        {
            file.AddLine(line);
        }
        if (!file.Write())
        {
            wxMessageBox("Unable to save the file: Error writing content.");
        }
        file.Close();
    }
    else
    {
        wxMessageBox("Unable to save the file: Could not create/open file.");
    }
}

void MainFrame::OnUndo(wxCommandEvent &evt)
{
    this->editor->Undo();
}

void MainFrame::OnRedo(wxCommandEvent &evt)
{
    this->editor->Redo();
}

void MainFrame::OnCut(wxCommandEvent &evt)
{
    this->editor->Cut();
}

void MainFrame::OnCopy(wxCommandEvent &evt)
{
    this->editor->Copy();
}

void MainFrame::OnPaste(wxCommandEvent &evt)
{
    this->editor->Paste();
}

void MainFrame::OnMyMessage(wxCommandEvent &evt)
{
    wxMessageBox("Welcome, did it work???", "Fixed Message", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnSaveAsCustom(wxCommandEvent &WXUNUSED(evt))
{
    wxString contentToSave = this->editor->GetText();

    wxFileDialog saveFileDialog(this, "Save file as", "", "",
                                "Text files (*.txt)|*.txt|All files (*.*)|*.*",
                                wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    wxString filePath = saveFileDialog.GetPath();

    if (filePath.IsEmpty())
    {
        wxMessageBox("No file path selected. Please try again.", "Save Error", wxOK | wxICON_ERROR);
        return;
    }

    wxFileName fn(filePath);
    if (fn.DirExists())
    {
        wxMessageBox("The specified path is a directory. Please provide a file name, not a directory.", "Save Error", wxOK | wxICON_ERROR);
        return;
    }

    wxTextFile file;
    if (file.Create(filePath))
    {
        file.Clear();
        wxArrayString lines = wxStringTokenize(contentToSave, "\n", wxTOKEN_RET_EMPTY_ALL);
        for (const wxString &line : lines)
        {
            file.AddLine(line);
        }
        if (!file.Write())
        {
            wxMessageBox("An error occurred during writing data to the file.", "Write Error", wxOK | wxICON_ERROR);
        }
        file.Close();
    }
    else
    {
        wxString errorMessage;
        errorMessage.Printf("Failed to open file for writing: \"%s\".\nPossible reasons: no write permissions, invalid file name, or file is in use by another application.", filePath);
        wxMessageBox(errorMessage, "File Write Error", wxOK | wxICON_ERROR);
        return;
    }

    wxMessageBox("File saved successfully!", "Save Complete", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnOpenFile(wxCommandEvent &WXUNUSED(evt))
{
    wxFileDialog openFileDialog(this, "Open file", "", "",
                                "Text files (*.txt)|*.txt|All files (*.*)|*.*",
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    wxString filePath = openFileDialog.GetPath();

    if (filePath.IsEmpty())
    {
        wxMessageBox("No file path selected. Please try again.", "Open Error", wxOK | wxICON_ERROR);
        return;
    }

    wxFileName fn(filePath);
    if (fn.DirExists() && !fn.FileExists())
    {
        wxMessageBox("The selected path is a directory. Please select a file, not a directory.", "Open Error", wxOK | wxICON_ERROR);
        return;
    }

    wxTextFile file;
    if (!file.Open(filePath))
    {
        wxString errorMessage;
        errorMessage.Printf("Failed to open file: \"%s\".\nPossible reasons: file does not exist, no read permissions, or file is in use.", filePath);
        wxMessageBox(errorMessage, "File Open Error", wxOK | wxICON_ERROR);
        return;
    }

    wxString fileContent;
    for (size_t i = 0; i < file.GetLineCount(); ++i)
    {
        fileContent += file[i];
        if (i < file.GetLineCount() - 1)
        {
            fileContent += "\n";
        }
    }
    file.Close();

    this->editor->SetText(fileContent);

    wxMessageBox("File opened successfully!", "Open Complete", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnCopyToClipboard(wxCommandEvent &WXUNUSED(evt))
{
    wxString textToCopy = editor->GetText();

    if (wxTheClipboard->Open())
    {
        wxTheClipboard->SetData(new wxTextDataObject(textToCopy));
        wxTheClipboard->Close();
        SetStatusText("All text copied to clipboard!");
    }
    else
    {
        wxMessageBox("Could not open the clipboard.", "Error", wxOK | wxICON_ERROR);
    }
}

void MainFrame::OnPasteFromClipboard(wxCommandEvent &WXUNUSED(evt))
{
    if (wxTheClipboard->Open())
    {
        if (wxTheClipboard->IsSupported(wxDF_TEXT))
        {
            wxTextDataObject data;
            wxTheClipboard->GetData(data);
            wxString textToPaste = data.GetText();

            this->editor->ReplaceSelection(textToPaste);

            SetStatusText("Text pasted from clipboard.");
        }
        else
        {
            wxMessageBox("No text data found on clipboard.", "Paste Error", wxOK | wxICON_INFORMATION);
        }
        wxTheClipboard->Close();
    }
    else
    {
        wxMessageBox("Could not open the clipboard.", "Paste Error", wxOK | wxICON_ERROR);
    }
}

void MainFrame::OnEditorChanged(const wxStyledTextEvent &WXUNUSED(evt))
{
    wxString text = editor->GetText();
    size_t charCount = text.Length();

    wxStringTokenizer tokenizer(text, " \n\r");
    size_t wordCount = 0;
    while (tokenizer.HasMoreTokens())
    {
        tokenizer.GetNextToken();
        wordCount++;
    }
    wxString statusText;
    statusText.Printf("Char Count: %zu | Word Count: %zu", charCount, wordCount);
    SetStatusText(statusText);
}

void MainFrame::OnCheckPalindrome(wxCommandEvent &WXUNUSED(evt))
{
    wxString currentText = editor->GetText();
    if (CheckTextForPalindrome(currentText))
    {
        wxMessageBox("The text IS a palindrome!", "Palindrome Check", wxOK | wxICON_INFORMATION);
    }
    else
    {
        wxMessageBox("The text IS NOT a palindrome.", "Palindrome Check", wxOK | wxICON_INFORMATION);
    }
}

void MainFrame::OnReplaceFooBar(wxCommandEvent &WXUNUSED(evt))
{
    wxString originalText = editor->GetText();
    wxString newText = ProcessReplaceFooBar(originalText);
    editor->SetText(newText);
    wxMessageBox("All occurrences of 'foo' have been replaced with 'bar'.",
                 "Replace Complete", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnReverseText(wxCommandEvent &WXUNUSED(evt))
{
    wxString originalText = editor->GetText();
    wxString reversedText = ProcessReverseText(originalText);
    editor->SetText(reversedText);
    wxMessageBox("Text has been successfully reversed.",
                 "Text Reverse Complete", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnSortLines(wxCommandEvent &WXUNUSED(evt))
{
    wxString originalText = editor->GetText();
    wxString sortedText = ProcessSortLines(originalText);
    editor->SetText(sortedText);
    wxMessageBox("Lines have been sorted by length.",
                 "Sort Complete", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnGoButton(wxCommandEvent &WXUNUSED(evt))
{
    wxString searchText = searchCtrl->GetValue();
    wxString replaceText = replaceCtrl->GetValue();

    if (searchText.IsEmpty()) {
        wxMessageBox("Search field cannot be empty.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxString originalText = editor->GetText();
    wxString newText = originalText;
    wxString message;
    
    if (useRegexCheckBox->GetValue()) {
        // Use regex replacement
        wxRegEx regex(searchText, wxRE_ADVANCED | wxRE_ICASE); 
        if (!regex.IsValid()) {
            wxMessageBox("Invalid regular expression.", "Regex Error", wxOK | wxICON_ERROR);
            return;
        }
        long numReplaced = regex.ReplaceAll(&newText, replaceText);
        message.Printf("Replaced %ld occurrences using regex.", numReplaced);
    } else {
        // Simple string replacement
        long numReplaced = newText.Replace(searchText, replaceText);
        message.Printf("Replaced %ld occurrences of '%s' with '%s'.", numReplaced, searchText, replaceText);
    }
    
    editor->SetText(newText);
    wxMessageBox(message, "Replace Complete", wxOK | wxICON_INFORMATION);
}
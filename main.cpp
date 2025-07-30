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

// --- Text processing functions ---

// Helper function for std::sort, sorts strings by length
bool CompareLength(const wxString &s1, const wxString &s2) {
    return s1.Length() < s2.Length();
}

bool CheckTextForPalindrome(const wxString &text) {
    wxString cleanedText;
    for (wxChar c : text) {
        if (wxIsalnum(c)) { 
            cleanedText += wxTolower(c); 
        }
    }
    // Use explicit reverse iteration for reversal
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

// --- End of text processing functions ---


wxIMPLEMENT_APP(App);

// Function for command line processing
int OnCmdLine(wxCmdLineParser& parser, int argc, char **argv)
{
    // Add command line options
    parser.AddOption("palindrome", "p", "Check if text is a palindrome", wxCMD_LINE_VAL_STRING);
    parser.AddOption("replace", "r", "Replace 'foo' with 'bar' in text", wxCMD_LINE_VAL_STRING);
    parser.AddOption("reverse", "v", "Reverse text", wxCMD_LINE_VAL_STRING);
    parser.AddOption("sortlines", "s", "Sort lines by length", wxCMD_LINE_VAL_STRING);
    // Changed --file from AddSwitch to AddOption to accept a file path
    parser.AddOption("file", "f", "Read input from a file instead of a string argument", wxCMD_LINE_VAL_STRING);
    parser.AddOption("outputfile", "o", "Write output to a file instead of stdout", wxCMD_LINE_VAL_STRING);
    parser.AddSwitch("help", "h", "Show help message"); // Added help switch

    int parseResult = parser.Parse();

    if (parseResult == 0) // Successful parsing
    {
        wxString operationValue; // Value associated with the operation (e.g., text for --reverse)
        wxString operationFound;
        bool operationRequested = false;

        if (parser.Found("palindrome", &operationValue)) {
            operationFound = "palindrome";
            operationRequested = true;
        } else if (parser.Found("replace", &operationValue)) {
            operationFound = "replace";
            operationRequested = true;
        } else if (parser.Found("reverse", &operationValue)) {
            operationFound = "reverse";
            operationRequested = true;
        } else if (parser.Found("sortlines", &operationValue)) {
            operationFound = "sortlines";
            operationRequested = true;
        }
        
        wxString inputFilePath;
        bool inputFileMode = parser.Found("file", &inputFilePath); // Get file path if --file is used

        wxString outputFilePath;
        bool outputToFileMode = parser.Found("outputfile", &outputFilePath);

        if (parser.Found("help")) { // If help is requested
            parser.Usage();
            return 0; // Exit after showing usage
        }


        if (operationRequested)
        {
            wxString inputText;
            if (inputFileMode) {
                // If --file is present, use inputFilePath as the source
                if (inputFilePath.IsEmpty()) {
                    std::cerr << "Error: --file requires a file path." << std::endl;
                    return 1;
                }
                wxTextFile file;
                if (!file.Open(inputFilePath)) {
                    std::cerr << "Error: Could not open input file for reading: " << std::string(inputFilePath.ToUTF8()) << std::endl;
                    return 1;
                }
                for (size_t i = 0; i < file.GetLineCount(); ++i) {
                    inputText += file[i];
                    if (i < file.GetLineCount() - 1) {
                        inputText += "\n";
                    }
                }
                file.Close();
            } else {
                // Otherwise, use the value passed directly to the operation
                if (operationValue.IsEmpty()) {
                    std::cerr << "Error: No input text or file specified for operation." << std::endl;
                    return 1;
                }
                inputText = operationValue;
            }

            std::string result;
            bool boolResult = false;

            if (operationFound == "palindrome") {
                boolResult = CheckTextForPalindrome(inputText);
                result = boolResult ? "1" : "0";
            } else if (operationFound == "replace") {
                result = std::string(ProcessReplaceFooBar(inputText).ToUTF8());
            } else if (operationFound == "reverse") {
                result = std::string(ProcessReverseText(inputText).ToUTF8());
            } else if (operationFound == "sortlines") {
                result = std::string(ProcessSortLines(inputText).ToUTF8());
            }

            if (outputToFileMode) {
                if (outputFilePath.IsEmpty()) {
                    std::cerr << "Error: --outputfile requires a file path." << std::endl;
                    return 1;
                }
                wxFileOutputStream outputStream(outputFilePath);
                if (!outputStream.IsOk()) {
                    std::cerr << "Error: Could not open output file: " << std::string(outputFilePath.ToUTF8()) << std::endl;
                    return 1;
                }
                outputStream.Write(result.c_str(), result.length());
                if (!outputStream.IsOk()) {
                    std::cerr << "Error: Could not write to output file: " << std::string(outputFilePath.ToUTF8()) << std::endl;
                    return 1;
                }
                std::cout << "Output written to file: " << std::string(outputFilePath.ToUTF8()) << std::endl;
            } else {
                std::cout << result << std::endl;
            }

            return 0; 
        } else {
            parser.Usage();
            return 1; 
        }
    }
    else if (parseResult == -1) 
    {
        return 1; 
    }

    return -1; 
}

bool App::OnInit()
{
    // New variables for refined manual parsing
    wxString manual_operation_type = wxEmptyString;
    wxString manual_input_value = wxEmptyString;
    bool manual_input_is_file = false;
    wxString manual_output_file_path = wxEmptyString;
    bool manual_output_to_file = false;
    bool help_requested = false;

    // Parse all command line arguments into our temporary variables
    for (int i = 1; i < argc; ++i) {
        wxString arg = argv[i];

        if (arg == "--palindrome" || arg == "-p") {
            manual_operation_type = "palindrome";
            // If the next arg is NOT --file, then it's the direct text value
            if (i + 1 < argc && !(wxString(argv[i+1]) == "--file" || wxString(argv[i+1]) == "-f")) {
                manual_input_value = argv[++i];
            }
        } else if (arg == "--replace" || arg == "-r") {
            manual_operation_type = "replace";
            if (i + 1 < argc && !(wxString(argv[i+1]) == "--file" || wxString(argv[i+1]) == "-f")) {
                manual_input_value = argv[++i];
            }
        } else if (arg == "--reverse" || arg == "-v") {
            manual_operation_type = "reverse";
            if (i + 1 < argc && !(wxString(argv[i+1]) == "--file" || wxString(argv[i+1]) == "-f")) {
                manual_input_value = argv[++i];
            }
        } else if (arg == "--sortlines" || arg == "-s") {
            manual_operation_type = "sortlines";
            if (i + 1 < argc && !(wxString(argv[i+1]) == "--file" || wxString(argv[i+1]) == "-f")) {
                manual_input_value = argv[++i];
            }
        } else if (arg == "--file" || arg == "-f") {
            manual_input_is_file = true;
            if (i + 1 < argc) {
                manual_input_value = argv[++i]; // Consume next arg as file path
            } else {
                std::cerr << "Error: --file requires a file path." << std::endl;
                return false; 
            }
        } else if (arg == "--outputfile" || arg == "-o") {
            manual_output_to_file = true;
            if (i + 1 < argc) {
                manual_output_file_path = argv[++i]; // Consume next arg as output file path
            } else {
                std::cerr << "Error: --outputfile requires a file path." << std::endl;
                return false; 
            }
        } else if (arg == "--help" || arg == "-h") {
            help_requested = true;
        }
        // Unrecognized argument or direct text input (if not already handled by --file)
        // This 'else' block for direct text input is now primarily for positional arguments
        // and should be handled with care to avoid misinterpreting flags.
        // For current logic, if an operation is followed by --file, manual_input_value would remain empty
        // untocessed, then manual_input_value gets the file path.
        // If an operation il --file is pris NOT followed by --file, it grabs the immediate next argument.
        // This is a common pattern: `tool --op VALUE` OR `tool --op --file PATH`.
    }

    // Now, process based on the parsed values
    if (!manual_operation_type.IsEmpty() || help_requested) { // If an operation was requested or help
        if (help_requested) {
            std::cout << "Usage: UNotePad <operation> [input_value] [--file <path>] [--outputfile <path>]" << std::endl;
            std::cout << "Operations:" << std::endl;
            std::cout << "  --palindrome, -p <text>\tCheck if text is a palindrome." << std::endl;
            std::cout << "  --replace, -r <text>\tReplace 'foo' with 'bar' in text." << std::endl;
            std::cout << "  --reverse, -v <text>\tReverse text." << std::endl;
            std::cout << "  --sortlines, -s <text>\tSort lines by length." << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --file, -f <path>\t\tRead input from a file. If used, the <text> for operation is ignored." << std::endl;
            std::cout << "  --outputfile, -o <path>\tWrite output to a file instead of stdout." << std::endl;
            std::cout << "  --help, -h\t\tShow this help message." << std::endl;
            return false; 
        }
        
        wxString processedInputText;
        if (manual_input_is_file) {
            if (manual_input_value.IsEmpty()) {
                std::cerr << "Error: --file flag requires a file path." << std::endl;
                return false;
            }
            wxTextFile file;
            if (!file.Open(manual_input_value)) { 
                std::cerr << "Error: Could not open input file for reading: " << std::string(manual_input_value.ToUTF8()) << std::endl;
                return false;
            }
            for (size_t i = 0; i < file.GetLineCount(); ++i) {
                processedInputText += file[i];
                if (i < file.GetLineCount() - 1) {
                    processedInputText += "\n";
                }
            }
            file.Close();
        } else {
            if (manual_input_value.IsEmpty()) {
                std::cerr << "Error: No input text provided for the operation." << std::endl;
                return false;
            }
            processedInputText = manual_input_value;
        }

        std::string result;
        bool boolResult = false;

        if (manual_operation_type == "palindrome") {
            boolResult = CheckTextForPalindrome(processedInputText);
            result = boolResult ? "1" : "0";
        } else if (manual_operation_type == "replace") {
            result = std::string(ProcessReplaceFooBar(processedInputText).ToUTF8());
        } else if (manual_operation_type == "reverse") {
            result = std::string(ProcessReverseText(processedInputText).ToUTF8());
        } else if (manual_operation_type == "sortlines") {
            result = std::string(ProcessSortLines(processedInputText).ToUTF8());
        }

        if (manual_output_to_file) {
            if (manual_output_file_path.IsEmpty()) {
                std::cerr << "Error: --outputfile requires a file path." << std::endl;
                return false;
            }
            wxFileOutputStream outputStream(manual_output_file_path);
            if (!outputStream.IsOk()) {
                std::cerr << "Error: Could not open output file for writing: " << std::string(manual_output_file_path.ToUTF8()) << std::endl;
                return false;
            }
            outputStream.Write(result.c_str(), result.length());
            if (!outputStream.IsOk()) {
                std::cerr << "Error: Could not write to output file: " << std::string(manual_output_file_path.ToUTF8()) << std::endl;
                return false;
            }
            std::cout << "Output written to file: " << std::string(manual_output_file_path.ToUTF8()) << std::endl;
        } else {
            std::cout << result << std::endl;
        }
        return false; 
    } 

    // Fallback to wxCmdLineParser if manual parsing does not handle the arguments
    wxCmdLineParser parser(argc, argv);
    int cmdLineResult = OnCmdLine(parser, argc, argv);

    if (cmdLineResult == 0) 
    {
        return false;
    }
    else if (cmdLineResult == 1) 
    {
        return false;
    }

    // Launch GUI if no command line arguments were provided or handled
    MainFrame *frm = new MainFrame("UNotePad");
    frm->Show(true);

    return true;
}

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxPoint(50, 50), wxSize(800, 600))
{
    panel = new wxPanel(this, wxID_ANY);
    editor = new wxStyledTextCtrl(panel, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxHSCROLL | wxVSCROLL);

#if WIN32
    // this->SetIcon(wxICON(appicon));
#endif

    // Widgets
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *panelsizer = new wxBoxSizer(wxVERTICAL);

    panelsizer->Add(editor, 1, wxALL | wxEXPAND, 0);
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
    editMenu->Append(In_ReverseText, "&Reverse Text", "Reverse the entire text in the editor.");
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
    Bind(wxEVT_MENU, &MainFrame::OnReverseText, this, In_ReverseText);
    Bind(wxEVT_MENU, &MainFrame::OnSortLines, this, ID_SortLines);

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
        for (const wxString& line : lines) {
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
    wxString content_to_save = this->editor->GetText();

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
    if (file.Create(filePath)) {
        file.Clear();
        wxArrayString lines = wxStringTokenize(content_to_save, "\n", wxTOKEN_RET_EMPTY_ALL);
        for (const wxString& line : lines) {
            file.AddLine(line);
        }
        if (!file.Write()) {
            wxMessageBox("An error occurred during writing data to the file.", "Write Error", wxOK | wxICON_ERROR);
        }
        file.Close();
    } else {
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
    if (!file.Open(filePath)) {
        wxString errorMessage;
        errorMessage.Printf("Failed to open file: \"%s\".\nPossible reasons: file does not exist, no read permissions, or file is in use.", filePath);
        wxMessageBox(errorMessage, "File Open Error", wxOK | wxICON_ERROR);
        return;
    }

    wxString fileContent;
    for (size_t i = 0; i < file.GetLineCount(); ++i) {
        fileContent += file[i];
        if (i < file.GetLineCount() - 1) {
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
    wxString statusText;
    statusText.Printf("Char Count: %zu", charCount);
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
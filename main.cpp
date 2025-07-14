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

bool App::OnInit() {
    MainFrame *frm = new MainFrame("UNotePad");
    frm->Show(true);

    return true;
}

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
#if WIN32
    this->SetIcon(wxICON(appicon)); // Set the application icon
#endif // WIN32

    //
    // Widgets
    //

    wxBoxSizer *main = new wxBoxSizer(wxVERTICAL);        // The main sizer contains all the widgets

    wxBoxSizer *panelsizer = new wxBoxSizer(wxVERTICAL);  // The sizer of the panel

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
    file->Append(wxID_OPEN); // Пункт меню "Открыть"
    file->AppendSeparator();
    file->Append(wxID_SAVE);
    file->Append(wxID_SAVEAS);
    file->Append(ID_SaveAsCustom, "Save as custom \tCtrl+Shift+S", "Save as custum pro");
    file->AppendSeparator();
    file->Append(wxID_EXIT);

    wxMenu *help = new wxMenu;
    help->Append(wxID_ABOUT);
    help->Append(ID_MyMessage, "Паведамленне \tCtrl+M", "Паказаць паведамленне");

    wxMenu *edit = new wxMenu;
    edit->Append(wxID_UNDO);
    edit->Append(wxID_REDO);
    edit->AppendSeparator();
    edit->Append(wxID_COPY);
    edit->Append(wxID_CUT);
    edit->Append(wxID_PASTE);
    edit->AppendSeparator(); 
    edit->Append(ID_CopyToClipboard, "Copy To Clipboard\tCtrl+Alt+C", "Copy all text from editor to clipboard");
    edit->Append(ID_PasteFromClipboard, "Paste From Clipboard\tCtrl+Alt+V", "Paste all text from editor to clipboard");

    wxMenuBar *menubar = new wxMenuBar;
    menubar->Append(file, "&File");
    menubar->Append(edit, "&Edit");
    menubar->Append(help, "&Help");

    SetMenuBar(menubar);

    CreateStatusBar();
    SetStatusText("Welcome to the lightweight text editor: UNotePad");

    //
    // Event bindings
    //
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

}

void MainFrame::OnExit(cmd& evt) {
    Close(true);
}

void MainFrame::OnAbout(cmd& evt) {
    AboutFrame *about = new AboutFrame(this, "About");
    about->Show(true);
}

void MainFrame::OnSaveAs(cmd& WXUNUSED(evt)) {

    // The text of the editor
    wxString str = this->editor->GetText();

    // Getting the path
    wxFileDialog saveFileAs(this, "Save as", "", "", "Plain text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if(saveFileAs.ShowModal() == wxID_CANCEL) {
        return;
    }
    auto path = saveFileAs.GetPath();

    // Write the str to the file
    wxFileOutputStream output(path);
    if(!output.IsOk()) {
        wxMessageBox("Unable to save the file");
        return;
    }

    wxFile *file = new wxFile(path, wxFile::write);
    if(file->IsOpened()) {
        file->Write(str);
    }
    file->Close();

}


//
// The following forward declared functions are self-explanatory :)
//

void MainFrame::OnUndo(cmd& evt) {
    this->editor->Undo();
}

void MainFrame::OnRedo(cmd& evt) {
    this->editor->Redo();
}

void MainFrame::OnCut(cmd& evt) {
    this->editor->Cut();
}

void MainFrame::OnCopy(cmd& evt) {
    this->editor->Copy();
}

void MainFrame::OnPaste(cmd& evt) {
    this->editor->Paste();
}

void MainFrame::OnMyMessage(cmd& evt) {
    wxMessageBox("Вітаю, зрабілася???", "Фіксіраванве паведамленне", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnSaveAsCustom(cmd& WXUNUSED(evt)) {
    wxString content_to_save = this->editor->GetText();

    wxFileDialog saveFileDialog(this, "Save file as", "", "",
                                "Text files (*.txt)|*.txt|All files (*.*)|*.*",
                                wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    wxString filePath = saveFileDialog.GetPath();

    if (filePath.IsEmpty()) {
        wxMessageBox("No file path selected. Please try again.", "Save Error", wxOK | wxICON_ERROR);
        return;
    }

    wxFileName fn(filePath);
    if (fn.DirExists()) {
        wxMessageBox("The specified path is a directory. Please provide a file name, not a directory.", "Save Error", wxOK | wxICON_ERROR);
        return;
    }

    wxFileOutputStream outputStream(filePath);

    if (!outputStream.IsOk()) {
        wxString errorMessage;
        errorMessage.Printf("Failed to open file for writing: \"%s\".\nPossible reasons: no write permissions, invalid file name, or file is in use by another application.", filePath);
        wxMessageBox(errorMessage, "File Write Error", wxOK | wxICON_ERROR);
        return;
    }

    outputStream.Write(content_to_save.mb_str(), content_to_save.length());

    if (!outputStream.IsOk()) {
        wxMessageBox("An error occurred during writing data to the file.", "Write Error", wxOK | wxICON_ERROR);
        return;
    }

    wxMessageBox("File saved successfully!", "Save Complete", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnOpenFile(cmd& WXUNUSED(evt)) {
    wxFileDialog openFileDialog(this, "Open file", "", "",
                                "Text files (*.txt)|*.txt|All files (*.*)|*.*", // Фильтры файлов
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    // диалог
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    // Получаем выбранный путь к файлу
    wxString filePath = openFileDialog.GetPath();

    wxString debugLogPath = wxStandardPaths::Get().GetTempDir() + wxFileName::GetPathSeparator() + "notepad_debug.log";
    wxFile debugLogFile;


    if (debugLogFile.Open(debugLogPath, wxFile::write_append)) {
        debugLogFile.Write(wxString::Format("DEBUG: [%s] Attempting to open file.\n", wxDateTime::Now().FormatISOTime()));
        debugLogFile.Write(wxString::Format("DEBUG: Selected file path: \"%s\"\n", filePath));

        wxFileName fn(filePath);
        debugLogFile.Write(wxString::Format("DEBUG: wxFileName believes path exists as directory: %d\n", fn.DirExists()));
        debugLogFile.Write(wxString::Format("DEBUG: wxFileName believes path exists as file: %d\n", fn.FileExists()));
        debugLogFile.Write(wxString::Format("DEBUG: wxFileName is an absolute path: %d\n", fn.IsAbsolute()));
        debugLogFile.Write(wxString::Format("DEBUG: wxFileName is valid: %d\n", fn.IsOk()));
        debugLogFile.Close();

    } else {
        wxMessageBox(wxString::Format("Could not open debug log file for writing: \"%s\"\n"
                                      "Check permissions or if the path is valid.", debugLogPath),
                     "Debug Error - File Write Failed", wxOK | wxICON_ERROR);
    }


    // Проверка на пустой путь (дополнительная защита)
    if (filePath.IsEmpty()) {
        wxMessageBox("No file path selected. Please try again.", "Open Error", wxOK | wxICON_ERROR);
        return;
    }

    // Проверка, является ли путь к файлу директорией
    wxFileName fn(filePath);
    if (fn.DirExists() && !fn.FileExists()) {
        wxMessageBox("The selected path is a directory. Please select a file, not a directory.", "Open Error", wxOK | wxICON_ERROR);
        return;
    }

    // Попытка открыть файл для чтения
    wxFileInputStream inputStream(filePath);

    // Проверяем, успешно ли открыт поток для чтения
    if (!inputStream.IsOk()) {
        wxString errorMessage;
        errorMessage.Printf("Failed to open file: \"%s\".\nPossible reasons: file does not exist, no read permissions, or file is in use.", filePath);
        wxMessageBox(errorMessage, "File Open Error", wxOK | wxICON_ERROR);
        return;
    }

    // Читаем содержимое файла в строку
    wxString fileContent;
    char buffer[4096]; // Буфер для чтения частями
    size_t bytesRead;

    while ((bytesRead = inputStream.Read(buffer, sizeof(buffer)).LastRead()) > 0) {
        fileContent += wxString(buffer, bytesRead);
    }

    // Проверяем, произошли ли ошибки чтения (например, во время чтения)
    if (!inputStream.IsOk() && inputStream.GetLastError() != wxSTREAM_EOF) { 
        wxMessageBox("An error occurred during reading data from the file.", "Read Error", wxOK | wxICON_ERROR);
        return;
    }

    // Устанавливаем прочитанный текст в редактор
    this->editor->SetText(fileContent);

    wxMessageBox("File opened successfully!", "Open Complete", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnCopyToClipboard(cmd& WXUNUSED(evt)) {
    wxString textToCopy = editor->GetText(); // Получаем весь текст из редактора

    if (wxTheClipboard->Open()) { 
      
        wxTheClipboard->SetData(new wxTextDataObject(textToCopy));
        wxTheClipboard->Close(); 
        SetStatusText("All text copied to clipboard!"); 
    } else {
        wxMessageBox("Could not open the clipboard.", "Error", wxOK | wxICON_ERROR);
    }
}

void MainFrame::OnPasteFromClipboard(cmd& WXUNUSED(evt)){
    if (wxTheClipboard->Open()) { 
        if (wxTheClipboard->IsSupported(wxDF_TEXT)) { 
            wxTextDataObject data;
            wxTheClipboard->GetData(data); // Получаем данные из буфера обмена в объект 'data'
            wxString textToPaste = data.GetText(); // Извлекаем сам текст из объекта 'data'

            // Вставляем полученный текст в редактор.
        
            this->editor->ReplaceSelection(textToPaste);

            SetStatusText("Text pasted from clipboard."); 
        } else {
            wxMessageBox("No text data found on clipboard.", "Paste Error", wxOK | wxICON_INFORMATION); 
        }
        wxTheClipboard->Close(); 
    } else {
        wxMessageBox("Could not open the clipboard.", "Paste Error", wxOK | wxICON_ERROR); 
    }
}
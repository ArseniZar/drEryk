

#ifndef UNOTEPADWINDOW_H 
#define UNOTEPADWINDOW_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/stc/stc.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/clipbrd.h>
#include <wx/dataobj.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <cctype>

#include "about.h"
#include "utils.h"

enum MyCustomIDs
{
    ID_COPY_ALL_FIELDS = wxID_HIGHEST + 1,
    ID_PASTE_ALL_FIELDS = wxID_HIGHEST + 2,
    ID_TOOLS_CHECK_PALINDROME = wxID_HIGHEST + 3,
    ID_TOOLS_REPLACE_FOO_BAR = wxID_HIGHEST + 4,
    ID_TOOLS_REVERS_TEXT =  wxID_HIGHEST + 5,
    ID_TOOLS_SORT_LINES_BY_TEXT = wxID_HIGHEST + 6,
};

constexpr char ICONS_PATH[] = "icons/";

/**
 * @brief The main UNotePadWindow class
 */
class UNotePadWindow : public wxApp
{
public:
    /**
     * @brief The application entry point
     *
     * @return true
     */
    virtual bool OnInit();
};

wxDECLARE_APP(UNotePadWindow);

/**
 * @brief The main window object.
 *
 */
class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title);

private:
    using cmd = wxCommandEvent;

    wxStyledTextCtrl *editor;

    // Event methods for menu items
    void OnInfo(cmd &evt);
    void OnExit(cmd &evt);
    void OnAbout(cmd &evt);
    void OnSaveAs(cmd &WXUNUSED(evt));
    void OnOpen(cmd &evt);
    void OnSaveAsCustom(cmd &evt);
    void OnUndo(cmd &evt);
    void OnRedo(cmd &evt);
    void OnCopy(cmd &evt);
    void OnCopyCustom(cmd &evt);
    void OnCut(cmd &evt);
    void OnPaste(cmd &evt);
    void OnCheckPalindrome(cmd &evt);
    void OnReplaceFooOnBar(cmd &evt);
    void OnTextChanged(cmd &evt);
    void OnReverseText(cmd &evt);
    void OnSortLinesByLength(cmd &evt);
};

void SaveStringToFile(const std::string &filepath, const std::string &content);
std::string ReadFileToString(const std::filesystem::path &path);

#endif // MAIN_H

/**
 * @mainpage UNotePad
 * This is a simple text editor application written in C++ using [wxWidgets](https://wxwidgets.org) (which is a cross platform library for building graphical user interfaces). This project is for beginners who wants to learn about basics about wxWidgets. You can just read the code. It don't has a good documentation but is has a readable and self-explanatory code. You can learn basics of wxWidgets (sizers, frames, events, file handling, etc) by reading the code.
 *
 * Currently this app is saving an Untitled.txt file in the same directory. I will add a file dialog box and let the user to choose the directory to save the file when I have a time. And also feel free to contribute to this project.
 *
 * ## Build Instructions
 *
 * To build this project you should have installed [CMake](https://cmake.org), a c++ compiler and built wxWidgets from source using CMake. (For wxWidgets build instructions see https://docs.wxwidgets.org/latest/overview_cmake.html).
 *
 * After that, open up the terminal:
 * ```bash
 * git clone https://github.com/usithadev/UNotePad.git #For this you need git installed. You can also go to the github repository and download the coe.
 *
 * cd UNotePad
 *
 * cmake -DwxWidgets_DIR=path/to/wxWidgets -S . -B ./build -DCMAKE_BUILD_TYPE="Release" -G "MinGW Makefiles"
 *
 * cd build
 *
 * cmake --build .
 *
 * ```
 *
 * Now you can find the UNotePad.exe (and the doxygen documentation files) in the build folder.
 *
 * @author Usitha Indeewara
 *
 */
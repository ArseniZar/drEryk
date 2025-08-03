#ifndef APP_H
#define APP_H

#include <wx/wx.h>
#include <wx/cmdline.h>
#include <wx/textfile.h>
#include <wx/wfstream.h>
#include <wx/filename.h>
#include <iostream>

class App : public wxApp {
public:
    virtual bool OnInit() override;
};

#endif // APP_H



#include "main.h"
#include "UNotePadCli.h"
#include "UNotePadWindow.h"
#include "Command.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {

        wxApp::SetInstance(new UNotePadWindow());
        return wxEntry(argc, argv);
    }
    else
    {
        Command cmd = parseCommand(argv[1]);
        if (cmd == Command::Cli)
        {
            UNotePadCli app;
            return app.run(argc, argv);
        }
        else if (cmd == Command::Help)
        {
            printAllCommands();
            return 0;
        }
    }
}
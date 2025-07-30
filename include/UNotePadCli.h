#ifndef UNOTEPADCLI_H
#define UNOTEPADCLI_H
#include <iostream>
#include "Command.h"
#include "utils.h"


class UNotePadCli
{
public:
    UNotePadCli();
    int run(int &argc, char **argv);
};

#endif
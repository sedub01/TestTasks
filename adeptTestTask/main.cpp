#include "mainmenu.h"
#include "tree.h"

#include <QCoreApplication>
#include <iostream>

int main()
{
    ITree* tree = new Tree();
    MainMenu menu(tree);
    menu.exe();
    delete tree;
    return 0;
}

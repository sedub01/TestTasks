#ifndef MAINMENU_H
#define MAINMENU_H

#include "itree.h"
#include <iostream>

#define MAXLEN 80
#define FILENAME "./treeData.txt"

#ifdef linux  //for linux
#define CLS system("clear")
#elif __APPLE__ //for mac
#define CLS system("clear")
#else
#define CLS system("cls") //for windows
#endif

using std::cout;
using std::cin;

class MainMenu
{
    ITree* tree {nullptr};
public:
    MainMenu(ITree* tree);
    void exe();

private:
    static int safe_cin();
    void addNewNodeMenu();
    void editNodeMenu();
    void deleteNodeMenu();
};

#endif // MAINMENU_H

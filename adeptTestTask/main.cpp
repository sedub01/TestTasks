#include "mainmenu.h"
#include "tree.h"

#include <QCoreApplication>
#include <iostream>
#include <memory>

int main()
{
    std::shared_ptr<ITree> tree = std::make_shared<Tree>();
    MainMenu menu(tree.get());
    menu.exe();
    return 0;
}

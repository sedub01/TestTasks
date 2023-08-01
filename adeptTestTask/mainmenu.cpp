#include "mainmenu.h"
#include "node.h"

#include <QDebug>

MainMenu::MainMenu(ITree *tree): tree(tree) {
    tree->loadTree(FILENAME);
}

void MainMenu::exe()
{
    int option = -1;
    cout << "Hello! I'm tree management app.\nWhat should I do?\n";
    while (option){
        cout << "\nChoose the option:\n";
        cout << "[1] Print the tree\n";
        cout << "[2] Add new node\n";
        cout << "[3] Edit node name by id\n";
        cout << "[4] Delete node by id\n";
        cout << "[0] Exit\n";

        option = safe_cin();
        CLS;

        if (option == 1){
            cout << "Resulted tree:\n";
            tree->printTree();
        }
        else if (option == 2){
            addNewNodeMenu();
        }
        else if (option == 3){
            editNodeMenu();
        }
        else if (option == 4){
            deleteNodeMenu();
        }
        else if (option == 0){
            tree->saveTree(FILENAME);
            cout << "Goodbye!\n";
        }
        else{
            cout << "Wrong option!\n";
        }
    }
}

int MainMenu::safe_cin()
{
    int chooce;
    char str[MAXLEN];

    scanf("%s", str);
    while(sscanf(str, "%d", &chooce) != 1) {
        printf("\nIncorrect input! Try again use only numbers!\n");
        scanf("%s" , str);
    }
    return chooce;
}

void MainMenu::addNewNodeMenu()
{
    char newName[MAXLEN];
    int parentId;
    cout << "Enter name of new node: ";
    cin >> newName;
    while (getchar() != '\n');
    cout << "Enter parent node id: ";
    parentId = safe_cin();
    tree->addNode(newName, parentId);
    cout << "Node added!\n";
}

void MainMenu::editNodeMenu()
{
    int id;
    char newName[MAXLEN];
    while (getchar() != '\n');
    cout << "Enter id node: ";
    id = safe_cin();
    cout << "Enter new name: ";
    cin >> newName;
    bool bf = tree->changeName(id, newName);
    cout << (bf? "Name changed!\n": "Node not found\n");
}

void MainMenu::deleteNodeMenu()
{
    int id;
    while (getchar() != '\n');
    cout << "Enter id node: ";
    id = safe_cin();
    bool bf = tree->deleteNode(id);
    cout << (bf? "Node deleted!\n": "Node not found\n");
}

#ifndef TREE_H
#define TREE_H

#include "itree.h"
#include <iostream>

using std::cout;

class Tree : public ITree
{
    Node* root {nullptr};
    int nodeCount = 0;

public:
    explicit Tree(QObject *parent = nullptr);

    // ITree interface
public:
    void addNode(const QString &name, int parentId) override;
    void saveTree(const QString &filename) override;
    void loadTree(const QString &filename) override;
    void printTree() override;
    Node* findChild(int id) override;
    bool changeName(int id, char *newName) override;
    bool deleteNode(int id) override;

private:
    void serializeNode(const Node *node, QJsonObject &json);
    Node *deserializeNode(const QJsonObject &json);
    void printNode(const Node *node, int indent);
};

#endif // TREE_H

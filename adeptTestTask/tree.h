#ifndef TREE_H
#define TREE_H

#include "itree.h"
#include <iostream>
#include <memory>

using std::cout;

class Tree : public ITree
{
    std::shared_ptr<Node> root {nullptr};
    int nodeCount = 0;

public:
    explicit Tree() {}
    ~Tree() = default;

    // ITree interface
public:
    void addNode(const QString &name, int parentId) override;
    void saveTree(const QString &filename) const override;
    void loadTree(const QString &filename) override;
    void printTree() const override;
    std::shared_ptr<Node> findChild(int id) const override;
    bool changeName(int id, char *newName) override;
    bool deleteNode(int id) override;

private:
    void serializeNode(const Node *node, QJsonObject &json) const;
    std::shared_ptr<Node> deserializeNode(const QJsonObject &json);
    void printNode(const Node *node, int indent) const;
};

#endif // TREE_H

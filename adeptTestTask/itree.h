#ifndef ITREE_H
#define ITREE_H

#include <QObject>
#include <memory>

class Node;

class ITree
{
public:
    explicit ITree() {}
    virtual ~ITree() {};

    virtual void addNode(const QString& name, int parentId) = 0;
    virtual void saveTree(const QString& filename) const = 0;
    virtual void loadTree(const QString& filename) = 0;
    virtual void printTree() const = 0;
    virtual std::shared_ptr<Node> findChild(int id) const = 0;
    virtual bool changeName(int id, char* newName) = 0;
    virtual bool deleteNode(int id) = 0;
};

#endif // ITREE_H

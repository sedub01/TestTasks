#ifndef ITREE_H
#define ITREE_H

#include <QObject>

class Node;

class ITree : public QObject
{
    Q_OBJECT
public:
    explicit ITree(QObject *parent = nullptr): QObject(parent) {}
    virtual ~ITree() {};

    virtual void addNode(const QString& name, int parentId) = 0;
    virtual void saveTree(const QString& filename) = 0;
    virtual void loadTree(const QString& filename) = 0;
    virtual void printTree() = 0;
    virtual Node* findChild(int id) = 0;
    virtual bool changeName(int id, char* newName) = 0;
    virtual bool deleteNode(int id) = 0;
};

#endif // ITREE_H

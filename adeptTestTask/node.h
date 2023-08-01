#ifndef NODE_H
#define NODE_H

#include <QObject>

class Node : public QObject
{
    Q_OBJECT

public:
    int id;
    QString name;
    int parentId;
    QList<Node*> children;

    explicit Node(int id, const QString& name, int parentId, QObject *parent):
        QObject(parent), id(id), name(name), parentId(parentId) {}

    ~Node(){
        for (auto child: qAsConst(children)){
            delete child;
            child = nullptr;
        }
    }

    void addChild(Node* child) {
        children.append(child);
    }

    Node* findChild(int id){
        for (const auto child: qAsConst(children)){
            if (child->id == id){
                return child;
            }
        }

        for (const auto child: qAsConst(children)){
            const auto neededNode = child->findChild(id);
            if (neededNode){
                return neededNode;
            }
        }

        return nullptr;
    }

    void deleteChild(Node* node){
        if (node){
            for (const auto child: qAsConst(node->children)){
                deleteChild(child);
            }
            delete node;
        }
    }
};

#endif // NODE_H

#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <memory>

class Node
{
public:
    int id;
    QString name;
    int parentId;
    QList<std::shared_ptr<Node>> children;

    explicit Node(int id, const QString& name, int parentId):
        id(id), name(name), parentId(parentId) {}
    ~Node() {}

    void addChild(std::shared_ptr<Node> child) {
        children.append(child);
    }

    std::shared_ptr<Node> findChild(int id) const{
        for (const auto &child: qAsConst(children)){
            if (child->id == id){
                return child;
            }
        }

        for (const auto &child: qAsConst(children)){
            const auto neededNode = child->findChild(id);
            if (neededNode){
                return neededNode;
            }
        }

        return nullptr;
    }

    void deleteChild(std::shared_ptr<Node> node){
        if (node){
            for (const auto &child: qAsConst(node->children)){
                deleteChild(child);
            }
            children.removeOne(node);
        }
    }
};

#endif // NODE_H

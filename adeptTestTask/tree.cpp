#include "tree.h"

#include "node.h"

#include <QFile>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

void Tree::addNode(const QString &name, int parentId)
{
    auto newNode = std::make_shared<Node>(nodeCount++, name, parentId);
    if (!root){
        newNode->id = newNode->parentId = 0;
        root = newNode;
    }
    else{
        const auto parentNode = root->findChild(parentId);
        if (parentNode){
            parentNode->addChild(newNode);
        }
        else{
            newNode->parentId = 0;
            root->addChild(newNode);
        }
    }
}

void Tree::saveTree(const QString &filename) const
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject json;
        serializeNode(root.get(), json);
        QJsonDocument doc(json);
        file.write(doc.toJson());
        file.close();
    }
    else{
        qDebug() << "Failed to save data to the disk.";
    }
}

void Tree::loadTree(const QString &filename)
{
    QFile file(filename);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject json = doc.object();
        if (!json.isEmpty()){
            nodeCount = 1;
            root = deserializeNode(json);
        }
        file.close();
    }
    else{
        qDebug() << "Failed to load data from the disk.";
    }
}

void Tree::printTree() const
{
    printNode(root.get(), 0);
}

std::shared_ptr<Node> Tree::findChild(int id) const
{
    if (root->id == id)
        return root;
    return root->findChild(id);
}

bool Tree::changeName(int id, char *newName)
{
    const auto node = this->findChild(id);
    if (node){
        node->name = newName;
        return true;
    }
    return false;
}

bool Tree::deleteNode(int id)
{
    const auto node = this->findChild(id);
    if (node){
        const auto parentNode = this->findChild(node->parentId);
        if (parentNode){
            parentNode->deleteChild(node);
            return true;
        }
    }
    return false;
}

void Tree::serializeNode(const Node *node, QJsonObject &json) const
{
    if (!node)
        return;

    json["id"] = node->id;
    json["name"] = node->name;
    json["parent"] = node->parentId;
    QJsonArray childrenJson;
    for (const auto &child : node->children) {
        QJsonObject childJson;
        serializeNode(child.get(), childJson);
        childrenJson.append(childJson);
    }
    json["children"] = childrenJson;
}

std::shared_ptr<Node> Tree::deserializeNode(const QJsonObject &json)
{
    int id = json["id"].toInt();
    QString name = json["name"].toString();
    int parentId = json["parentId"].toInt();
    auto node = std::make_shared<Node>(id, name, parentId);
    QJsonArray childrenJson = json["children"].toArray();
    for (const QJsonValue& childJson : qAsConst(childrenJson)) {
        QJsonObject childObj = childJson.toObject();
        const auto child = deserializeNode(childObj);
        node->addChild(child);
        nodeCount++;
    }
    return node;
}

void Tree::printNode(const Node *node, int indent) const
{
    if (!node)
        return;

    QString indentation(indent, ' ');
    cout << indentation.toStdString() << "[" << node->id << ", " <<
            node->parentId << "] " << node->name.toStdString() << "\n";
    for (const auto &child: qAsConst(node->children)){
        printNode(child.get(), indent+2);
    }
}

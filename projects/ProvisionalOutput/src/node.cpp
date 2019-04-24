#include "node.hpp"

Node::Node(const int argW, const int argH, Node* argParentNodePtr) {
    mW = argW;
    mH = argH;
    mThisNodePtr = this;
    mParentNodePtr = argParentNodePtr;
    
    if (mParentNodePtr != nullptr) {
        mTreeHigh = mParentNodePtr->getTreeHigh()+1;
    }
    else {
        mTreeHigh = 0;
    }
}

void Node::setChildNode(Node* argChildNodePtr) {
    mChildNodesPtr.push_back(argChildNodePtr);
    //argChildNodePtr->setParentNode(mThisNodePtr);
}

Node* Node::getChildNode(const int n) {
    if (n > mChildNodesPtr.size()) {
        cout << "要素数がノードの数を超えています" << endl;
        return nullptr;
    }
    
    return mChildNodesPtr[n];
}

void Node::setParentNode(Node* argParentNodePtr) {
    mParentNodePtr = argParentNodePtr;
}

Node* Node::getParentNode() {
    return mParentNodePtr;
}

Node* Node::getThisNode() {
    return mThisNodePtr;
}

Vec2 Node::getPosition() {
    return Vec2{mW, mH};
}

int Node::getTreeHigh() {
    return mTreeHigh;
}

#include "node.hpp"

/**
 Node::Node:
 Nodeクラスのコンストラクタ
 ノードの持つ座標と親ノードを設定する（親ノードがない場合はnullptrを指定）
 
 @param argW                ノードに設定するx座標
 @param argH                ノードに設定するy座標
 @param argParentNodePtr    設定する親ノードのポインタ
 */
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

/**
 Node::setChildNode:
 子ノードを設定する
 
 @param argChildNodePtr 設定する子ノードのポインタ
 */
void Node::setChildNode(Node* argChildNodePtr) {
    mChildNodesPtr.push_back(argChildNodePtr);
    //argChildNodePtr->setParentNode(mThisNodePtr);
}

/**
 Node::getChildNode:
 子ノードのポインタを返す
 
 @param n 子ノードの番号
 @return n番目の子ノードのポインタ
 */
Node* Node::getChildNode(const int n) {
    if (n > mChildNodesPtr.size()) {
        cout << "要素数がノードの数を超えています" << endl;
        return nullptr;
    }
    
    return mChildNodesPtr[n];
}

/**
 Node::setParentNode:
 親ノードの設定を行う
 
 @param argParentNodePtr 親ノードのポインタ
 */
void Node::setParentNode(Node* argParentNodePtr) {
    mParentNodePtr = argParentNodePtr;
}

/**
 Node::getParentNode:
 親ノードのポインタを返す
 
 @return 親ノードのポインタ
 */
Node* Node::getParentNode() {
    return mParentNodePtr;
}

/**
 Node::getThisNode:
 このノードのポインタを返す
 
 @return このノードのポインタ
 */
Node* Node::getThisNode() {
    return mThisNodePtr;
}

/**
 Node::getPosition:
 このノードが持つVec2型で座標を返す
 
 @return このノードが持つ座標
 */
Vec2 Node::getPosition() {
    return Vec2{mW, mH};
}

/**
 Node::getTreeHigh:
 ノードの高さ（一番上の親からどれだけ離れているか）を返す
 
 @return ノードの高さ
 */
int Node::getTreeHigh() {
    return mTreeHigh;
}


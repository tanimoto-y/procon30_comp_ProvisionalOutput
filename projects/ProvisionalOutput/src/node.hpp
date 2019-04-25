#ifndef node_hpp
#define node_hpp

#pragma once

#include <Siv3D.hpp> // OpenSiv3D v0.3.1
#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;

class Node {
    vector<Node*> mChildNodesPtr;       // 子ノードのポインタ
    Node* mParentNodePtr = nullptr;     // 親ノードのポインタ
    Node* mThisNodePtr;                 // このノードのポインタ
    
    int mX, mY;                         // 座標
    int mTreeHigh;                      // ノードの高さ
    
public:
    Node(const int argX, const int argY, Node* argParentNodePtr);   // コンストラクタ
    void setChildNode(Node* argChildNodePtr);                       // 子ノードの設定
    Node* getChildNode(const int n);                                // 子ノードのポインタを返す
    void setParentNode(Node* argParentNodePtr);                     // 子ノードの設定
    Node* getParentNode();                                          // 親ノードのポインタを返す
    Node* getThisNode();                                            // このノードのポインタを返す
    Vec2 getPosition();                                             // このノードが持つ座標を返す
    int getTreeHigh();                                              // ノードの高さを返す
};

#endif /* node_hpp */


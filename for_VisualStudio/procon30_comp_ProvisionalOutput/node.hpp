#ifndef node_hpp
#define node_hpp

#pragma once

#include <Siv3D.hpp> // OpenSiv3D v0.3.2
#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;

class Node {
    vector<Node*> mChildNodesPtr;       // �q�m�[�h�̃|�C���^
    Node* mParentNodePtr = nullptr;     // �e�m�[�h�̃|�C���^
    Node* mThisNodePtr;                 // ���̃m�[�h�̃|�C���^
    
    int mX, mY;                         // ���W
    int mTreeHigh;                      // �m�[�h�̍���
    
public:
    Node(const int argX, const int argY, Node* argParentNodePtr);   // �R���X�g���N�^
    void setChildNode(Node* argChildNodePtr);                       // �q�m�[�h�̐ݒ�
    Node* getChildNode(const int n);                                // �q�m�[�h�̃|�C���^��Ԃ�
    void setParentNode(Node* argParentNodePtr);                     // �q�m�[�h�̐ݒ�
    Node* getParentNode();                                          // �e�m�[�h�̃|�C���^��Ԃ�
    Node* getThisNode();                                            // ���̃m�[�h�̃|�C���^��Ԃ�
    Vec2 getPosition();                                             // ���̃m�[�h�������W��Ԃ�
    int getTreeHigh();                                              // �m�[�h�̍�����Ԃ�
};

#endif /* node_hpp */


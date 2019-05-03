#include "node.hpp"

/**
 Node::Node:
 Node�N���X�̃R���X�g���N�^
 �m�[�h�̎����W�Ɛe�m�[�h��ݒ肷��i�e�m�[�h���Ȃ��ꍇ��nullptr���w��j
 
 @param argX                �m�[�h�ɐݒ肷��x���W
 @param argY                �m�[�h�ɐݒ肷��y���W
 @param argParentNodePtr    �ݒ肷��e�m�[�h�̃|�C���^
 */
Node::Node(const int argX, const int argY, Node* argParentNodePtr) {
    mX = argX;
    mY = argY;
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
 �q�m�[�h��ݒ肷��
 
 @param argChildNodePtr �ݒ肷��q�m�[�h�̃|�C���^
 */
void Node::setChildNode(Node* argChildNodePtr) {
    mChildNodesPtr.push_back(argChildNodePtr);
    //argChildNodePtr->setParentNode(mThisNodePtr);
}

/**
 Node::getChildNode:
 �q�m�[�h�̃|�C���^��Ԃ�
 
 @param n �q�m�[�h�̔ԍ�
 @return n�Ԗڂ̎q�m�[�h�̃|�C���^
 */
Node* Node::getChildNode(const int n) {
    if (n > mChildNodesPtr.size()) {
        cout << "�v�f�����m�[�h�̐��𒴂��Ă��܂�" << endl;
        return nullptr;
    }
    
    return mChildNodesPtr[n];
}

/**
 Node::setParentNode:
 �e�m�[�h�̐ݒ���s��
 
 @param argParentNodePtr �e�m�[�h�̃|�C���^
 */
void Node::setParentNode(Node* argParentNodePtr) {
    mParentNodePtr = argParentNodePtr;
}

/**
 Node::getParentNode:
 �e�m�[�h�̃|�C���^��Ԃ�
 
 @return �e�m�[�h�̃|�C���^
 */
Node* Node::getParentNode() {
    return mParentNodePtr;
}

/**
 Node::getThisNode:
 ���̃m�[�h�̃|�C���^��Ԃ�
 
 @return ���̃m�[�h�̃|�C���^
 */
Node* Node::getThisNode() {
    return mThisNodePtr;
}

/**
 Node::getPosition:
 ���̃m�[�h������Vec2�^�ō��W��Ԃ�
 
 @return ���̃m�[�h�������W
 */
Vec2 Node::getPosition() {
    return Vec2{mX, mY};
}

/**
 Node::getTreeHigh:
 �m�[�h�̍����i��ԏ�̐e����ǂꂾ������Ă��邩�j��Ԃ�
 
 @return �m�[�h�̍���
 */
int Node::getTreeHigh() {
    return mTreeHigh;
}


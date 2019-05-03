/*
 header.hpp
 
 �萔�A�O���[�o���ϐ��A�\���́A���O��Ԃ̐錾
 */

#ifndef Header_h
#define Header_h

/*------------------- �萔 --------------------*/
#define WINDOW_SIZE_W               1280    // �E�B���h�E�̉���
#define WINDOW_SIZE_H               720     // �E�B���h�E�̏c��

#define FIELD_LINE_MAX_LENGTH       700     // �t�B�[���h�̘g�̍ő咷
#define FIELD_SIZE_MAX              20      // �t�B�[���h��1��̍ő�̃}�X�̐�

#define POINT_TEXT_FONT_SIZE        40      // �_���\���p�̃t�H���g�̃T�C�Y�i�t�B�[���h��1�}�X������̑傫������������l�j
#define POINT_TEXT_BIG_FONT_SIZE    30      // �_���\���p�̑傫���t�H���g�̃T�C�Y�i�t�B�[���h��1�}�X������̑傫������������l�j
#define AGENT_ID_TEXT_FONT_SIZE     55      // �G�[�W�F���gID�\���p�̃t�H���g�̃T�C�Y�i�t�B�[���h��1�}�X������̑傫������������l�j

#define DIRECTIONS                  9       // �G�[�W�F���g���ړ��ł�������̐��i�u�Ƃǂ܂�v���܂ށj

/*---------------- �O���[�o���萔 ----------------*/
// �G�[�W�F���g���ړ��ł�����W
const Vec2 gMoveDirections[9] {
    {-1, -1}, { 0, -1}, { 1, -1},
    {-1,  0}, { 0,  0}, { 1,  0},
    {-1,  1}, { 0,  1}, { 1,  1}
};

// ����4�����i�㉺���E�j�̍��W
const Vec2 gSearchTileDirections[4] = {
    {0, -1}, {1, 0}, {0, 1}, {-1, 0}
};

/*-------------------- �\���� --------------------*/
/**
 Score:
 ���_�̊i�[�p�̍\���́B
 
 @var tile   �^�C���|�C���g
 @var area   �̈�|�C���g
 @function total ���v�_�itile+area�j��Ԃ��֐�
 */
typedef struct Score {
    int tile = 0;
    int area = 0;
    
    int total() {
        return tile + area;
    }
} Score;

/*-------------------- ���O��� --------------------*/
// �`�[���̐F
namespace TeamColor {
    enum Type {
        BLUE    = 0,
        RED     = 1,
    };
}

// �`�[���������l
namespace Team {
    enum Type {
        ALLY    = 0,
        ENEMY   = 1,
    };
}

// �^�C���̏�Ԃ������l
namespace TileStatus {
    enum Type {
        ALLY    =  1,
        NONE    =  0,
        ENEMY   = -1,
    };
}

// �G�[�W�F���g�̍s���̔ԍ�
namespace AgentActNumbers {
    enum Type {
        STAY                = 0,    // �����Ȃ�
        
        GO_UP_LEFT          = 1,    // �ړ�: ����
        GO_UP               = 2,    // �ړ�: ��
        GO_UP_RIGHT         = 3,    // �ړ�: �E��
        GO_RIGHT            = 4,    // �ړ�: �E
        GO_DOWN_RIGHT       = 5,    // �ړ�: �E��
        GO_DOWN             = 6,    // �ړ�: ��
        GO_DOWN_LEFT        = 7,    // �ړ�: ����
        GO_LEFT             = 8,    // �ړ�: ��
        
        REMOVE_UP_LEFT      = 9,    // ����: ����
        REMOVE_UP           = 10,   // ����: ��
        REMOVE_UP_RIGHT     = 11,   // ����: �E��
        REMOVE_RIGHT        = 12,   // ����: �E
        REMOVE_DOWN_RIGHT   = 13,   // ����: �E��
        REMOVE_DOWN         = 14,   // ����: ��
        REMOVE_DOWN_LEFT    = 15,   // ����: ����
        REMOVE_LEFT         = 16,   // ����: ��
    };
}


#endif

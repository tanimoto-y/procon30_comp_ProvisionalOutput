#ifndef field_hpp
#define field_hpp

#pragma once

#include <Siv3D.hpp> // OpenSiv3D v0.3.2
#include <stdio.h>
#include "node.hpp"
#include "header.hpp"

using namespace std;

// �t�B�[���h�N���X
class Field {
public:
    // �R���X�g���N�^�i���������s���j
    Field(const vector<vector<int>> &argFieldPointsArray, const vector<vector<int>> &argFieldStatusArray, const Vec2 position, const bool argAllyTeamColor);
    
    // �t�B�[���h�̐ݒ�
    void setField(const vector<vector<int>> &argFieldPointsArray, const vector<vector<int>> &argFieldStatusArray);
    
    // �t�B�[���h����G�[�W�F���g�̈ʒu���擾
    // �v�f�ԍ��̓G�[�W�F���gID�Ɠ����ŁA����̃G�[�W�F���g�̏ꍇ�̓G�[�W�F���gID�̐�Βl���v�f�ԍ��ɂȂ�
    // ��: �����1�l�ڂ̃G�[�W�F���g�i�G�[�W�F���gID -1�j��x���W -> argEnemyAgentsPosition[1].first
    void getAgentsPosition(vector<pair<int, int>> &argAllyAgentsPosition, vector<pair<int, int>> &argEnemyAgentsPosition);
    
    // �e�`�[���̃G�[�W�F���g�̐l����Ԃ�
    int getTotalTeamAgents();
    
    // �t�B�[���h��1�}�X�̑傫����Ԃ�
    int getFieldSquareSize();
    
    // ���_��Ԃ�
    int getTotalPoints(const Team::Type argTeam);     // ���v���_
    int getTilePoints(const Team::Type argTeam);      // �^�C���|�C���g
    int getAreaPoints(const Team::Type argTeam);      // �̈�|�C���g
    
    // Field�N���X�Ŏg���t�H���g�̐ݒ�
    void setFont(const Font argPointTextFont, const Font argPointTextFontBold, const Font argAgentIDTextFont);
    
    // �}�X�̏�ɃJ�[�\�������邩���f
    bool isCursorOnTheSquare(const Vec2 argPosition);
    
    // �̈�|�C���g�̎擾
    void searchAreaPointsSide(vector<vector<bool>> argFieldMark, const int argStartX, const int argStartY, const int argX, const int argY, Node* argParentNode);
    bool searchAreaPointsSquares(vector<vector<bool>>& argAreaSquares, const int argStartX, const int argStartY, const int argBeTileStatus, const bool argDeleteArea);
    void searchAreaPoints(const int argStartX, const int argStartY);
    
    // �}�X�̓h��Ԃ�
    void fillSquare(const Vec2 argSquarePosition, const int argX, const int argY, Color& rectColor);
    
    // �̈�̓h��Ԃ�
    void fillAreaSquare(const Vec2 argSquarePosition, const int argX, const int argY);
    
    // �^�C���_���̕\��
    void printSquarePoint(const Vec2 argSquarePosition, const int argX, const int argY, Color& rectColor);
    
    // �^�C������
    void removeTile(const int argX, const int argY, const int argTileStatus);
    
    // �^�C���ݒu
    void putTile(const int argX, const int argY, const int argTileStatus);
    
    // �G�[�W�F���g�̍s��
    int agentMovement(int argX, int argY, int argBeforeX, int argBeforeY);
    
    // �I������G�[�W�F���g�̕ύX
    void setCurrentAgent(int argX, int argY);
    
    // �}�X�̕`��
    void drawSquares();
    
    // �t�B�[���h�̕`��
    void draw();
    
private:
    vector<vector<int>> mFieldPointsArray;              // �t�B�[���h�̓_���i�[�p
    vector<vector<int>> mFieldStatusArray;              // �t�B�[���h�̏󋵊i�[�p
    vector<vector<int>> mFieldAgentsIDArray;            // �t�B�[���h��̃G�[�W�F���g�̏ꏊ�̊i�[�p
    vector<vector<bool>> mFieldAllyAreaSquaresArray;    // �����̗̈�̊i�[�p
    vector<vector<bool>> mFieldEnemyAreaSquaresArray;   // ����̗̈�̊i�[�p
    vector<vector<bool>> mFieldAreaSideLinesArray;      // �̈�𐬂��ӂ̊i�[�p
    
    vector<int> mAllyAgentsActNumbers;              // �����G�[�W�F���g�̍s���̔ԍ�
    vector<int> mEnemyAgentsActNumbers;             // ����G�[�W�F���g�̍s���̔ԍ�
    
    Score mAllyScore;                               // �����`�[���̓��_�i���v���_ total()�A�^�C���|�C���g tile�A�̈�|�C���g area���܂ލ\���́j
    Score mEnemyScore;                              // ����`�[���̓��_�i���v���_ total()�A�^�C���|�C���g tile�A�̈�|�C���g area���܂ލ\���́j
    
    int mFieldSizeW, mFieldSizeH;                   // �t�B�[���h�̑傫��
    int mFieldLineLengthW, mFieldLineLengthH;       // �t�B�[���h�̘g�̒���
    int mFieldSquareSize;                           // �t�B�[���h��1�}�X������̑傫���i�����`�j
    
    int mTotalTeamAgents;                           // �e�`�[���̃G�[�W�F���g�̑���
    
    int mFieldLeftmostPoint;                        // �t�B�[���h�̕`��͈͂̍��[�̍��W
    int mFieldRightmostPoint;                       // �t�B�[���h�̕`��͈͂̉E�[�̍��W
    int mFieldTopmostPoint;                         // �t�B�[���h�̕`��͈͂̏�[�̍��W
    int mFieldBottommostPoint;                      // �t�B�[���h�̕`��͈͂̉��[�̍��W
    
    bool mAllyTeamColor;                            // �����`�[���̐F�i�G�`�[���̐F��!allyTeamColor�Ŏ擾�ł���j
    
    Vec2 mCurrentSquarePosition = { -1, -1 };       // �I������Ă���}�X�̍��W�i�I������Ă��Ȃ����(-1, -1)�j
    int mCurrentAgentID = 0;                        // �I������Ă���}�X�ɂ���G�[�W�F���g��ID�i�I������Ă��Ȃ���� 0�j
    
    bool mMousePressing = false;                    // �}�E�X��������Ă���Ԃ�true
    
    Font mPointTextFont;                            // �_���\���p�̃t�H���g
    Font mPointTextFontBold;                        // �_���\���p�̑����t�H���g
    Font mAgentIDTextFont;                          // �G�[�W�F���gID�\���p�̃t�H���g
    
    int mBigPointsBorder;                           // �傫���_���̊�i���̒l�𒴂���Ƒ����œ_����\������j
};

#endif /* field_hpp */


#include "field.hpp"

using namespace std;

/**
 centerText:
 �����Ŏw�肵�����W�̒����ɕ�����\������֐��B
 ������(String)�^�p
 
 @param argStr          �\�����镶����
 @param argLeftFRONTX     �����x���W
 @param argLeftFRONTY     �����y���W
 @param argRightBottomX �E����x���W
 @param argRightBottomY �E����y���W
 @param argFont         �\���Ɏg���t�H���g�̃|�C���^
 @param argColor        �����̐F
 */
void centerText(const String argStr, const int argLeftFRONTX, const int argLeftFRONTY, const int argRightBottomX, const int argRightBottomY, const Font& argFont, const Color argColor) {
    unsigned long int textLength = argStr.length();
    
    argFont(argStr).draw(argLeftFRONTX+(argRightBottomX-argLeftFRONTX)/2-textLength*argFont.fontSize()/3, argLeftFRONTY+(argRightBottomY-argLeftFRONTY)/2-argFont.fontSize()/2, argColor);
}
/**
 centerText:
 �����Ŏw�肵�����W�̒����ɕ�����\������֐��B
 ���l(int)�^�p
 
 @param argInt          �\�����鐔�l
 @param argLeftFRONTX     �����x���W
 @param argLeftFRONTY     �����y���W
 @param argRightBottomX �E����x���W
 @param argRightBottomY �E����y���W
 @param argFont         �\���Ɏg���t�H���g�̃|�C���^
 @param argColor        �����̐F
 */
void centerText(const int argInt, const int argLeftFRONTX, const int argLeftFRONTY, const int argRightBottomX, const int argRightBottomY, const Font& argFont, const Color argColor) {
    unsigned long int textLength = to_string(argInt).length();
    
    argFont(argInt).draw(argLeftFRONTX+(argRightBottomX-argLeftFRONTX)/2-textLength*argFont.fontSize()/3, argLeftFRONTY+(argRightBottomY-argLeftFRONTY)/2-argFont.fontSize()/2, argColor);
}

/**
 printTotalPoints:
 ���_�ƊT�v�̕\��
 
 @param argTeam                 �ǂ���̃`�[����\�����邩�iTeam::ALLY or Team::ENEMY�j
 @param argTeamColor            �`�[���̐F�iTeamColor::RED or TeamColor::BLUE�j
 @param argFieldData            �t�B�[���h�N���X�̃|�C���^
 @param argFont16               �T�C�Y16�̃t�H���g�̃|�C���^
 @param argFont16Bold           �T�C�Y16�̑����t�H���g�̃|�C���^
 @param argFont28               �T�C�Y28�̃t�H���g�̃|�C���^
 @param argTotalPointsTextFont  �T�C�Y28�̑����t�H���g�̃|�C���^
 */
void printTotalPoints(const Team::Type argTeam, const bool argTeamColor, Field& argFieldData, const Font& argFont16, const Font& argFont16Bold, const Font& argFont28, const Font& argTotalPointsTextFont) {
    // �h��Ԃ��F�i=�`�[���̐F�j�̐ݒ�
    Color rectColor;
    if (argTeamColor == TeamColor::BLUE) {
        rectColor = Color(0, 100, 200);
    }
    else {
        rectColor = Color(200, 10, 0);
    }
    
    String statusStr;
    
    // �\�����e�������̏��̏ꍇ
    if (argTeam == Team::ALLY) {
        // �w�i�F
        Rect(WINDOW_SIZE_W-290+100, 50, WINDOW_SIZE_W-290-100, 300).draw(rectColor);
        
        // "Ally"�ƕ\��
        argFont28(U"Ally").draw(WINDOW_SIZE_W-290+165, 60, Color(Palette::White));
        
        // �����_��\��
        centerText(argFieldData.getTotalPoints(Team::ALLY), WINDOW_SIZE_W-190, 80, WINDOW_SIZE_W, 150, argTotalPointsTextFont, Color(Palette::White));
        
        // �^�C���|�C���g�̕\��
        argFont16(U"�^�C��").draw(WINDOW_SIZE_W-290+150, 200, Color(Palette::White));
        argFont16Bold(argFieldData.getTilePoints(Team::ALLY)).draw(WINDOW_SIZE_W-290+150+60, 200, Color(Palette::White));
        
        if (argFieldData.getTilePoints(Team::ALLY) > argFieldData.getTilePoints(Team::ENEMY)) {
            Circle(WINDOW_SIZE_W-290+140, 200+12, 5).draw(Color(Palette::Lightgreen));
        }
        else if (argFieldData.getTilePoints(Team::ALLY) == argFieldData.getTilePoints(Team::ENEMY)) {
            Circle(WINDOW_SIZE_W-290+140, 200+12, 5).draw(Color(Palette::Gray));
        }
        else {
            Circle(WINDOW_SIZE_W-290+140, 200+12, 5).draw(Color(Palette::Orange));
        }
        
        // �̈�|�C���g�̕\��
        argFont16(U"�̈�").draw(WINDOW_SIZE_W-290+150+15, 230, Color(Palette::White));
        argFont16Bold(argFieldData.getAreaPoints(Team::ALLY)).draw(WINDOW_SIZE_W-290+150+60, 230, Color(Palette::White));
        
        if (argFieldData.getAreaPoints(Team::ALLY) > argFieldData.getAreaPoints(Team::ENEMY)) {
            Circle(WINDOW_SIZE_W-290+140, 230+12, 5).draw(Color(Palette::Lightgreen));
        }
        else if (argFieldData.getAreaPoints(Team::ALLY) == argFieldData.getAreaPoints(Team::ENEMY)) {
            Circle(WINDOW_SIZE_W-290+140, 230+12, 5).draw(Color(Palette::Gray));
        }
        else {
            Circle(WINDOW_SIZE_W-290+140, 230+12, 5).draw(Color(Palette::Orange));
        }
        
        /*
         argFont18(U"�_��").draw(WINDOW_SIZE_W-290+150, 200, Color(Palette::White));
         argFont18Bold(argFieldData.getTotalPoints(Team::ALLY)-argFieldData.getTotalPoints(Team::ENEMY)).draw(WINDOW_SIZE_W-290+200, 200, Color(Palette::White));
         argFont18(U"��").draw(WINDOW_SIZE_W-290+150, 230, Color(Palette::White));
         if (argFieldData.getTotalPoints(Team::ALLY)-argFieldData.getTotalPoints(Team::ENEMY) > 0) {
         statusStr = U"����";
         }
         else if (argFieldData.getTotalPoints(Team::ALLY)-argFieldData.getTotalPoints(Team::ENEMY) < 0) {
         statusStr = U"����";
         }
         else {
         statusStr = U"��������";
         }
         */
        
        //argFont16Bold(statusStr).draw(WINDOW_SIZE_W-290+200, 230, Color(Palette::White));
    }
    // �\�����e������̏��̏ꍇ
    else if (argTeam == Team::ENEMY) {
        // �w�i�F
        Rect(0, 50, 290-100, 300).draw(rectColor);
        
        // "Enemy"�ƕ\��
        argFont28(U"Enemy").draw(50, 60, Color(Palette::White));
        
        // �����_��\��
        centerText(argFieldData.getTotalPoints(Team::ENEMY), 0, 80, 190, 150, argTotalPointsTextFont, Color(Palette::White));
        
        // �^�C���|�C���g�̕\��
        argFont16(U"�^�C��").draw(50, 200, Color(Palette::White));
        argFont16Bold(argFieldData.getTilePoints(Team::ENEMY)).draw(50+60, 200, Color(Palette::White));
        
        if (argFieldData.getTilePoints(Team::ENEMY) > argFieldData.getTilePoints(Team::ALLY)) {
            Circle(40, 200+12, 5).draw(Color(Palette::Lightgreen));
        }
        else if (argFieldData.getTilePoints(Team::ENEMY) == argFieldData.getTilePoints(Team::ALLY)) {
            Circle(40, 200+12, 5).draw(Color(Palette::Gray));
        }
        else {
            Circle(40, 200+12, 5).draw(Color(Palette::Orange));
        }
        
        // �̈�|�C���g�̕\��
        argFont16(U"�̈�").draw(50+15, 230, Color(Palette::White));
        argFont16Bold(argFieldData.getAreaPoints(Team::ENEMY)).draw(50+60, 230, Color(Palette::White));
        
        if (argFieldData.getAreaPoints(Team::ENEMY) > argFieldData.getAreaPoints(Team::ALLY)) {
            Circle(40, 230+12, 5).draw(Color(Palette::Lightgreen));
        }
        else if (argFieldData.getAreaPoints(Team::ENEMY) == argFieldData.getAreaPoints(Team::ALLY)) {
            Circle(40, 230+12, 5).draw(Color(Palette::Gray));
        }
        else {
            Circle(40, 230+12, 5).draw(Color(Palette::Orange));
        }
        
        /*
         argFont16(U"�_��").draw(50, 200, Color(Palette::White));
         argFont16Bold(argFieldData.getTotalPoints(Team::ENEMY)-argFieldData.getTotalPoints(Team::ALLY)).draw(100, 200, Color(Palette::White));
         argFont16(U"��").draw(50, 230, Color(Palette::White));
         if (argFieldData.getTotalPoints(Team::ENEMY)-argFieldData.getTotalPoints(Team::ALLY) > 0) {
         statusStr = U"����";
         }
         else if (argFieldData.getTotalPoints(Team::ENEMY)-argFieldData.getTotalPoints(Team::ALLY) < 0) {
         statusStr = U"����";
         }
         else {
         statusStr = U"��������";
         }
         argFont16Bold(statusStr).draw(100, 230, Color(Palette::White));
         */
    }
}

/**
 Main�֐�
 */
void Main() {
    /*----------�E�B���h�E�̑傫���ݒ�--------------*/
    Window::Resize(WINDOW_SIZE_W, WINDOW_SIZE_H);
    
    /*-----------------�w�i�F--------------------*/
    Graphics::SetBackground(Color(Palette::White));
    
    /*-------------���̑��̕ϐ��̐錾--------------*/
    int fieldSizeW = 10;        // �t�B�[���h�̉���
    int fieldSizeH = 10;        // �t�B�[���h�̏c��
    
    /*--------------�t�B�[���h�̍쐬--------------*/
    // �t�B�[���h�̓��_
    /*
     vector<vector<int>> fieldPointsArray(fieldSizeH, vector<int>(fieldSizeW));
     fieldPointsArray[0] = { -2,  1,  0,  1,  2,  0,  2,  1,  0,  1, -2 };
     fieldPointsArray[1] = {  1,  0,  2, -2,  0,  1,  0, -2,  2,  0,  1 };
     fieldPointsArray[2] = {  1,  3,  0,  1,  0, -2,  0,  1,  0,  3,  1 };
     fieldPointsArray[3] = {  2,  1,  1,  2,  2,  3,  2,  2,  1,  1,  2 };
     fieldPointsArray[4] = {  2,  1,  1,  2,  2,  3,  2,  2,  1,  1,  2 };
     fieldPointsArray[5] = {  1,  3,  0,  1,  0, -2,  0,  1,  0,  3,  1 };
     fieldPointsArray[6] = {  1,  0,  2, -2,  0,  1,  0, -2,  2,  0,  1 };
     fieldPointsArray[7] = { -2,  1,  0,  1,  2,  0,  2,  1,  0,  1, -2 };
     
     // �t�B�[���h�̏󋵁i1: �����̃^�C�� -1: ����̃^�C�� 0: �ǂ���ł��Ȃ��j
     vector<vector<int>> fieldStatusArray(fieldSizeH, vector<int>(fieldSizeW));
     fieldStatusArray[0] = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };
     fieldStatusArray[1] = {  0,  1,  0,  0,  0,  0,  0,  0,  0, -1,  0 };
     fieldStatusArray[2] = {  0,  0, -1,  0,  0,  0,  0,  0,  1,  0,  0 };
     fieldStatusArray[3] = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };
     fieldStatusArray[4] = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };
     fieldStatusArray[5] = {  0,  0,  1,  0,  0,  0,  0,  0, -1,  0,  0 };
     fieldStatusArray[6] = {  0, -1,  0,  0,  0,  0,  0,  0,  0,  1,  0 };
     fieldStatusArray[7] = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };
     */
    
    vector<vector<int>> fieldPointsArray(fieldSizeH, vector<int>(fieldSizeW));
    fieldPointsArray[0] = {  1,  2,  3,  4,  5,  5,  4,  3,  2,  1 };
    fieldPointsArray[1] = { -2, -1,  0,  1,  2,  2,  1,  0, -1, -2 };
    fieldPointsArray[2] = { 16, 14, 12, 10,  8,  8, 10, 12, 14, 16 };
    fieldPointsArray[3] = {  8,  7,  6,  5,  4,  4,  5,  6,  7,  8 };
    fieldPointsArray[4] = {  6,  7,  8,  9, 10, 10,  9,  8,  7,  6 };
    fieldPointsArray[5] = {  6,  7,  8,  9, 10, 10,  9,  8,  7,  6 };
    fieldPointsArray[6] = {  8,  7,  6,  5,  4,  4,  5,  6,  7,  8 };
    fieldPointsArray[7] = { 16, 14, 12, 10,  8,  8, 10, 12, 14, 16 };
    fieldPointsArray[8] = { -2, -1,  0,  1,  2,  2,  1,  0, -1, -2 };
    fieldPointsArray[9] = {  1,  2,  3,  4,  5,  5,  4,  3,  2,  1 };
    
    // �t�B�[���h�̏󋵁i1: �����̃^�C�� -1: ����̃^�C�� 0: �ǂ���ł��Ȃ��j
    vector<vector<int>> fieldStatusArray(fieldSizeH, vector<int>(fieldSizeW));
    fieldStatusArray[0] = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };
    fieldStatusArray[1] = {  0,  1,  0,  0,  0,  0,  0,  0, -1,  0 };
    fieldStatusArray[2] = {  0,  0, -1,  0,  0,  0,  0,  1,  0,  0 };
    fieldStatusArray[3] = {  0,  0,  0,  1,  0,  0, -1,  0,  0,  0 };
    fieldStatusArray[4] = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };
    fieldStatusArray[5] = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };
    fieldStatusArray[6] = {  0,  0,  0, -1,  0,  0,  1,  0,  0,  0 };
    fieldStatusArray[7] = {  0,  0,  1,  0,  0,  0,  0, -1,  0,  0 };
    fieldStatusArray[8] = {  0, -1,  0,  0,  0,  0,  0,  0,  1,  0 };
    fieldStatusArray[9] = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };
    
    // �����`�[���̐F
    bool allyTeamColor = TeamColor::BLUE;       // ��: TeamColor::RED ��: TeamColor::BLUE
    
    // �t�B�[���h�̍쐬
    Field fieldData(fieldPointsArray, fieldStatusArray, Vec2(fieldSizeW, fieldSizeH), allyTeamColor);
    
    // �t�B�[���h����G�[�W�F���g�̈ʒu���擾
    vector<pair<int, int>> allyAgentsPosition;
    vector<pair<int, int>> enemyAgentsPosition;
    fieldData.getAgentsPosition(allyAgentsPosition, enemyAgentsPosition);
    
    for (int i = 0; i < fieldData.getTotalTeamAgents(); i++) {
        cout << "Ally [" << i << "] (Agent ID: " << i+1 << ") : (" << allyAgentsPosition[i].first << ',' << allyAgentsPosition[i].second << ") | ";
        cout << "Enemy [" << i << "] (Agent ID: " << -(i+1) << ") : (" << enemyAgentsPosition[i].first << ',' << enemyAgentsPosition[i].second << ")" << endl;
    }
    
    /*---------------�t�H���g�̗p��---------------*/
    // �t�B�[���h�`��p
    const Font pointTextFont(fieldData.getFieldSquareSize()-POINT_TEXT_FONT_SIZE);
    const Font pointTextFontBold(fieldData.getFieldSquareSize()-POINT_TEXT_BIG_FONT_SIZE, Typeface::Bold);
    const Font agentIDTextFont(fieldData.getFieldSquareSize()-AGENT_ID_TEXT_FONT_SIZE);
    
    // ���_�\���p
    const Font totalPointsTextFont(72);
    
    // ���̑�
    const Font font28(28);
    const Font font16(16);
    const Font font16Bold(16, Typeface::Bold);
    
    // �t�H���g��fieldData�ɐݒ�
    fieldData.setFont(pointTextFont, pointTextFontBold, agentIDTextFont);
    
    /*------���[�v�i�E�B���h�E��������܂Łj------*/
    while (System::Update()) {
        // �t�B�[���h�̕`��
        fieldData.draw();
        
        // ���_�̕`��
        printTotalPoints(Team::ENEMY, !allyTeamColor, fieldData, font16, font16Bold, font28, totalPointsTextFont);
        printTotalPoints(Team::ALLY, allyTeamColor, fieldData, font16, font16Bold, font28, totalPointsTextFont);
    }
}


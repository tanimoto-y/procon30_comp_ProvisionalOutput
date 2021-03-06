#include "field.hpp"
#include "button.hpp"
#include "check_box.hpp"

using namespace std;

/**
 AgentActNumbers::getAgentActNumber:
    座標をもとに、エージェントの行動の番号を返す
 
 @param position                移動先の座標（pair<int,int>型）
 @param beforePosition          移動前の座標（pair<int,int>型）
 @param argIsAgentRemovingTiles タイルの除去ならtrue, 除去でなければfalse
 @return                        行動の番号
 */
int AgentActNumbers::getAgentActNumber(pair<int, int> position, pair<int, int> beforePosition, bool argIsAgentRemovingTiles) {
    pair movedPosition = {position.first-beforePosition.first, position.second-beforePosition.second};
    int actNumber = -1;
    
    // 返す行動番号の取得
    if (movedPosition.second == -1) {
        if (movedPosition.first == -1) {
            if (argIsAgentRemovingTiles) {
                actNumber = AgentActNumbers::REMOVE_UP_LEFT;      // 左上を除去
            }
            else {
                actNumber = AgentActNumbers::GO_UP_LEFT;          // 左上へ移動
            }
        }
        else if (movedPosition.first == 0) {
            if (argIsAgentRemovingTiles) {
                actNumber = AgentActNumbers::REMOVE_UP;           // 上を除去
            }
            else {
                actNumber = AgentActNumbers::GO_UP;               // 上へ移動
            }
        }
        else if (movedPosition.first == 1) {
            if (argIsAgentRemovingTiles) {
                actNumber = AgentActNumbers::REMOVE_UP_RIGHT;     // 右上を除去
            }
            else {
                actNumber = AgentActNumbers::GO_UP_RIGHT;         // 右上へ移動
            }
        }
    }
    else if (movedPosition.second == 0) {
        if (movedPosition.first == -1) {
            if (argIsAgentRemovingTiles) {
                actNumber = AgentActNumbers::REMOVE_LEFT;         // 左を除去
            }
            else {
                actNumber = AgentActNumbers::GO_LEFT;             // 左へ移動
            }
        }
        else if (movedPosition.first == 1) {
            if (argIsAgentRemovingTiles) {
                actNumber = AgentActNumbers::REMOVE_RIGHT;        // 左上を除去
            }
            else {
                actNumber = AgentActNumbers::GO_RIGHT;            // 左上へ移動
            }
        }
        else  {
            actNumber = AgentActNumbers::STAY;                    // とどまる
        }
    }
    else {
        if (movedPosition.first == -1) {
            if (argIsAgentRemovingTiles) {
                actNumber = AgentActNumbers::REMOVE_DOWN_LEFT;    // 左下を除去
            }
            else {
                actNumber = AgentActNumbers::GO_DOWN_LEFT;        // 左下へ移動
            }
        }
        else if (movedPosition.first == 0) {
            if (argIsAgentRemovingTiles) {
                actNumber = AgentActNumbers::REMOVE_DOWN;         // 下を除去
            }
            else {
                actNumber = AgentActNumbers::GO_DOWN;             // 下へ移動
            }
        }
        else if (movedPosition.first == 1) {
            if (argIsAgentRemovingTiles) {
                actNumber = AgentActNumbers::REMOVE_DOWN_RIGHT;   // 右下を除去
            }
            else {
                actNumber = AgentActNumbers::GO_DOWN_RIGHT;       // 右下へ移動
            }
        }
    }
    
    return actNumber;
}

pair<int, int> AgentActNumbers::getAgentActDirection(int argActNumber) {
    pair<int, int> movedPosition;
    
    if (argActNumber == AgentActNumbers::STAY) {
        movedPosition = {0, 0};
    }
    else if (argActNumber == AgentActNumbers::GO_UP_LEFT    || argActNumber == AgentActNumbers::REMOVE_UP_LEFT) {
        movedPosition = {-1, -1};
    }
    else if (argActNumber == AgentActNumbers::GO_UP         || argActNumber == AgentActNumbers::REMOVE_UP) {
        movedPosition = {0, -1};
    }
    else if (argActNumber == AgentActNumbers::GO_UP_RIGHT   || argActNumber == AgentActNumbers::REMOVE_UP_RIGHT) {
        movedPosition = {1, -1};
    }
    else if (argActNumber == AgentActNumbers::GO_RIGHT      || argActNumber == AgentActNumbers::REMOVE_RIGHT) {
        movedPosition = {1, 0};
    }
    else if (argActNumber == AgentActNumbers::GO_DOWN_RIGHT || argActNumber == AgentActNumbers::REMOVE_DOWN_RIGHT) {
        movedPosition = {1, 1};
    }
    else if (argActNumber == AgentActNumbers::GO_DOWN       || argActNumber == AgentActNumbers::REMOVE_DOWN) {
        movedPosition = {0, 1};
    }
    else if (argActNumber == AgentActNumbers::GO_DOWN_LEFT  || argActNumber == AgentActNumbers::REMOVE_DOWN_LEFT) {
        movedPosition = {-1, 1};
    }
    else if (argActNumber == AgentActNumbers::GO_LEFT       || argActNumber == AgentActNumbers::REMOVE_LEFT) {
        movedPosition = {-1, 0};
    }
    
    return movedPosition;
}

/**
 centerText:
 引数で指定した座標の中央に文字を表示する関数。
 文字列(String)型用
 
 @param argStr          表示する文字列
 @param argLeftFRONTX     左上のx座標
 @param argLeftFRONTY     左上のy座標
 @param argRightBottomX 右下のx座標
 @param argRightBottomY 右下のy座標
 @param argFont         表示に使うフォントのポインタ
 @param argColor        文字の色
 */
void centerText(const String argStr, const int argLeftFRONTX, const int argLeftFRONTY, const int argRightBottomX, const int argRightBottomY, const Font& argFont, const Color argColor) {
    unsigned long int textLength = argStr.length();
    
    argFont(argStr).draw(argLeftFRONTX+(argRightBottomX-argLeftFRONTX)/2-textLength*argFont.fontSize()/3, argLeftFRONTY+(argRightBottomY-argLeftFRONTY)/2-argFont.fontSize()/2, argColor);
}

/**
 centerText:
 引数で指定した座標の中央に文字を表示する関数。
 数値(int)型用
 
 @param argInt          表示する数値
 @param argLeftFRONTX     左上のx座標
 @param argLeftFRONTY     左上のy座標
 @param argRightBottomX 右下のx座標
 @param argRightBottomY 右下のy座標
 @param argFont         表示に使うフォントのポインタ
 @param argColor        文字の色
 */
void centerText(const int argInt, const int argLeftFRONTX, const int argLeftFRONTY, const int argRightBottomX, const int argRightBottomY, const Font& argFont, const Color argColor) {
    unsigned long int textLength = to_string(argInt).length();
    
    argFont(argInt).draw(argLeftFRONTX+(argRightBottomX-argLeftFRONTX)/2-textLength*argFont.fontSize()/3, argLeftFRONTY+(argRightBottomY-argLeftFRONTY)/2-argFont.fontSize()/2, argColor);
}

/**
 printTotalPoints:
 得点と概要の表示
 
 @param argTeam                 どちらのチームを表示するか（Team::ALLY or Team::ENEMY）
 @param argTeamColor            チームの色（TeamColor::RED or TeamColor::BLUE）
 @param argFieldData            フィールドクラスのポインタ
 @param argFont16               サイズ16のフォントのポインタ
 @param argFont16Bold           サイズ16の太字フォントのポインタ
 @param argFont28               サイズ28のフォントのポインタ
 @param argTotalPointsTextFont  サイズ28の太字フォントのポインタ
 */
void printTotalPoints(const Team::Type argTeam, const bool argTeamColor, Field& argFieldData, const Font& argFont16, const Font& argFont16Bold, const Font& argFont28, const Font& argTotalPointsTextFont) {
    // 塗りつぶす色（=チームの色）の設定
    Color rectColor;
    if (argTeamColor == TeamColor::BLUE) {
        rectColor = Color(0, 100, 200);
    }
    else {
        rectColor = Color(200, 10, 0);
    }
    
    String statusStr;
    
    // 表示内容が味方の情報の場合
    if (argTeam == Team::ALLY) {
        // 背景色
        Rect(WINDOW_SIZE_W-290+100, 50, WINDOW_SIZE_W-290-100, 300).draw(rectColor);
        
        // "Ally"と表示
        argFont28(U"Ally").draw(WINDOW_SIZE_W-290+165, 60, Color(Palette::White));
        
        // 総得点を表示
        centerText(argFieldData.getTotalPoints(Team::ALLY), WINDOW_SIZE_W-190, 80, WINDOW_SIZE_W, 150, argTotalPointsTextFont, Color(Palette::White));
        
        // タイルポイントの表示
        argFont16(U"タイル").draw(WINDOW_SIZE_W-290+150, 200, Color(Palette::White));
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
        
        // 領域ポイントの表示
        argFont16(U"領域").draw(WINDOW_SIZE_W-290+150+7, 230, Color(Palette::White));
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
         argFont18(U"点差").draw(WINDOW_SIZE_W-290+150, 200, Color(Palette::White));
         argFont18Bold(argFieldData.getTotalPoints(Team::ALLY)-argFieldData.getTotalPoints(Team::ENEMY)).draw(WINDOW_SIZE_W-290+200, 200, Color(Palette::White));
         argFont18(U"状況").draw(WINDOW_SIZE_W-290+150, 230, Color(Palette::White));
         if (argFieldData.getTotalPoints(Team::ALLY)-argFieldData.getTotalPoints(Team::ENEMY) > 0) {
         statusStr = U"勝ち";
         }
         else if (argFieldData.getTotalPoints(Team::ALLY)-argFieldData.getTotalPoints(Team::ENEMY) < 0) {
         statusStr = U"負け";
         }
         else {
         statusStr = U"引き分け";
         }
         */
        
        //argFont16Bold(statusStr).draw(WINDOW_SIZE_W-290+200, 230, Color(Palette::White));
    }
    // 表示内容が相手の情報の場合
    else if (argTeam == Team::ENEMY) {
        // 背景色
        Rect(0, 50, 290-100, 300).draw(rectColor);
        
        // "Enemy"と表示
        argFont28(U"Enemy").draw(50, 60, Color(Palette::White));
        
        // 総得点を表示
        centerText(argFieldData.getTotalPoints(Team::ENEMY), 0, 80, 190, 150, argTotalPointsTextFont, Color(Palette::White));
        
        // タイルポイントの表示
        argFont16(U"タイル").draw(50, 200, Color(Palette::White));
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
        
        // 領域ポイントの表示
        argFont16(U"領域").draw(50+7, 230, Color(Palette::White));
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
         argFont16(U"点差").draw(50, 200, Color(Palette::White));
         argFont16Bold(argFieldData.getTotalPoints(Team::ENEMY)-argFieldData.getTotalPoints(Team::ALLY)).draw(100, 200, Color(Palette::White));
         argFont16(U"状況").draw(50, 230, Color(Palette::White));
         if (argFieldData.getTotalPoints(Team::ENEMY)-argFieldData.getTotalPoints(Team::ALLY) > 0) {
         statusStr = U"勝ち";
         }
         else if (argFieldData.getTotalPoints(Team::ENEMY)-argFieldData.getTotalPoints(Team::ALLY) < 0) {
         statusStr = U"負け";
         }
         else {
         statusStr = U"引き分け";
         }
         argFont16Bold(statusStr).draw(100, 230, Color(Palette::White));
         */
    }
}

/**
 Main関数
 */
void Main() {
    /*----------ウィンドウの大きさ設定--------------*/
    Window::Resize(WINDOW_SIZE_W, WINDOW_SIZE_H);
    
    /*-----------------背景色--------------------*/
    Graphics::SetBackground(Color(Palette::White));
    
    /*-------------その他の変数の宣言--------------*/
    int fieldSizeW = 10;        // フィールドの横幅
    int fieldSizeH = 10;        // フィールドの縦幅
    
    /*--------------フィールドの作成--------------*/
    // フィールドの得点
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
     
     // フィールドの状況（1: 味方のタイル -1: 相手のタイル 0: どちらでもない）
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
    
    // フィールドの状況（1: 味方のタイル -1: 相手のタイル 0: どちらでもない）
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
    
    // 味方チームの色
    bool allyTeamColor = TeamColor::BLUE;       // 赤: TeamColor::RED 青: TeamColor::BLUE
    
    // フィールドの作成
    Field fieldData(fieldPointsArray, fieldStatusArray, Vec2(fieldSizeW, fieldSizeH), allyTeamColor);
    
    // フィールドからエージェントの位置を取得
    vector<pair<int, int>> allyAgentsPosition;
    vector<pair<int, int>> enemyAgentsPosition;
    fieldData.getAgentsPosition(allyAgentsPosition, enemyAgentsPosition);
    
    for (int i = 0; i < fieldData.getTotalTeamAgents(); i++) {
        cout << "Ally [" << i << "] (Agent ID: " << i+1 << ") : (" << allyAgentsPosition[i].first << ',' << allyAgentsPosition[i].second << ") | ";
        cout << "Enemy [" << i << "] (Agent ID: " << -(i+1) << ") : (" << enemyAgentsPosition[i].first << ',' << enemyAgentsPosition[i].second << ")" << endl;
    }
    
    /*---------------フォントの用意---------------*/
    // フィールド描画用
    const Font pointTextFont(fieldData.getFieldSquareSize()-POINT_TEXT_FONT_SIZE);
    const Font pointTextFontBold(fieldData.getFieldSquareSize()-POINT_TEXT_BIG_FONT_SIZE, Typeface::Bold);
    const Font agentIDTextFont(fieldData.getFieldSquareSize()-AGENT_ID_TEXT_FONT_SIZE);
    
    // 得点表示用
    const Font totalPointsTextFont(72);
    
    // その他
    const Font font28(28);
    const Font font16(16);
    const Font font16Bold(16, Typeface::Bold);
    
    // フォントをfieldDataに設定
    fieldData.setFont(pointTextFont, pointTextFontBold, agentIDTextFont);
    
    /*-----------------ターン関連----------------*/
    int countTurns = 1;
    
    /*-----------------ボタン関連----------------*/
    Button nextTurnButton(U"確定", Rect(50, WINDOW_SIZE_H-150, 180, 80));
    Button solverButton(U"探索", Rect(WINDOW_SIZE_W-50-180, WINDOW_SIZE_H-150, 180, 80));
    Color buttonBaseColor(255, 255, 255);
    Color buttonBaseColorUnderCursor(230, 230, 230);
    Color buttonFrameColor(0, 162, 232);
    
    /*-------------チェックボックス関連------------*/
    CheckBox solvingCheck(Rect(50, WINDOW_SIZE_H-60, 20, 20));
    
    /*------ループ（ウィンドウが閉じられるまで）------*/
    while (System::Update()) {
        // フィールドの描画
        fieldData.draw();
        
        // ターン数の描画
        font16(U"Turns : ").draw(10, 10, Color(Palette::Black));
        font16(countTurns).draw(80, 10, Color(Palette::Black));
        
        // 確定ボタン -> 押す: 次のターンへ
        nextTurnButton.draw(buttonBaseColor, buttonBaseColorUnderCursor, buttonFrameColor, font28);
        if (nextTurnButton.getStatus()) {
            fieldData.decision();
            countTurns ++;
        }
        
        // 探索ボタン -> 押す: 探索開始
        solverButton.draw(buttonBaseColor, buttonBaseColorUnderCursor, buttonFrameColor, font28);
        if (solverButton.getStatus()) {
            fieldData.startSolving();
        }
        
        // チェックボックス -> ✓: ターン変更と同時に探索
        solvingCheck.draw(buttonBaseColor, buttonBaseColorUnderCursor, buttonFrameColor, font16);
        font16(U"確定と同時に探索").draw(50+20+5, WINDOW_SIZE_H-60, Color(Palette::Black));
        fieldData.setSolvingWithNextTurn(solvingCheck.getStatus());
        
        
        // 得点の描画
        printTotalPoints(Team::ENEMY, !allyTeamColor, fieldData, font16, font16Bold, font28, totalPointsTextFont);
        printTotalPoints(Team::ALLY, allyTeamColor, fieldData, font16, font16Bold, font28, totalPointsTextFont);
    }
}


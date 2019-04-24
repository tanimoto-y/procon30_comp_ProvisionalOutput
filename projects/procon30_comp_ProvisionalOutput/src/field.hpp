#ifndef field_hpp
#define field_hpp

#pragma once

#include <Siv3D.hpp> // OpenSiv3D v0.3.1
#include <stdio.h>
#include "node.hpp"

using namespace std;

#define WINDOW_SIZE_W               1280    // ウィンドウの横幅
#define WINDOW_SIZE_H               720     // ウィンドウの縦幅

#define FIELD_LINE_MAX_LENGTH       700     // フィールドの枠の最大長
#define FIELD_SIZE_MAX              20      // フィールドの1列の最大のマスの数

#define POINT_TEXT_FONT_SIZE        40      // 点数表示用のフォントのサイズ（フィールドの1マスあたりの大きさから引く数値）
#define POINT_TEXT_BIG_FONT_SIZE    30      // 点数表示用の大きいフォントのサイズ（フィールドの1マスあたりの大きさから引く数値）
#define AGENT_ID_TEXT_FONT_SIZE     55      // エージェントID表示用のフォントのサイズ（フィールドの1マスあたりの大きさから引く数値）

#define DIRECTIONS                  9       // エージェントが移動できる方向の数（「とどまる」も含む）

// エージェントが移動できる座標
const Vec2 gMoveDirections[9] {
    {-1, -1}, { 0, -1}, { 1, -1},
    {-1,  0}, { 0,  0}, { 1,  0},
    {-1,  1}, { 0,  1}, { 1,  1}
};

// 周囲4方向（上下左右）の座標
const Vec2 gSearchTileDirections[4] = {
    {0, -1}, {1, 0}, {0, 1}, {-1, 0}
};

// チームの色
namespace TeamColor {
    enum Type {
        BLUE    = 0,
        RED     = 1,
    };
}

// チームを示す値
namespace Team {
    enum Type {
        ALLY    = 0,
        ENEMY   = 1,
    };
}

// タイルの状態を示す値
namespace TileStatus {
    enum Type {
        ALLY    =  1,
        NONE    =  0,
        ENEMY   = -1,
    };
}

// 領域の状態を示す値
namespace AreaStatus {
    enum Type {
        NONE    = 0,
        ALLY    = 1,
        ENEMY   = 2,
        BOTH    = 3,
    };
}

// フィールドクラス
class Field {
    // プライベートメンバ
    vector<vector<int>> mFieldPointsArray;              // フィールドの点数格納用
    vector<vector<int>> mFieldStatusArray;              // フィールドの状況格納用
    vector<vector<int>> mFieldAgentsIDArray;            // フィールド上のエージェントの場所の格納用
    vector<vector<bool>> mFieldAllyAreaSquaresArray;    // 味方の領域の格納用
    vector<vector<bool>> mFieldEnemyAreaSquaresArray;   // 相手の領域の格納用
    vector<vector<bool>> mFieldAreaSideLinesArray;      // 領域を成す辺の格納用
    
    int mAllyPoints  = 0;                           // 味方チームの合計得点
    int mEnemyPoints = 0;                           // 相手チームの合計得点
    
    int mAllyTilePoints  = 0;                       // 味方チームのタイルポイント
    int mEnemyTilePoints = 0;                       // 相手チームのタイルポイント
    
    int mAllyAreaPoints  = 0;                       // 相手チームの領域ポイント
    int mEnemyAreaPoints = 0;                       // 相手チームの領域ポイント
    
    int mFieldSizeW, mFieldSizeH;                   // フィールドの大きさ
    int mFieldLineLengthW, mFieldLineLengthH;       // フィールドの枠の長さ
    int mFieldSquareSize;                           // フィールドの1マスあたりの大きさ（正方形）
    
    int mTotalTeamAgents;                           // 各チームのエージェントの総数
    
    int mFieldLeftmostPoint;                        // フィールドの描画範囲の左端の座標
    int mFieldRightmostPoint;                       // フィールドの描画範囲の右端の座標
    int mFieldTopmostPoint;                         // フィールドの描画範囲の上端の座標
    int mFieldBottommostPoint;                      // フィールドの描画範囲の下端の座標
    
    bool mAllyTeamColor;                            // 味方チームの色（敵チームの色は!allyTeamColorで取得できる）
    
    Vec2 mCurrentSquarePosition = { -1, -1 };       // 選択されているマスの座標（選択されていなければ(-1, -1)）
    int mCurrentAgentID = 0;                        // 選択されているマスにいるエージェントのID（選択されていなければ 0）
    
    bool mMousePressing = false;                    // マウスが押されている間はtrue
    
    Font mPointTextFont;                            // 点数表示用のフォント
    Font mPointTextFontBold;                        // 点数表示用の太字フォント
    Font mAgentIDTextFont;                          // エージェントID表示用のフォント
    
    int mBigPointsBorder;                           // 大きい点数の基準（この値を超えると太字で点数を表示する）
    
public:
    // コンストラクタ（初期化も行う）
    Field(const vector<vector<int>> &argFieldPointsArray, const vector<vector<int>> &argFieldStatusArray, const Vec2 position, const bool argAllyTeamColor);
    
    // フィールドの設定
    void setField(const vector<vector<int>> &argFieldPointsArray, const vector<vector<int>> &argFieldStatusArray);
    
    // フィールドの1マスの大きさを返す
    int getFieldSquareSize();
    
    // 得点を返す
    int getTotalPoints(const Team::Type argTeam);     // 合計得点
    int getTilePoints(const Team::Type argTeam);      // タイルポイント
    int getAreaPoints(const Team::Type argTeam);      // 領域ポイント
    
    // Fieldクラスで使うフォントの設定
    void setFont(const Font argPointTextFont, const Font argPointTextFontBold, const Font argAgentIDTextFont);
    
    // マスの上にカーソルがあるか判断
    bool isCursorOnTheSquare(const Vec2 argPosition);
    
    // 領域ポイントの取得
    void searchAreaPointsSide(vector<vector<bool>> argFieldMark, const int argStartW, const int argStartH, const int argW, const int argH, Node* argParentNode);
    bool searchAreaPointsSquares(vector<vector<bool>>& argAreaSquares, const int argStartW, const int argStartH, const int argBeTileStatus, const bool argDeleteArea);
    void searchAreaPoints(const int argStartW, const int argStartH);
    
    // マスの塗りつぶし
    void fillSquare(const Vec2 argSquarePosition, const int argW, const int argH, Color& rectColor);
    
    // 領域の塗りつぶし
    void fillAreaSquare(const Vec2 argSquarePosition, const int argW, const int argH);
    
    // タイル点数の表示
    void printSquarePoint(const Vec2 argSquarePosition, const int argW, const int argH, Color& rectColor);
    
    // エージェントの行動
    void agentMovement(int argW, int argH, int argBeforeW, int argBeforeH);
    
    // 選択するエージェントの変更
    void setCurrentAgent(int argW, int argH);
    
    // マスの描画
    void drawSquares();
    
    // フィールドの描画
    void draw();
};

#endif /* field_hpp */

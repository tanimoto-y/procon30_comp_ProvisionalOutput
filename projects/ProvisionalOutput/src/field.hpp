#ifndef field_hpp
#define field_hpp

#pragma once

#include <Siv3D.hpp> // OpenSiv3D v0.3.1
#include <stdio.h>
#include "node.hpp"
#include "header.hpp"

using namespace std;

// フィールドクラス
class Field {
public:
    // コンストラクタ（初期化も行う）
    Field(const vector<vector<int>> &argFieldPointsArray, const vector<vector<int>> &argFieldStatusArray, const Vec2 position, const bool argAllyTeamColor);
    
    // フィールドの設定
    void setField(const vector<vector<int>> &argFieldPointsArray, const vector<vector<int>> &argFieldStatusArray);
    
    // 操作の確定（反映）
    void decision();
    
    // フィールドからエージェントの位置を取得
    // 要素番号はエージェントIDと同じで、相手のエージェントの場合はエージェントIDの絶対値が要素番号になる
    // 例: 相手の1人目のエージェント（エージェントID -1）のx座標 -> argEnemyAgentsPosition[1].first
    void getAgentsPosition(vector<pair<int, int>> &argAllyAgentsPosition, vector<pair<int, int>> &argEnemyAgentsPosition);
    
    // 各チームのエージェントの人数を返す
    int getTotalTeamAgents();
    
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
    void searchAreaPointsSide(vector<vector<bool>> argFieldMark, const int argStartX, const int argStartY, const int argX, const int argY, Node* argParentNode);
    bool searchAreaPointsSquares(vector<vector<bool>>& argAreaSquares, const int argStartX, const int argStartY, const int argBeTileStatus, const bool argDeleteArea);
    void searchAreaPoints(const int argStartX, const int argStartY);
    
    // マスの塗りつぶし
    void fillSquare(const Vec2 argSquarePosition, const int argX, const int argY, Color& rectColor);
    
    // 領域の塗りつぶし
    void fillAreaSquare(const Vec2 argSquarePosition, const int argX, const int argY);
    
    // タイル点数の表示
    void printSquarePoint(const Vec2 argSquarePosition, const int argX, const int argY, Color& rectColor);
    
    // エージェントの移動経路を示す矢印の表示
    void drawArrow(const Vec2 argSquarePosition, const int argBeforeX, const int argBeforeY, const int argX, const int argY, const bool argIsTileRemoved);
    
    // タイル除去
    void removeTile(const int argX, const int argY);
    
    // 領域を除去
    void removeArea(const int argX, const int argY, const int argRemoveTeam);
    
    // タイル設置
    void putTile(const int argX, const int argY, const int argTileStatus);
    
    // エージェントの行動
    int agentMovement(int argX, int argY, int argBeforeX, int argBeforeY, bool argRemoveTile);
    
    // 選択するエージェントの変更
    void setCurrentAgent(int argX, int argY);
    
    // マスの描画
    void drawSquares();
    
    // フィールドの描画
    void draw();
    
    // 探索部の起動
    void startSolving();
    
    // 自動探索（ターン切替時）
    void setSolvingWithNextTurn(bool argBool);
    
private:
    vector<vector<int>> mFieldPointsArray;              // フィールドの点数格納用
    /*
    vector<vector<int>> mFieldStatusArray;              // フィールドの状況格納用
    vector<vector<int>> mFieldAgentsIDArray;            // フィールド上のエージェントの場所の格納用
    vector<vector<bool>> mFieldAllyAreaSquaresArray;    // 味方の領域の格納用
    vector<vector<bool>> mFieldEnemyAreaSquaresArray;   // 相手の領域の格納用
    vector<vector<bool>> mFieldAreaSideLinesArray;      // 領域を成す辺の格納用
    */
    fieldDataStruct mFieldData;                         // フィールドの情報（点数, 状況, エージェントの場所, 領域, 領域を成す辺）
    
    vector<fieldDataStruct> mFieldDataHistory;   // このターン内のフィールドへの操作履歴
    
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
    
    bool mSolvedPerTurn;                            // このターン内で探索したかどうか（したならtrue）
    bool mSolvingWithChangeTurn;                    // ターン切り替えと同時に探索するかどうか（するならtrue）
};

#endif /* field_hpp */


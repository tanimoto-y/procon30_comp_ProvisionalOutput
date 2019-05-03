/*
 header.hpp
 
 定数、グローバル変数、構造体、名前空間の宣言
 */

#ifndef Header_h
#define Header_h

/*------------------- 定数 --------------------*/
#define WINDOW_SIZE_W               1280    // ウィンドウの横幅
#define WINDOW_SIZE_H               720     // ウィンドウの縦幅

#define FIELD_LINE_MAX_LENGTH       700     // フィールドの枠の最大長
#define FIELD_SIZE_MAX              20      // フィールドの1列の最大のマスの数

#define POINT_TEXT_FONT_SIZE        40      // 点数表示用のフォントのサイズ（フィールドの1マスあたりの大きさから引く数値）
#define POINT_TEXT_BIG_FONT_SIZE    30      // 点数表示用の大きいフォントのサイズ（フィールドの1マスあたりの大きさから引く数値）
#define AGENT_ID_TEXT_FONT_SIZE     55      // エージェントID表示用のフォントのサイズ（フィールドの1マスあたりの大きさから引く数値）

#define DIRECTIONS                  9       // エージェントが移動できる方向の数（「とどまる」も含む）

/*---------------- グローバル定数 ----------------*/
// エージェントが移動できる座標
// 時計回り
const Vec2 gMoveDirections[9] {
    {-1, -1}, { 0, -1}, { 1, -1}, { 1,  0}, { 1,  1}, { 0,  1}, {-1,  1}, {-1,  0}, { 0,  0}
};

// 周囲4方向（上下左右）の座標
const Vec2 gSearchTileDirections[4] = {
    {0, -1}, {1, 0}, {0, 1}, {-1, 0}
};

/*-------------------- 構造体 --------------------*/
/**
 Score:
 得点の格納用の構造体。
 
 @var tile   タイルポイント
 @var area   領域ポイント
 @function total 合計点（tile+area）を返す関数
 */
typedef struct Score {
    int tile = 0;
    int area = 0;
    
    int total() {
        return tile + area;
    }
} Score;

/**
 Score:
 フィールドの情報格納用の構造体。
 
 @var fieldPointsArray              フィールドの点数格納用
 @var fieldStatusArray              フィールドの状況格納用
 @var fieldAgentsIDArray            フィールド上のエージェントの場所の格納用
 @var fieldAllyAreaSquaresArray     味方の領域の格納用
 @var fieldEnemyAreaSquaresArray    相手の領域の格納用
 @var fieldAreaSideLinesArray       領域を成す辺の格納用
 */
typedef struct fieldDataStruct {
    vector<vector<int>> fieldPointsArray;              // フィールドの点数格納用
    vector<vector<int>> fieldStatusArray;              // フィールドの状況格納用
    vector<vector<int>> fieldAgentsIDArray;            // フィールド上のエージェントの場所の格納用
    vector<vector<bool>> fieldAllyAreaSquaresArray;    // 味方の領域の格納用
    vector<vector<bool>> fieldEnemyAreaSquaresArray;   // 相手の領域の格納用
    vector<vector<bool>> fieldAreaSideLinesArray;      // 領域を成す辺の格納用
} fieldDataStruct;

/*-------------------- 名前空間 --------------------*/
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

// エージェントの行動の番号
namespace AgentActNumbers {
    enum Type {
        STAY                = 0,    // 動きなし
        
        GO_UP_LEFT          = 1,    // 移動: 左上
        GO_UP               = 2,    // 移動: 上
        GO_UP_RIGHT         = 3,    // 移動: 右上
        GO_RIGHT            = 4,    // 移動: 右
        GO_DOWN_RIGHT       = 5,    // 移動: 右下
        GO_DOWN             = 6,    // 移動: 下
        GO_DOWN_LEFT        = 7,    // 移動: 左下
        GO_LEFT             = 8,    // 移動: 左
        
        REMOVE_UP_LEFT      = 9,    // 除去: 左上
        REMOVE_UP           = 10,   // 除去: 上
        REMOVE_UP_RIGHT     = 11,   // 除去: 右上
        REMOVE_RIGHT        = 12,   // 除去: 右
        REMOVE_DOWN_RIGHT   = 13,   // 除去: 右下
        REMOVE_DOWN         = 14,   // 除去: 下
        REMOVE_DOWN_LEFT    = 15,   // 除去: 左下
        REMOVE_LEFT         = 16,   // 除去: 左
    };
}


#endif

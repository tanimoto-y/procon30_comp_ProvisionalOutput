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

/*---------------- グローバル変数 ----------------*/
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

/*-------------------- 構造体 --------------------*/
// 得点の格納用の構造体
typedef struct Score {
    int tile = 0;
    int area = 0;
    
    int total() {
        return tile + area;
    }
} Score;

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

// 領域の状態を示す値
namespace AreaStatus {
    enum Type {
        NONE    = 0,
        ALLY    = 1,
        ENEMY   = 2,
        BOTH    = 3,
    };
}

// エージェントの行動の番号
namespace AgentActNumbers {
    enum Type {
        NONE                = 0,    // 動きなし
        
        GOTO_LEFT_TOP       = 1,    // 移動: 左上
        GOTO_TOP            = 2,    // 移動: 上
        GOTO_RIGHT_TOP      = 3,    // 移動: 右上
        GOTO_RIGHT          = 4,    // 移動: 右
        GOTO_RIGHT_BOTTOM   = 5,    // 移動: 右下
        GOTO_BOTTOM         = 6,    // 移動: 下
        GOTO_LEFT_BOTTOM    = 7,    // 移動: 左下
        GOTO_LEFT           = 8,    // 移動: 左
        
        REMOVE_LEFT_TOP     = 9,    // 除去: 左上
        REMOVE_TOP          = 10,   // 除去: 上
        REMOVE_RIGHT_TOP    = 11,   // 除去: 右上
        REMOVE_RIGHT        = 12,   // 除去: 右
        REMOVE_RIGHT_BOTTOM = 13,   // 除去: 右下
        REMOVE_BOTTOM       = 14,   // 除去: 下
        REMOVE_LEFT_BOTTOM  = 15,   // 除去: 左下
        REMOVE_LEFT         = 16,   // 除去: 左
    };
}


#endif

#include "field.hpp"

// コンストラクタ（初期化も行う）
/**
 Field::Field
 Fieldクラスのコンストラクタ（初期化も行う）。
 
 @param argFieldPointsArray フィールドの得点の配列
 @param argFieldStatusArray フィールドの状態を示す配列
 @param position            フィールドの大きさを示す配列（例：{10, 10}）
 @param argAllyTeamColor    味方チームの色（TeamColor::RED or TeamColor::BLUE）
 */
Field::Field(const vector<vector<int>> &argFieldPointsArray, const vector<vector<int>> &argFieldStatusArray, const Vec2 position, const bool argAllyTeamColor) {
    // フィールドの大きさの設定
    mFieldSizeW = position.x;
    mFieldSizeH = position.y;
    
    // フィールドの得点、状況とエージェントIDを設定
    setField(argFieldPointsArray, argFieldStatusArray);
    
    // 味方チームの色
    mAllyTeamColor = argAllyTeamColor;
    
    // フィールドの枠の長さの決定
    if (mFieldSizeW > mFieldSizeH) {
        mFieldSquareSize = FIELD_LINE_MAX_LENGTH/mFieldSizeW;
        
        mFieldLineLengthW = mFieldSquareSize * mFieldSizeW;
        mFieldLineLengthH = mFieldSquareSize * mFieldSizeH;
    }
    else if (mFieldSizeW < mFieldSizeH) {
        mFieldSquareSize = FIELD_LINE_MAX_LENGTH/mFieldSizeH;
        
        mFieldLineLengthW = mFieldSquareSize * mFieldSizeW;
        mFieldLineLengthH = mFieldSquareSize * mFieldSizeH;
    }
    else {
        mFieldSquareSize = FIELD_LINE_MAX_LENGTH/mFieldSizeH;
        
        mFieldLineLengthW = mFieldSquareSize * mFieldSizeW;
        mFieldLineLengthH = mFieldSquareSize * mFieldSizeH;
    }
    
    // フィールドの縦線・横線の描画
    mFieldLeftmostPoint      = WINDOW_SIZE_W/2-mFieldLineLengthW/2;
    mFieldTopmostPoint       = WINDOW_SIZE_H/2-mFieldLineLengthH/2;
    mFieldRightmostPoint     = WINDOW_SIZE_W/2+mFieldLineLengthW/2;
    mFieldBottommostPoint    = WINDOW_SIZE_H/2+mFieldLineLengthH/2;
}

/**
 Field::setField
 フィールドの設定を行う。
 フィールドの得点、状況をプライベート配列にコピーし、エージェントにIDを振り分ける。
 IDはフィールドの左上から順に振り分けられ、味方のエージェントのIDは1から上がっていき、相手のエージェントのIDは-1から下がっていく。
 この関数は、基本的にFieldコンストラクタから呼び出される。
 例）エージェントが各チーム3人のとき、味方のエージェントIDは1〜3、相手のエージェントIDは-1〜-3
 
 @param argFieldPointsArray フィールドの得点の配列
 @param argFieldStatusArray フィールドの状態を示す配列
 */
void Field::setField(const vector<vector<int>> &argFieldPointsArray, const vector<vector<int>> &argFieldStatusArray) {
    mFieldData.fieldPointsArray = argFieldPointsArray;    // フィールドの得点
    mFieldData.fieldStatusArray = argFieldStatusArray;    // フィールドの状況
    
    mFieldData.allyAgentsActNumbers.resize(mTotalTeamAgents, AgentActNumbers::STAY);  // 味方エージェントの行動番号の初期化
    mFieldData.enemyAgentsActNumbers.resize(mTotalTeamAgents, AgentActNumbers::STAY); // 相手エージェントの行動番号の初期化
    
    mFieldData.allyAgentsPosition.resize(mTotalTeamAgents);   // 味方エージェントの位置の初期化
    mFieldData.enemyAgentsPosition.resize(mTotalTeamAgents);  // 相手エージェントの位置の初期化
    
    // 得点の初期化
    mFieldData.allyScore  = Score{0, 0};
    mFieldData.enemyScore = Score{0, 0};
    
    int maxPoint = -16, minPoint = 16;
    int allyAgentsCount = 0, enemyAgentsCount = 0;
    
    for (int y = 0; y < mFieldSizeH; y++) {
        mFieldData.fieldAgentsIDArray.push_back(vector<int>(mFieldSizeW, 0));
        
        mFieldData.fieldAllyAreaSquaresArray.push_back(vector<bool>(mFieldSizeW, false));
        mFieldData.fieldEnemyAreaSquaresArray.push_back(vector<bool>(mFieldSizeW, false));
        
        mFieldData.fieldAreaSideLinesArray.push_back(vector<bool>(mFieldSizeW, false));
        
        for (int x = 0; x < mFieldSizeW; x++) {
            if (mFieldData.fieldStatusArray[y][x] == 1) {
                allyAgentsCount ++;
                mFieldData.fieldAgentsIDArray[y][x] = allyAgentsCount;    // エージェントにIDを振り分ける
                mFieldData.allyAgentsPosition[allyAgentsCount-1] = pair{x, y};
                
                mFieldData.allyScore.tile += mFieldData.fieldPointsArray[y][x];     // 初期段階の合計得点の算出
            }
            else if (mFieldData.fieldStatusArray[y][x] == -1) {
                enemyAgentsCount ++;
                mFieldData.fieldAgentsIDArray[y][x] = -enemyAgentsCount;  // エージェントにIDを振り分ける
                mFieldData.enemyAgentsPosition[enemyAgentsCount-1] = pair{x, y};
                
                mFieldData.enemyScore.tile += mFieldData.fieldPointsArray[y][x];    // 初期段階の合計得点の算出
            }
            
            // 最大値と最小値の検出
            if (mFieldData.fieldPointsArray[y][x] > maxPoint) {
                maxPoint = mFieldData.fieldPointsArray[y][x];
            }
            if (mFieldData.fieldPointsArray[y][x] < minPoint) {
                minPoint = mFieldData.fieldPointsArray[y][x];
            }
        }
    }
    
    // 大きい点数の基準を決める（最大値-最小値の上位20％）
    mBigPointsBorder = minPoint+(maxPoint-minPoint)*0.8;
    
    // 合計エージェント数
    mTotalTeamAgents = allyAgentsCount;
    
    // 履歴には初期状態を代入
    mFieldDataHistory.push_back(mFieldData);
}

void Field::decision() {
    // 履歴の最後からmFieldDataに反映
    mFieldData = mFieldDataHistory.back();
    
    // 履歴をリセットし、履歴の最初にmFieldDataと同じ情報を代入
    mFieldDataHistory.erase(mFieldDataHistory.begin(), mFieldDataHistory.end());
    mFieldDataHistory.shrink_to_fit();
    mFieldDataHistory.push_back(mFieldData);
    
    // 探索をリセット
    mSolvedPerTurn = false;
    
    // mSolvingWithChangeTurn == trueなら、探索も行う
    if (mSolvingWithChangeTurn) {
        startSolving();
    }
    
    cout << "------------------" << endl;
}

/**
 Field::getAgentsPosition:
 フィールドからエージェントの位置を取得する。
 要素番号はエージェントIDと同じで、相手のエージェントの場合はエージェントIDの絶対値が要素番号になる。
 例: 相手の1人目のエージェント（エージェントID -1）のx座標 -> argEnemyAgentsPosition[1].first
 各配列は参照渡しであるため、この関数を実行することで自動的に座標が代入される。

 @param argAllyAgentsPosition  味方のエージェントの座標配列
 @param argEnemyAgentsPosition 相手のエージェントの座標配列
 */
void Field::getAgentsPosition(vector<pair<int, int>> &argAllyAgentsPosition, vector<pair<int, int>> &argEnemyAgentsPosition) {
    int countAllyAgents = 0, countEnemyAgents = 0;
    
    argAllyAgentsPosition.resize(mTotalTeamAgents);
    argEnemyAgentsPosition.resize(mTotalTeamAgents);
    
    for (int y = 0; y < mFieldSizeH; y++) {
        for (int x = 0; x < mFieldSizeW; x++) {
            // 味方のエージェント
            if (mFieldDataHistory.back().fieldAgentsIDArray[y][x] > 0) {
                argAllyAgentsPosition[countAllyAgents] = {x, y};
                countAllyAgents ++;
            }
            // 相手のエージェント
            if (mFieldDataHistory.back().fieldAgentsIDArray[y][x] < 0) {
                argEnemyAgentsPosition[countEnemyAgents] = {x, y};
                countEnemyAgents ++;
            }
        }
    }
}

/**
 Field::getTotalTeamAgents:
 各チームのエージェントの人数を返す

 @return 各チームのエージェントの人数
 */
int Field::getTotalTeamAgents() {
    return mTotalTeamAgents;
}

/**
 Field::getFieldSquareSize:
 フィールドの1マスの大きさ（mFieldSquareSize）を返す。
 
 @return mFieldSquareSize（フィールドの1マスの大きさ）
 */
int Field::getFieldSquareSize() {
    return mFieldSquareSize;
}

/**
 Field::getTotalPoints:
 合計得点(mFieldDataHistory.back().allyScore.total, mFieldDataHistory.back().enemyScore.total)を返す。
 
 @param argTeam 得点を取得したいチームの番号(味方 Team::ALLY, 相手 Team::ENEMY)
 @return 合計得点
 */
int Field::getTotalPoints(const Team::Type argTeam) {
    if (argTeam == Team::ALLY) {    // 味方
        return mFieldDataHistory.back().allyScore.total();
    }
    else {                          // 敵
        return mFieldDataHistory.back().enemyScore.total();
    }
}

/**
 Field::getTilePoints:
 タイルポイント(mFieldDataHistory.back().allyScore.tile, mFieldDataHistory.back().enemyScore.tile)を返す。
 
 @param argTeam 得点を取得したいチームの番号(味方 Team::ALLY, 相手 Team::ENEMY)
 @return タイルポイント
 */
int Field::getTilePoints(const Team::Type argTeam) {
    if (argTeam == Team::ALLY) {    // 味方
        return mFieldDataHistory.back().allyScore.tile;
    }
    else {                          // 敵
        return mFieldDataHistory.back().enemyScore.tile;
    }
}

/**
 Field::getAreaPoints:
 領域ポイント(mFieldDataHistory.back().allyScore.area, mFieldDataHistory.back().enemyScore.area)を返す。
 
 @param argTeam 得点を取得したいチームの番号(味方 Team::ALLY, 相手 Team::ENEMY)
 @return 領域ポイント
 */
int Field::getAreaPoints(const Team::Type argTeam) {
    if (argTeam == Team::ALLY) {    // 味方
        return mFieldDataHistory.back().allyScore.area;
    }
    else {                          // 敵
        return mFieldDataHistory.back().enemyScore.area;
    }
}

/**
 Field::setFont:
 Fieldクラスで使うフォントの設定
 
 @param argPointTextFont¥       普通のフォント
 @param argPointTextFontBold    太字のフォント
 @param argAgentIDTextFont      エージェントのIDを表示する時に使うフォント
 */
void Field::setFont(const Font argPointTextFont, const Font argPointTextFontBold, const Font argAgentIDTextFont) {
    mPointTextFont = argPointTextFont;
    mPointTextFontBold = argPointTextFontBold;
    mAgentIDTextFont = argAgentIDTextFont;
}

/**
 Field::isCursorOnTheSquare
 マスの上にカーソルがあるか判断
 
 @param argPosition 検知するマスの座標（Vec2型, 例: {0, 0}）
 @return カーソルがあればtrue, なければfalse
 */
bool Field::isCursorOnTheSquare(const Vec2 argPosition) {
    if (Cursor::Pos().x > argPosition.x && Cursor::Pos().y > argPosition.y
        && Cursor::Pos().x < argPosition.x+mFieldSquareSize && Cursor::Pos().y < argPosition.y+mFieldSquareSize) {
        return true;        // あればtrue
    }
    else {
        return false;       // なければfalse
    }
}

/**
 Field::searchAreaPointsSide:
 *再帰関数
 フィールド上に領域が存在するかどうかを判断し、存在したら領域ポイントを設定する。
 引数で指定した座標のタイルから同じ色のタイルをたどっていき、同じタイルを二度通ることなく最初のタイルまで戻ってこられたら、フィールド上に領域が存在すると仮定する。
 その後、領域と仮定したタイルの間に、相手チームあるいは白いタイルが存在するか調査し、存在したら領域として認定する。
 領域の存在が認められたら、searchAreaPointsSquaresで領域内のマスに領域内であることを設定する。
 
 @param argFieldMark    フィールドの領域に認定したかどうか（一時的な配列）
 @param argStartX       領域探索の始点のマスのx座標
 @param argStartY       領域探索の始点のマスのy座標
 @param argX            現在探索中のマスのx座標
 @param argY            現在探索中のマスのy座標
 @param argParentNode   親ノード(Nodeクラス)のポインタ
 */
void Field::searchAreaPointsSide(vector<vector<bool>> argFieldMark, const int argStartX, const int argStartY, const int argX, const int argY, Node* argParentNode) {
    int x, y;
    int nodes = 0;
    Node node(argX, argY, argParentNode);
    
    if (argParentNode != nullptr) {
        argParentNode->setChildNode(&node);
    }
    
    // どちらのチームが対象か
    TileStatus::Type team;
    if (mFieldDataHistory.back().fieldStatusArray[argStartY][argStartX] == TileStatus::ALLY) {
        team = TileStatus::ALLY;
    }
    else {
        team = TileStatus::ENEMY;
    }
    
    for (int i = 0; i < DIRECTIONS; i++) {
        x = argX;
        y = argY;
        
        // 「とどまる」は除く
        if (gMoveDirections[i].x == 0 && gMoveDirections[i].y == 0) {
            continue;
        }
        
        // 対象となるマスをずらす
        // フィールドから出たらcontinue
        if (x+gMoveDirections[i].x < 0 || y+gMoveDirections[i].y < 0 || x+gMoveDirections[i].x > mFieldSizeW-1 || y+gMoveDirections[i].y > mFieldSizeH-1) {
            continue;
        }
        
        x += gMoveDirections[i].x;
        y += gMoveDirections[i].y;
        
        // 周囲に自チームのものではないタイルがなければcontinue
        bool areThereOthersTiles = false;
        for (int j = 0; j < 4; j++) {
            if (x+gSearchTileDirections[j].x >= 0 && y+gSearchTileDirections[j].y >= 0 &&
                x+gSearchTileDirections[j].x <= mFieldSizeW-1 && y+gSearchTileDirections[j].y <= mFieldSizeH-1) {
                if (mFieldDataHistory.back().fieldStatusArray[y+gSearchTileDirections[j].y][x+gSearchTileDirections[j].x] != team) {
                    areThereOthersTiles = true;
                    break;
                }
            }
        }
        if (!areThereOthersTiles) {
            continue;
        }
        
        // 違う色のマスならcontinue
        if (mFieldDataHistory.back().fieldStatusArray[y][x] != team) {
            continue;
        }
        
        //　親ノードと同じ位置ならcontinue
        bool cont = false;
        if (argParentNode != nullptr) {
            Node* parentNode = &node;
            while(1) {
                if (parentNode->getParentNode() == nullptr) {
                    break;
                }
                
                parentNode = parentNode->getParentNode();
                
                if (parentNode->getPosition().x == x && parentNode->getPosition().y == y) {
                    if ((x == argStartX && y == argStartY && !argFieldMark[argStartY][argStartX]) ||
                        (x != argStartX && y != argStartY)) {
                        cont = true;
                        break;
                    }
                }
            }
        }
        if (cont) {
            continue;
        }
        
        // すでに通ったマスの場合:
        //  -> 始点のマスなら囲いを検知する
        //  -> そうでなければcontinue
        if (argFieldMark[y][x]) {
            if (argFieldMark[argStartY][argStartX] && x == argStartX && y == argStartY && node.getTreeHigh() >= 3) {
                // マスを囲う辺がどの方向にあるか調査する
                // 現在のノードから親ノードをたどっていく
                Node* searchingNode = &node;
                int whiteTiles = 0;
                
                while(1) {
                    if (searchingNode->getParentNode() == nullptr) {
                        break;
                    }
                    
                    searchingNode = searchingNode->getParentNode();
                    // このタイルから親ノードのタイルの間に自分のチームではないタイルがあるか確認
                    // このタイルと親ノードのタイルを直線で結び、その間に自チームのタイルではないマス（=相手のタイルまたは白いマス）があるか確認
                    // あれば領域あり、なければ領域なし
                    
                    // 直線の係数を算出
                    int slope;
                    if (searchingNode->getPosition().x == x || searchingNode->getPosition().y == y) {
                        slope = 0;      // 傾きがなければ0
                    }
                    else {
                        slope = (searchingNode->getPosition().y - y) / (searchingNode->getPosition().x - x);
                    }
                    
                    // 直線の向きを指定
                    int directionX, directionY;
                    if (searchingNode->getPosition().x > x) {
                        directionX = -1;            // 親ノードのタイルx座標 > このタイルのx座標
                    }
                    else if (searchingNode->getPosition().x < x) {
                        directionX = 1;             // 親ノードのタイルx座標 < このタイルのx座標
                    }
                    else {
                        directionX = 0;             // 親ノードのタイルx座標 == このタイルのx座標
                    }
                    
                    if (searchingNode->getPosition().y > y) {
                        directionY = -1;            // 親ノードのタイルy座標 > このタイルのy座標
                    }
                    else if (searchingNode->getPosition().y < y) {
                        directionY = 1;             // 親ノードのタイルy座標 < このタイルのy座標
                    }
                    else {
                        directionY = 0;             // 親ノードのタイルy座標 == このタイルのy座標
                    }
                    
                    // 直線の始点 = 親ノードのタイル
                    int lineX = searchingNode->getPosition().x, lineY = searchingNode->getPosition().y;
                    
                    whiteTiles = 0;
                    int lineXBefore, lineYBefore;
                    
                    // 直線をたどっていき、その間に自チームのタイルがないマスがいくつあるか数える
                    while(1) {
                        lineXBefore = lineX;
                        lineYBefore = lineY;
                        
                        if (directionX != 0) {
                            lineX += directionX;
                            lineY = slope * lineX + searchingNode->getPosition().y;
                        }
                        else {
                            lineY += directionY;
                        }
                        
                        // フィールドの外に出たらbreak
                        if (lineX < 0 || lineY < 0 || lineX > mFieldSizeW-1 || lineY > mFieldSizeH-1) {
                            break;
                        }
                        
                        // 自チームのタイルがなければカウント
                        // 条件: 自チームのタイルがなく、自チームの領域にも含まれていない
                        if (mFieldDataHistory.back().fieldStatusArray[lineY][lineX] != team &&
                            ((team == TileStatus::ALLY && !mFieldDataHistory.back().fieldAllyAreaSquaresArray[lineY][lineX]) ||
                             (team == TileStatus::ENEMY && !mFieldDataHistory.back().fieldEnemyAreaSquaresArray[lineY][lineX]))) {
                                whiteTiles ++;
                            }
                        
                        // 目的のマスにたどり着いたときの処理
                        if (lineX == x && lineY == y) {
                            // 自チーム以外のタイルのカウントが1以上なら、領域を認識する
                            if (whiteTiles > 0) {
                                //cout << "囲いを検知(" << team << "): ";
                                //cout << searchingNode->getPosition().x << ',' << searchingNode->getPosition().y << '(' << searchingNode << ')' << endl;
                                
                                // 本当に囲めているか、searchAreaPointsSquares関数で確認
                                // 確認と同時に領域への指定を仮に行う
                                // 領域が確認できたら、areThereAreaPointsにtrue できなければfalse（ここでfalseになることはほぼありえない）
                                // 領域が確認できたら、仮の領域への指定を反映し、領域の辺を成すマスをmFieldAreaSideLinesArrayでtrueにする
                                vector<vector<bool>> areaSquares;
                                if (team == TileStatus::ALLY) {
                                    areaSquares = mFieldDataHistory.back().fieldAllyAreaSquaresArray;
                                }
                                else {
                                    areaSquares = mFieldDataHistory.back().fieldEnemyAreaSquaresArray;
                                }
                                bool areThereAreaPoints = searchAreaPointsSquares(areaSquares, lineXBefore, lineYBefore, team, false);
                                
                                // 領域が確認できたら...
                                if (areThereAreaPoints) {
                                    // 領域ポイントに反映
                                    if (team == TileStatus::ALLY) {
                                        mFieldDataHistory.back().allyScore.area = 0;
                                        
                                        // 仮の領域への指定を反映
                                        mFieldDataHistory.back().fieldAllyAreaSquaresArray = areaSquares;
                                        
                                    }
                                    else {
                                        mFieldDataHistory.back().enemyScore.area = 0;
                                        
                                        // 仮の領域への指定を反映
                                        mFieldDataHistory.back().fieldEnemyAreaSquaresArray = areaSquares;
                                        
                                    }
                                    
                                    for (int j = 0; j < mFieldSizeH; j++) {
                                        for (int k = 0; k < mFieldSizeW; k++) {
                                            if (team == TileStatus::ALLY && mFieldDataHistory.back().fieldAllyAreaSquaresArray[j][k]) {
                                                mFieldDataHistory.back().allyScore.area += abs(mFieldDataHistory.back().fieldPointsArray[j][k]);
                                            }
                                            if (team == TileStatus::ENEMY && mFieldDataHistory.back().fieldEnemyAreaSquaresArray[j][k]) {
                                                mFieldDataHistory.back().enemyScore.area += abs(mFieldDataHistory.back().fieldPointsArray[j][k]);
                                            }
                                        }
                                    }
                                    
                                    // 領域を成す辺を反映
                                    Node* currentNode = &node;
                                    while(1) {
                                        mFieldDataHistory.back().fieldAreaSideLinesArray[currentNode->getPosition().y][currentNode->getPosition().x] = true;
                                        
                                        if (currentNode->getParentNode() == nullptr) {
                                            break;
                                        }
                                        currentNode = currentNode->getParentNode();
                                    }
                                }
                            }
                            break;
                        }
                    }
                }
            }
            
            continue;       // すでに通ったマスだが始点ではないとき
        }
        
        // 始点から2マス以上離れていたら、始点にもマークを付ける（初期状態ではついていない）
        if (!argFieldMark[argStartY][argStartX] && (abs(argStartX-x) >= 2 || abs(argStartY-y) >= 2)) {
            argFieldMark[argStartY][argStartX] = true;
        }
        
        // マスにマークを付ける
        argFieldMark[y][x] = true;
        
        searchAreaPointsSide(argFieldMark, argStartX, argStartY, x, y, &node);
        
        nodes ++;
    }
}

/**
 searchAreaPointsSquares:
 *再帰関数
 領域と仮定したマスの探索
 Field::searchAreaPointsSideで領域が存在すると仮定したあとに、本当に領域が存在するか確認する。
 あるいは領域を成すタイルが相手に除去された時に、その後も領域が成立するかどうか確認する。
 走査線は4方向に移動しながら調査する。
 走査線が同じチームのタイルにぶつかるまで探索を続け、
 同じチームのタイルにぶつかることなくフィールドの外に走査線が出たら、領域が成立しないと判断する。
 フィールドの外に出ることなく、すべての走査線が同じチームのタイルにぶつかったら、領域が成立することを認める。
 領域が成立するならtrue, 成立しないならfalseを返す。
 
 @param argAreaSquares  領域にマスが含まれているかどうかを示す一時的な配列
 @param argStartX       領域探索の始点のマスのx座標
 @param argStartY       領域探索の始点のマスのy座標
 @param argBeTileStatus タイルをどちらのちチームの領域にしたいか（TileStatus::ALLY or TileStatus::ENEMY）
 @param argDeleteArea   領域を解除したいときはtrue, 領域を設定したいときはfalse
 @return 領域が成立すればtrue, しないならfalse（領域の解除に成功した場合もfalse）
 */
bool Field::searchAreaPointsSquares(vector<vector<bool>> &argAreaSquares, const int argStartX, const int argStartY, const int argBeTileStatus, const bool argDeleteArea) {
    int x, y;
    bool possible = true;
    
    // 領域を探索する対象のチーム
    // 領域の解除処理であれば、解除する対象のチーム
    Team::Type team;
    if (argBeTileStatus == TileStatus::ALLY) {
        team = Team::ALLY;
    }
    else if (argBeTileStatus == TileStatus::ENEMY) {
        team = Team::ENEMY;
    }
    else {
        return false;
    }
    
    // 領域内に指定（argDeleteArea == false）するとき
    if (!argDeleteArea) {
        if (!argAreaSquares[argStartY][argStartX]) {
            argAreaSquares[argStartY][argStartX] = true;
        }
    }
    // 領域を解除（argDeleteArea == true）するとき
    else {
        argAreaSquares[argStartY][argStartX] = false;
    }
    
    // 周囲4方向のマスの確認
    for (int i = 0; i < 4; i++) {
        x = argStartX + gSearchTileDirections[i].x;
        y = argStartY + gSearchTileDirections[i].y;
        
        
        // フィールドから出たら: 領域ができていないためループから出てfalseを返す
        if (x < 0 || y < 0 || x > mFieldSizeW-1 || y > mFieldSizeH-1) {
            possible = false;
            continue;
        }
        
        // 同じ色のマスにあたったら: continue
        if (mFieldDataHistory.back().fieldStatusArray[y][x] == argBeTileStatus) {
            continue;
        }
        
        // 領域の探索のとき すでに領域内に指定されているマスなら: continue
        if (!argDeleteArea && argAreaSquares[y][x]) {
            continue;
        }
        
        // 領域の解除のとき 自チームの領域でなければ
        if (argDeleteArea && !argAreaSquares[y][x]) {
            if (mFieldDataHistory.back().fieldStatusArray[y][x] == TileStatus::NONE) {
                possible = false;
            }
            
            continue;
        }
        
        // 周囲にまだ領域として指定していないマスがあったら: そのマスに移動
        // 領域の解除の場合は領域として指定されているマスがあったら
        bool nextTile = searchAreaPointsSquares(argAreaSquares, x, y, argBeTileStatus, argDeleteArea);
        
        if (!nextTile) {
            possible = false;   // 移動先で領域が成立しないことがわかったら: possible = false（領域実現不可）
        }
    }
    
    // 領域が成立しないことがわかったらfalseを返す（領域が成立しないことを示す）
    if (!possible) {
        return false;
    }
    
    // 特に問題がなければtrueを返す
    return true;
}


/**
 Field::searchAreaPoints:
 領域が存在するか確認する。
 この関数は、再帰関数であるsearchAreaPointsSideの呼び出し用の関数で、
 この関数自体は領域の探索を行わない。
 
 @param argStartX 領域探索の始点のマスのx座標
 @param argStartY 領域探索の始点のマスのy座標
 */
void Field::searchAreaPoints(const int argStartX, const int argStartY) {
    vector<vector<bool>> fieldMark(mFieldSizeH, vector<bool>(mFieldSizeW, false));
    searchAreaPointsSide(fieldMark, argStartX, argStartY, argStartX, argStartY, nullptr);
}

/**
 Field::fillSquare:
 マスを塗りつぶす（データ上で塗りつぶすのではなく、塗りつぶしたマスを描画する）
 
 @param argSquarePosition   マスの描画位置 Vec2型
 @param argX                マスのx座標
 @param argY                マスのy座標
 @param rectColor           Color型（空のままでも、なにか指定していても動作上変わることはない）
 */
void Field::fillSquare(const Vec2 argSquarePosition, const int argX, const int argY, Color& rectColor) {
    Color agentIDRectColor = Palette::White;
    
    // 味方のタイルのとき
    if (mFieldDataHistory.back().fieldStatusArray[argY][argX] == 1) {
        if (mAllyTeamColor == TeamColor::BLUE) {
            agentIDRectColor = Color(0, 128, 255); // 青
            rectColor = Color(0, 100, 200);
            
        }
        else if (mAllyTeamColor == TeamColor::RED) {
            agentIDRectColor = Color(255, 50, 0);  // 赤
            rectColor = Color(200, 10, 0);
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
        if (mFieldDataHistory.back().fieldAgentsIDArray[argY][argX] != 0) {
            Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize/3*2, 20).draw(agentIDRectColor);
            mAgentIDTextFont(U"Ally").draw(argSquarePosition.x+2, argSquarePosition.y, Color(Palette::White));
        }
    }
    // 相手のタイルのとき
    if (mFieldDataHistory.back().fieldStatusArray[argY][argX] == -1) {
        if (!mAllyTeamColor == TeamColor::BLUE) {
            agentIDRectColor = Color(0, 128, 255); // 青
            rectColor = Color(0, 100, 200);
        }
        else if (!mAllyTeamColor == TeamColor::RED) {
            agentIDRectColor = Color(255, 50, 0);  // 赤
            rectColor = Color(200, 10, 0);
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
        
        if (mFieldDataHistory.back().fieldAgentsIDArray[argY][argX] != 0) {
            Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize/3*2, 20).draw(agentIDRectColor);
            mAgentIDTextFont(U"Enemy").draw(argSquarePosition.x+2, argSquarePosition.y, Color(Palette::White));
        }
    }
}

/**
 Field::fillAreaSquare:
 領域マスを塗りつぶす（データ上で塗りつぶすのではなく、塗りつぶしたマスを描画する）
 
 @param argSquarePosition   マスの描画位置 Vec2型
 @param argX                マスのx座標
 @param argY                マスのy座標
 */
void Field::fillAreaSquare(const Vec2 argSquarePosition, const int argX, const int argY) {
    Color rectColor;
    
    // 味方の領域のとき
    if (mFieldDataHistory.back().fieldAllyAreaSquaresArray[argY][argX]) {
        if (mAllyTeamColor == TeamColor::BLUE) {
            rectColor = Color(150, 150, 255);   // 青
            
        }
        else if (mAllyTeamColor == TeamColor::RED) {
            rectColor = Color(255, 150, 150);   // 赤
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
    }
    // 相手の領域のとき
    if (mFieldDataHistory.back().fieldEnemyAreaSquaresArray[argY][argX]) {
        if (!mAllyTeamColor == TeamColor::BLUE) {
            rectColor = Color(150, 150, 255);   // 青
        }
        else if (!mAllyTeamColor == TeamColor::RED) {
            rectColor = Color(255, 150, 150);   // 赤
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
    }
    // 両チームの領域のとき
    if (mFieldDataHistory.back().fieldAllyAreaSquaresArray[argY][argX] && mFieldDataHistory.back().fieldEnemyAreaSquaresArray[argY][argX]) {
        rectColor = Color(200, 100, 200);       // 紫
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
    }
}

/**
 Field::printSquarePoint:
 タイル点数の表示
 
 @param argSquarePosition   表示するマスの描画位置 Vec2型
 @param argX                表示するマスのx座標
 @param argY                表示するマスのy座標
 @param rectColor           Color型（空のままでも、なにか指定していても動作上変わることはない）
 */
void Field::printSquarePoint(const Vec2 argSquarePosition, const int argX, const int argY, Color& rectColor) {
    unsigned long int textLength = to_string(mFieldDataHistory.back().fieldPointsArray[argY][argX]).length();
    
    Color textColor = Palette::Black;
    if (rectColor != Palette::White) {
        textColor = Palette::White;
    }
    
    if (mFieldDataHistory.back().fieldPointsArray[argY][argX] >= mBigPointsBorder) {    // 得点が上位20％(=mBigPointsBorder)以上なら太字
        mPointTextFontBold(mFieldDataHistory.back().fieldPointsArray[argY][argX])
        .draw(argSquarePosition.x+mFieldSquareSize/2-textLength*(mFieldSquareSize-POINT_TEXT_BIG_FONT_SIZE)/3, argSquarePosition.y+mFieldSquareSize/2-(mFieldSquareSize-POINT_TEXT_BIG_FONT_SIZE)/2-5, Color(textColor));
    }
    else {                                                      // 得点が上位20％(=mBigPointsBorder)未満なら普通の字
        mPointTextFont(mFieldDataHistory.back().fieldPointsArray[argY][argX])
        .draw(argSquarePosition.x+mFieldSquareSize/2-textLength*(mFieldSquareSize-POINT_TEXT_FONT_SIZE)/3, argSquarePosition.y+mFieldSquareSize/2-(mFieldSquareSize-POINT_TEXT_FONT_SIZE)/2-5, Color(textColor));
    }
}

void Field::drawArrow(const Vec2 argSquarePosition, const int argBeforeX, const int argBeforeY, const int argX, const int argY) {
    // 表示する矢印の決定
    Vec2 movedDirection = Vec2{argX, argY}-Vec2{argBeforeX, argBeforeY};
    String arrow;
    Vec2 drawPosition;
    
    if (movedDirection == Vec2{-1, -1}) {       // 左上
        arrow = U"↖";
        drawPosition = Vec2{argSquarePosition.x+mFieldSquareSize-(POINT_TEXT_FONT_SIZE+10)/3, argSquarePosition.y+mFieldSquareSize-(POINT_TEXT_FONT_SIZE+10)/2};
    }
    else if (movedDirection == Vec2{0, -1}) {   // 上
        arrow = U"↑";
        drawPosition = Vec2{argSquarePosition.x+mFieldSquareSize/2-(POINT_TEXT_FONT_SIZE+10)/3, argSquarePosition.y+mFieldSquareSize-(POINT_TEXT_FONT_SIZE+10)/2};
    }
    else if (movedDirection == Vec2{1, -1}) {   // 右上
        arrow = U"↗";
        drawPosition = Vec2{argSquarePosition.x-(POINT_TEXT_FONT_SIZE+10)/3, argSquarePosition.y+mFieldSquareSize-(POINT_TEXT_FONT_SIZE+10)/2};
    }
    else if (movedDirection == Vec2{1, 0}) {    // 右
        arrow = U"→";
        drawPosition = Vec2{argSquarePosition.x-(POINT_TEXT_FONT_SIZE+10)/3, argSquarePosition.y+mFieldSquareSize/2-(POINT_TEXT_FONT_SIZE+10)/2};
    }
    else if (movedDirection == Vec2{1, 1}) {    // 右下
        arrow = U"↘";
        drawPosition = Vec2{argSquarePosition.x-(POINT_TEXT_FONT_SIZE+10)/3, argSquarePosition.y-(POINT_TEXT_FONT_SIZE+10)/2};
    }
    else if (movedDirection == Vec2{0, 1}) {    // 下
        arrow = U"↓";
        drawPosition = Vec2{argSquarePosition.x+mFieldSquareSize/2-(POINT_TEXT_FONT_SIZE+10)/3, argSquarePosition.y-(POINT_TEXT_FONT_SIZE+10)/2};
    }
    else if (movedDirection == Vec2{-1, 1}) {   // 左下
        arrow = U"↙";
        drawPosition = Vec2{argSquarePosition.x+mFieldSquareSize-(POINT_TEXT_FONT_SIZE+10)/3, argSquarePosition.y-(POINT_TEXT_FONT_SIZE+10)/2};
    }
    else if (movedDirection == Vec2{-1, 0}) {   // 左
        arrow = U"←";
        drawPosition = Vec2{argSquarePosition.x+mFieldSquareSize-(POINT_TEXT_FONT_SIZE+10)/3, argSquarePosition.y+mFieldSquareSize/2-(POINT_TEXT_FONT_SIZE+10)/2};
    }
    else {
        return;
    }
    
    mPointTextFontBold(arrow).draw(drawPosition.x, drawPosition.y, Color(Palette::Black));
}

/**
 Field::removeTile:
 タイルを除去する。
 また、領域を成すタイルを除去する場合は、
 除去後に領域が成立するか確認するため、searchAreaPointsSquare関数を呼び出す。
 
 @param argX            タイルを除去するマスのx座標
 @param argY            タイルを除去するマスのy座標
 @param argTileStatus   タイルを除去するチームのTileStatus（除去されるチームではない）
 */
void Field::removeTile(const int argX, const int argY, const int argTileStatus) {
    mFieldDataHistory.back().fieldStatusArray[argY][argX] = 0;
    
    // 相手から点数を引く
    if (mCurrentAgentID > 0) {
        mFieldDataHistory.back().enemyScore.tile -= mFieldDataHistory.back().fieldPointsArray[argY][argX];
    }
    else {
        mFieldDataHistory.back().allyScore.tile -= mFieldDataHistory.back().fieldPointsArray[argY][argX];
    }
    
    // 相手チームの領域に隣接していたら、相手チームの領域を再計算
    for (int i = 0; i < 4; i++) {
        if (argX+gSearchTileDirections[i].x < 0 || argY+gSearchTileDirections[i].y < 0 ||
            argX+gSearchTileDirections[i].x > mFieldSizeW-1 || argY+gSearchTileDirections[i].y > mFieldSizeH-1) {
            continue;
        }
        
        // 領域の辺を成すタイルなら、除去後も領域が成立するか確認
        // 成立しなければ、領域を解除する
        if ((argTileStatus == TileStatus::ENEMY && mFieldDataHistory.back().fieldAllyAreaSquaresArray[argY+gSearchTileDirections[i].y][argX+gSearchTileDirections[i].x]) ||
            (argTileStatus == TileStatus::ALLY && mFieldDataHistory.back().fieldEnemyAreaSquaresArray[argY+gSearchTileDirections[i].y][argX+gSearchTileDirections[i].x])) {
            if (mFieldDataHistory.back().fieldAreaSideLinesArray[argY][argX]) {
                
                // 味方チームのエージェントが相手チームのタイルを除去するとき
                if (mCurrentAgentID > 0) {
                    removeArea(argX+gSearchTileDirections[i].x, argY+gSearchTileDirections[i].y, TileStatus::ENEMY);
                }
                // 相手チームのエージェントが味方チームのタイルを除去するとき
                else if (mCurrentAgentID < 0) {
                    removeArea(argX+gSearchTileDirections[i].x, argY+gSearchTileDirections[i].y, TileStatus::ALLY);
                }
                
            }
            /*
            else {
                if (mCurrentAgentID > 0) {
                    vector<vector<bool>> areaSquares = mFieldDataHistory.back().fieldAllyAreaSquaresArray;
                    bool areThereArea = searchAreaPointsSquares(areaSquares, argX+gSearchTileDirections[i].x, argY+gSearchTileDirections[i].y, TileStatus::ALLY, false);
                    
                    if (areThereArea) {
                        mFieldDataHistory.back().fieldAllyAreaSquaresArray = areaSquares;
                    }
                }
                else if (mCurrentAgentID < 0) {
                    vector<vector<bool>> areaSquares = mFieldDataHistory.back().fieldEnemyAreaSquaresArray;
                    bool areThereArea = searchAreaPointsSquares(areaSquares, argX+gSearchTileDirections[i].x, argY+gSearchTileDirections[i].y, TileStatus::ENEMY, false);
                    
                    if (areThereArea) {
                        mFieldDataHistory.back().fieldEnemyAreaSquaresArray = areaSquares;
                    }
                }
            }
             */
        }
    }
}

/**
 Field::removeArea:
    (argX, argY)に隣接する領域を除去する。

 @param argX            始点のx座標
 @param argY            始点のy座標
 @param argBeforeX      移動前のx座標
 @param argBeforeY      移動前のy座標
 @param argRemoveTeam   除去されるチーム
 */
void Field::removeArea(const int argX, const int argY, const int argRemoveTeam) {
    vector<vector<bool>> areaSquares;
    if (argRemoveTeam == TileStatus::ALLY) {
        areaSquares = mFieldDataHistory.back().fieldAllyAreaSquaresArray;
    }
    else if (argRemoveTeam == TileStatus::ENEMY) {
        areaSquares = mFieldDataHistory.back().fieldEnemyAreaSquaresArray;
    }
    
    bool areThereArea = searchAreaPointsSquares(areaSquares, argX, argY, argRemoveTeam, true);
    
    if (!areThereArea) {
        // 領域ポイントに反映
        for (int j = 0; j < mFieldSizeH; j++) {
            for (int k = 0; k < mFieldSizeW; k++) {
                if (argRemoveTeam == TileStatus::ALLY) {
                    if (mFieldDataHistory.back().fieldAllyAreaSquaresArray[j][k] && !areaSquares[j][k]) {
                        mFieldDataHistory.back().allyScore.area -= abs(mFieldDataHistory.back().fieldPointsArray[j][k]);
                    }
                }
                else if (argRemoveTeam == TileStatus::ENEMY) {
                    if (mFieldDataHistory.back().fieldEnemyAreaSquaresArray[j][k] && !areaSquares[j][k]) {
                        mFieldDataHistory.back().enemyScore.area -= abs(mFieldDataHistory.back().fieldPointsArray[j][k]);
                    }
                }
            }
        }
        
        if (argRemoveTeam == TileStatus::ALLY) {
            mFieldDataHistory.back().fieldAllyAreaSquaresArray = areaSquares;
        }
        else if (argRemoveTeam == TileStatus::ENEMY) {
            mFieldDataHistory.back().fieldEnemyAreaSquaresArray = areaSquares;
        }
        
        // もう一度確認する（このタイルを除去しても領域が成立する場合もあるため）
        // 成立するなら領域を解除しない
        bool areaRecheck = searchAreaPointsSquares(areaSquares, argX, argY, argRemoveTeam, false);
        
        if (areaRecheck) {
            // 領域ポイントに反映
            for (int j = 0; j < mFieldSizeH; j++) {
                for (int k = 0; k < mFieldSizeW; k++) {
                    if (!mFieldDataHistory.back().fieldAllyAreaSquaresArray[j][k] && areaSquares[j][k]) {
                        if (argRemoveTeam == TileStatus::ALLY) {
                            mFieldDataHistory.back().allyScore.area += abs(mFieldDataHistory.back().fieldPointsArray[j][k]);
                        }
                        else if (argRemoveTeam == TileStatus::ENEMY) {
                            mFieldDataHistory.back().enemyScore.area += abs(mFieldDataHistory.back().fieldPointsArray[j][k]);
                        }
                    }
                }
            }
            
            if (argRemoveTeam == TileStatus::ALLY) {
                mFieldDataHistory.back().fieldAllyAreaSquaresArray = areaSquares;
            }
            else if (argRemoveTeam == TileStatus::ENEMY) {
                mFieldDataHistory.back().fieldEnemyAreaSquaresArray = areaSquares;
            }
        }
    }
}

/**
 Field::putTile:
 タイルを置く。
 また、領域ポイントの存在を確認するため、searchAreaPoints関数を呼び出す。

 @param argX            タイルを置くマスのx座標
 @param argY            タイルを置くマスのy座標
 @param argTileStatus   タイルを置くチームのTileStatus
 */
void Field::putTile(const int argX, const int argY, const int argTileStatus) {
    mFieldDataHistory.back().fieldStatusArray[argY][argX] = argTileStatus;
        
    if (mCurrentAgentID > 0) {          // 味方エージェント
        if (mFieldDataHistory.back().fieldAllyAreaSquaresArray[argY][argX]) {
            mFieldDataHistory.back().fieldAllyAreaSquaresArray[argY][argX] = false;
            mFieldDataHistory.back().allyScore.area -= abs(mFieldDataHistory.back().fieldPointsArray[argY][argX]);
        }
            
        mFieldDataHistory.back().allyScore.tile += mFieldDataHistory.back().fieldPointsArray[argY][argX];       // タイルポイントにマスの得点を加算
    }
    else if (mCurrentAgentID < 0) {     // 相手エージェント
        if (mFieldDataHistory.back().fieldEnemyAreaSquaresArray[argY][argX]) {
            mFieldDataHistory.back().fieldEnemyAreaSquaresArray[argY][argX] = false;
            mFieldDataHistory.back().enemyScore.area -= abs(mFieldDataHistory.back().fieldPointsArray[argY][argX]);
        }
            
        mFieldDataHistory.back().enemyScore.tile += mFieldDataHistory.back().fieldPointsArray[argY][argX];      // タイルポイントにマスの得点を加算
    }
    else {                              // どちらでもない（エージェントが選択されていない）
        return;
    }
    
    // 領域ポイントができているか確認
    searchAreaPoints(argX, argY);
}

/**
 Field::agenrMovement:
 エージェントの行動
 エージェントの移動、タイル設置(Field::putTile関数内)、タイルの除去(Field::removeTile関数内)などを行う。
 タイル設置時には、領域が成立するかも確認する(Field::searchAreaPoints関数内)。
 相手チームの領域の辺を成すタイルを除去するときは、相手チームの領域の解除ができるかどうかも調べる。
 最後に、エージェントの行動の番号を返す。
 
 @param argX        エージェントの移動先のマスのx座標
 @param argY        エージェントの移動先のマスのy座標
 @param argBeforeX  エージェントの移動前のマスのx座標
 @param argBeforeY  エージェントの移動前のマスのy座標
 @return エージェントの行動の番号（0~16）　field.hppのAgentActNumbersに定義済み
 */
int Field::agentMovement(int argX, int argY, int argBeforeX, int argBeforeY) {
    int actNumber = 0;      // 行動の番号
    
    if (Vec2{argY, argX} != Vec2{-1, -1}) {
        mFieldDataHistory.push_back(mFieldDataHistory.back());
        
        int beforeCurrentX = argBeforeX, beforeCurrentY = argBeforeY;
        
        // 最後の操作履歴と前回のターンでのエージェントの位置が一致しない（=1回以上同一ターン内で同じエージェントを移動させた）場合
        // 最後の操作履歴を削除し、2つ前の操作履歴に戻す
        if (mCurrentAgentID > 0) {
            // 同じエージェントが同じターン内の前回の操作で除去を行っていた場合
            if (mFieldDataHistory.size() > 2) {
                if (mFieldDataHistory.end()[-2].allyAgentsActNumbers[mCurrentAgentID-1] >= 9 && mFieldDataHistory.end()[-3].allyAgentsActNumbers[mCurrentAgentID-1] < 9) {
                    mFieldDataHistory.back().fieldStatusArray = mFieldDataHistory.end()[-3].fieldStatusArray;
                    mFieldDataHistory.back().fieldEnemyAreaSquaresArray = mFieldDataHistory.end()[-3].fieldEnemyAreaSquaresArray;
                }
            }
            
            if (mFieldDataHistory.back().allyAgentsPosition[mCurrentAgentID-1].first  != mFieldData.allyAgentsPosition[mCurrentAgentID-1].first ||
                mFieldDataHistory.back().allyAgentsPosition[mCurrentAgentID-1].second != mFieldData.allyAgentsPosition[mCurrentAgentID-1].second) {
                
                mFieldDataHistory.back().fieldStatusArray[argBeforeY][argBeforeX] = 0;
                mFieldDataHistory.back().fieldAgentsIDArray[argBeforeY][argBeforeX] = 0;
                mFieldDataHistory.back().allyScore.tile -= mFieldDataHistory.back().fieldPointsArray[argBeforeY][argBeforeX];
                
                argBeforeX = mFieldData.allyAgentsPosition[mCurrentAgentID-1].first;
                argBeforeY = mFieldData.allyAgentsPosition[mCurrentAgentID-1].second;
            }
        }
        else if (mCurrentAgentID < 0) {
            // 同じエージェントが同じターン内の前回の操作で除去を行っていた場合
            if (mFieldDataHistory.size() > 2) {
                if (mFieldDataHistory.end()[-2].enemyAgentsActNumbers[(-1)*mCurrentAgentID-1] >= 9 && mFieldDataHistory.end()[-3].enemyAgentsActNumbers[(-1)*mCurrentAgentID-1] < 9) {
                    mFieldDataHistory.back().fieldStatusArray = mFieldDataHistory.end()[-3].fieldStatusArray;
                    mFieldDataHistory.back().fieldAllyAreaSquaresArray = mFieldDataHistory.end()[-3].fieldAllyAreaSquaresArray;
                }
            }
            
            if (mFieldDataHistory.back().enemyAgentsPosition[(-1)*mCurrentAgentID-1].first  != mFieldData.enemyAgentsPosition[(-1)*mCurrentAgentID-1].first ||
                mFieldDataHistory.back().enemyAgentsPosition[(-1)*mCurrentAgentID-1].second != mFieldData.enemyAgentsPosition[(-1)*mCurrentAgentID-1].second) {
                
                mFieldDataHistory.back().fieldStatusArray[argBeforeY][argBeforeX] = 0;
                mFieldDataHistory.back().fieldAgentsIDArray[argBeforeY][argBeforeX] = 0;
                mFieldDataHistory.back().enemyScore.tile -= mFieldDataHistory.back().fieldPointsArray[argBeforeY][argBeforeX];
                
                argBeforeX = mFieldData.enemyAgentsPosition[(-1)*mCurrentAgentID-1].first;
                argBeforeY = mFieldData.enemyAgentsPosition[(-1)*mCurrentAgentID-1].second;
            }
        }
        
        // 同じターン内で領域を成すマスからエージェントを移動させようとしたとき
        if (mFieldDataHistory.size() > 2) {
            if (mFieldDataHistory.back().fieldAreaSideLinesArray[beforeCurrentY][beforeCurrentX] &&
                mFieldDataHistory.back().fieldAgentsIDArray[argY][argX] == mFieldDataHistory.end()[-3].fieldAgentsIDArray[beforeCurrentY][beforeCurrentX]) {
                
                for (int i = 0; i < 4; i++) {
                    if (beforeCurrentX + gSearchTileDirections[i].x < 0 || beforeCurrentY + gSearchTileDirections[i].y < 0 ||
                        beforeCurrentX + gSearchTileDirections[i].x > mFieldSizeW-1 || beforeCurrentY + gSearchTileDirections[i].y > mFieldSizeH-1) {
                        continue;
                    }
                    
                    if (mCurrentAgentID > 0 && mFieldDataHistory.back().fieldAllyAreaSquaresArray[beforeCurrentY+gSearchTileDirections[i].y][beforeCurrentX+gSearchTileDirections[i].x]) {
                        removeArea(beforeCurrentX+gSearchTileDirections[i].x, beforeCurrentY+gSearchTileDirections[i].y, TileStatus::ALLY);
                    }
                    else if (mCurrentAgentID < 0 && mFieldDataHistory.back().fieldEnemyAreaSquaresArray[beforeCurrentY+gSearchTileDirections[i].y][beforeCurrentX+gSearchTileDirections[i].x]) {
                        removeArea(beforeCurrentX+gSearchTileDirections[i].x, beforeCurrentY+gSearchTileDirections[i].y, TileStatus::ENEMY);
                    }
                }
            }
        }
        
        int beTileStatus = mFieldData.fieldStatusArray[argBeforeY][argBeforeX];   // 捜査対象のエージェントのチームのStatus
        
        // （動かすエージェントから見て）相手のタイルなら タイルを除去
        // 相手の領域を成すタイルなら、除去後にその領域が成立するか確認
        if (mFieldDataHistory.back().fieldStatusArray[argY][argX] == (-1)*mFieldDataHistory.back().fieldStatusArray[argBeforeY][argBeforeX] &&
            mFieldData.fieldAgentsIDArray[argY][argX] == 0) {
            // タイルの除去（相手の領域を成すタイルなら、除去後にその領域が成立するか確認）
            removeTile(argX, argY, beTileStatus);
            
            // 返す行動番号の取得
            actNumber = AgentActNumbers::getAgentActNumber(pair{argX, argY}, pair{argBeforeX, argBeforeY}, true);
        }
        // それ以外ならマスにエージェントを移動させタイルを置く
        // 領域が成立するなら領域の設定も行う
        else if ((mFieldData.fieldStatusArray[argY][argX] == 0 || mFieldData.fieldStatusArray[argY][argX] == beTileStatus) &&
                 mFieldDataHistory.back().fieldAgentsIDArray[argY][argX] == 0) {
            mCurrentSquarePosition = {argX, argY};
            
            // タイルを置く処理（まだタイルが置かれていない場合のみ）
            // 領域が成立するなら領域の設定も行う
            if (mFieldDataHistory.back().fieldStatusArray[argY][argX] != beTileStatus) {
                putTile(argX, argY, beTileStatus);
            }
            
            // エージェントの位置も移動させる
            mFieldDataHistory.back().fieldAgentsIDArray[argBeforeY][argBeforeX] = 0;
            mFieldDataHistory.back().fieldAgentsIDArray[argY][argX] = mCurrentAgentID;
            
            // エージェントの座標を配列に格納
            // 味方エージェントの場合
            if (mCurrentAgentID > 0) {
                mFieldDataHistory.back().allyAgentsPosition[mCurrentAgentID-1] = pair{argX, argY};
            }
            // 相手エージェントの場合
            else if (mCurrentAgentID < 0) {
                mFieldDataHistory.back().enemyAgentsPosition[(-1)*mCurrentAgentID-1] = pair{argX, argY};
            }
            
            // 返す行動番号の取得
            actNumber = AgentActNumbers::getAgentActNumber(pair{argX, argY}, pair{argBeforeX, argBeforeY}, false);
        }
    }
    
    // 行動の番号を配列に格納
    // 味方エージェントの場合
    if (mCurrentAgentID > 0) {
        mFieldDataHistory.back().allyAgentsActNumbers[mCurrentAgentID-1] = actNumber;
    }
    // 相手エージェントの場合
    else {
        mFieldDataHistory.back().enemyAgentsActNumbers[(-1)*mCurrentAgentID-1] = actNumber;
    }
    
    return actNumber;
}

/**
 Field::setCurrentAgent:
 選択するエージェントの変更処理
 
 @param argX カーソルで選択されたx座標
 @param argY カーソルで選択されたy座標
 */
void Field::setCurrentAgent(int argX, int argY) {
    // 新しくエージェントを選択するとき
    if (mFieldDataHistory.back().fieldAgentsIDArray[argY][argX] != 0) {
        if (mCurrentAgentID == mFieldDataHistory.back().fieldAgentsIDArray[argY][argX]) {
            mCurrentSquarePosition = {-1, -1};
            mCurrentAgentID = 0;
        }
        else {
            mCurrentSquarePosition = {argX, argY};
            mCurrentAgentID = mFieldDataHistory.back().fieldAgentsIDArray[argY][argX];
        }
    }
}

/**
 Field::drawSquares:
 フィールドの縦線・横線の描画
 */
void Field::drawSquares() {
    for (int x = 0; x < mFieldSizeW; x++) {
        Line(x*mFieldSquareSize+mFieldLeftmostPoint, mFieldTopmostPoint,
             x*mFieldSquareSize+mFieldLeftmostPoint, mFieldBottommostPoint).draw(Color(Palette::Gray));
    }
    for (int y = 0; y < mFieldSizeH; y++) {
        Line(mFieldLeftmostPoint, y*mFieldSquareSize+mFieldTopmostPoint,
             mFieldRightmostPoint, y*mFieldSquareSize+mFieldTopmostPoint).draw(Color(Palette::Gray));
    }
}

/**
 Field::draw:
 フィールドの描画
 */
void Field::draw() {
    Color rectColor;
    
    // フィールドの縦線・横線の描画
    drawSquares();
    
    // 点数の表示
    // チーム色による塗りつぶし
    // エージェントIDの表示
    // マスの上にカーソルがあれば マスに枠を表示
    //Vec2 currentSquarePositionBefore = mCurrentSquarePosition;
    
    for (int y = 0; y < mFieldSizeH; y++) {
        for (int x = 0; x < mFieldSizeW; x++) {
            // マスの描画位置
            int squarePositionX = mFieldLeftmostPoint+x*mFieldSquareSize;
            int squarePositionY = mFieldTopmostPoint+y*mFieldSquareSize;
            
            // 領域の塗りつぶし（マスが囲まれている場合）
            if (mFieldData.fieldStatusArray[y][x] == TileStatus::NONE) {
                fillAreaSquare(Vec2{squarePositionX, squarePositionY}, x, y);
            }
            
            // マスの塗りつぶし（どちらかのチームのタイルの場合）
            rectColor = Palette::White;
            fillSquare(Vec2{squarePositionX, squarePositionY}, x, y, rectColor);
            
            // 点数の表示
            printSquarePoint(Vec2{squarePositionX, squarePositionY}, x, y, rectColor);
            
            // 枠の表示
            if (isCursorOnTheSquare(Vec2(mFieldLeftmostPoint+x*mFieldSquareSize, mFieldTopmostPoint+y*mFieldSquareSize))) {
                // マウスが左クリックの状態かつ 選択されていたマスから移動できるマスならば、選択されているマスに設定
                // エージェントを選択されているマスに移動
                if (MouseL.pressed() && !mMousePressing) {
                    mMousePressing = true;      // 左クリックが押された瞬間だけ
                    
                    if (mFieldDataHistory.back().fieldAgentsIDArray[y][x] != mCurrentAgentID && mFieldDataHistory.back().fieldAgentsIDArray[y][x] != 0) {
                        // 新しくエージェントを選択するときの処理
                        setCurrentAgent(x, y);
                    }
                    else {
                        // 移動前のタイルから見た周囲8方向のタイルの調査
                        // 周囲8方向のタイルの一つが選択したタイルならば、移動・除去が可能
                        for (int i=0; i<DIRECTIONS; i++) {
                            
                            if (x+(-1)*gMoveDirections[i].x < 0 || y+(-1)*gMoveDirections[i].y < 0 ||
                                x+(-1)*gMoveDirections[i].x > mFieldSizeW-1 || y+(-1)*gMoveDirections[i].y > mFieldSizeH-1) {
                                continue;
                            }
                            
                            // 味方エージェントの操作
                            if (mCurrentAgentID > 0) {
                                if (mFieldData.allyAgentsPosition[mCurrentAgentID-1].first + gMoveDirections[i].x == x &&
                                    mFieldData.allyAgentsPosition[mCurrentAgentID-1].second + gMoveDirections[i].y == y) {
                                    // エージェントの行動に反映
                                    // 移動, 削除
                                    agentMovement(x, y, mFieldDataHistory.back().allyAgentsPosition[mCurrentAgentID-1].first, mFieldDataHistory.back().allyAgentsPosition[mCurrentAgentID-1].second);
                                    
                                    break;
                                }
                            }
                            // 相手エージェントの操作
                            if (mCurrentAgentID < 0) {
                                if (mFieldData.enemyAgentsPosition[(-1)*mCurrentAgentID-1].first + gMoveDirections[i].x == x &&
                                    mFieldData.enemyAgentsPosition[(-1)*mCurrentAgentID-1].second + gMoveDirections[i].y == y) {
                                    // エージェントの行動に反映
                                    // 移動, 削除
                                    agentMovement(x, y, mFieldDataHistory.back().enemyAgentsPosition[(-1)*mCurrentAgentID-1].first, mFieldDataHistory.back().enemyAgentsPosition[(-1)*mCurrentAgentID-1].second);
                                    
                                    break;
                                }
                            }
                        }
                    }
                }
                else {
                    // カーソルが上にあればマスに黄色い枠を表示
                    Rect(squarePositionX, squarePositionY, mFieldSquareSize, mFieldSquareSize).drawFrame(5, 0, Color(Palette::Yellow));
                }
            }
            
            // マスが選択されている状態ならば 橙色の枠を表示
            if (Vec2{x, y} == mCurrentSquarePosition) {
                Rect(squarePositionX, squarePositionY, mFieldSquareSize, mFieldSquareSize).drawFrame(5, 0, Color(Palette::Orange));
            }
        }
        
        if (!MouseL.pressed()) {
            mMousePressing = false;
        }
    }
    
    // エージェントの経路を示す矢印の表示
    // 一番上に表示させたいため、もう一度ループ
    for (int y = 0; y < mFieldSizeH; y++) {
        for (int x = 0; x < mFieldSizeW; x++) {
            if (mFieldDataHistory.back().fieldAgentsIDArray[y][x] != 0) {
                for (int i = 0; i < 8; i++) {
                    if (x+gMoveDirections[i].x < 0 || y+gMoveDirections[i].y < 0 ||
                        x+gMoveDirections[i].x > mFieldSizeW-1 || y+gMoveDirections[i].y > mFieldSizeH-1) {
                        continue;
                    }
                    
                    if (mFieldData.fieldAgentsIDArray[y+gMoveDirections[i].y][x+gMoveDirections[i].x] == mFieldDataHistory.back().fieldAgentsIDArray[y][x]) {
                        drawArrow(Vec2{mFieldLeftmostPoint+x*mFieldSquareSize, mFieldTopmostPoint+y*mFieldSquareSize}, x+gMoveDirections[i].x, y+gMoveDirections[i].y, x, y);
                    }
                }
            }
        }
    }
    
    // フィールドの枠の描画
    Rect(mFieldLeftmostPoint, mFieldTopmostPoint, mFieldLineLengthW, mFieldLineLengthH).drawFrame(0, 4, Color(Palette::Black));
    
    return;
}

/**
 Field::startSolving:
    探索の開始
 */
void Field::startSolving() {
    // 探索は1ターン1回まで
    if (!mSolvedPerTurn) {
        mSolvedPerTurn = true;
    }
    else {
        return;
    }
    
    // とりあえず全てのエージェントをランダムに移動
    for (int i = 0; i < mTotalTeamAgents; i++) {
        // 味方
        int moveX = rand()%3-1;
        int moveY = rand()%3-1;
        
        if (mFieldData.allyAgentsPosition[i].first+moveX < 0 || mFieldData.allyAgentsPosition[i].second+moveY < 0 || mFieldData.allyAgentsPosition[i].first+moveX > mFieldSizeW-1 || mFieldData.allyAgentsPosition[i].second+moveY > mFieldSizeH-1) {
            continue;
        }
        
        setCurrentAgent(mFieldData.allyAgentsPosition[i].first, mFieldData.allyAgentsPosition[i].second);
        
        agentMovement(mFieldData.allyAgentsPosition[i].first+moveX, mFieldData.allyAgentsPosition[i].second+moveY, mFieldData.allyAgentsPosition[i].first, mFieldData.allyAgentsPosition[i].second);
        
        // 相手
        moveX = rand()%3-1;
        moveY = rand()%3-1;
        
        if (mFieldData.enemyAgentsPosition[i].first+moveX < 0 || mFieldData.enemyAgentsPosition[i].second+moveY < 0 || mFieldData.enemyAgentsPosition[i].first+moveX > mFieldSizeW-1 || mFieldData.enemyAgentsPosition[i].second+moveY > mFieldSizeH-1) {
            continue;
        }
        
        setCurrentAgent(mFieldData.enemyAgentsPosition[i].first, mFieldData.enemyAgentsPosition[i].second);
        agentMovement(mFieldData.enemyAgentsPosition[i].first+moveX, mFieldData.enemyAgentsPosition[i].second+moveY, mFieldData.enemyAgentsPosition[i].first, mFieldData.enemyAgentsPosition[i].second);
    }
}

void Field::setSolvingWithChangeTurn(bool argBool) {
    mSolvingWithChangeTurn = argBool;
}

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
    mFieldPointsArray = argFieldPointsArray;    // フィールドの得点
    mFieldStatusArray = argFieldStatusArray;    // フィールドの状況
    
    int maxPoint = -16, minPoint = 16;
    int allyAgentsCount = 0, enemyAgentsCount = 0;
    
    for (int y = 0; y < mFieldSizeH; y++) {
        mFieldAgentsIDArray.push_back(vector<int>(mFieldSizeW, 0));
        
        mFieldAllyAreaSquaresArray.push_back(vector<bool>(mFieldSizeW, false));
        mFieldEnemyAreaSquaresArray.push_back(vector<bool>(mFieldSizeW, false));
        
        mFieldAreaSideLinesArray.push_back(vector<bool>(mFieldSizeW, false));
        for (int x = 0; x < mFieldSizeW; x++) {
            if (mFieldStatusArray[y][x] == 1) {
                allyAgentsCount ++;
                mFieldAgentsIDArray[y][x] = allyAgentsCount;    // エージェントにIDを振り分ける
                
                mAllyTilePoints += mFieldPointsArray[y][x];     // 初期段階の合計得点の算出
            }
            else if (mFieldStatusArray[y][x] == -1) {
                enemyAgentsCount ++;
                mFieldAgentsIDArray[y][x] = -enemyAgentsCount;  // エージェントにIDを振り分ける
                
                mEnemyTilePoints += mFieldPointsArray[y][x];    // 初期段階の合計得点の算出
            }
            
            // 最大値と最小値の検出
            if (mFieldPointsArray[y][x] > maxPoint) {
                maxPoint = mFieldPointsArray[y][x];
            }
            if (mFieldPointsArray[y][x] < minPoint) {
                minPoint = mFieldPointsArray[y][x];
            }
        }
        
        mAllyPoints = mAllyTilePoints;
        mEnemyPoints = mEnemyTilePoints;
    }
    
    // 大きい点数の基準を決める（最大値-最小値の上位20％）
    mBigPointsBorder = minPoint+(maxPoint-minPoint)*0.8;
    
    // 合計エージェント数
    mTotalTeamAgents = allyAgentsCount;
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
            if (mFieldAgentsIDArray[y][x] > 0) {
                argAllyAgentsPosition[countAllyAgents] = {x, y};
                countAllyAgents ++;
            }
            // 相手のエージェント
            if (mFieldAgentsIDArray[y][x] < 0) {
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
 合計得点(mAllyPoints, mEnemyPoints)を返す。
 
 @param argTeam 得点を取得したいチームの番号(味方 Team::ALLY, 相手 Team::ENEMY)
 @return 合計得点
 */
int Field::getTotalPoints(const Team::Type argTeam) {
    if (argTeam == Team::ALLY) {    // 味方
        return mAllyPoints;
    }
    else {                          // 敵
        return mEnemyPoints;
    }
}

/**
 Field::getTilePoints:
 タイルポイント(mAllyTilePoints, mEnemyTilePoints)を返す。
 
 @param argTeam 得点を取得したいチームの番号(味方 Team::ALLY, 相手 Team::ENEMY)
 @return タイルポイント
 */
int Field::getTilePoints(const Team::Type argTeam) {
    if (argTeam == Team::ALLY) {    // 味方
        return mAllyTilePoints;
    }
    else {                          // 敵
        return mEnemyTilePoints;
    }
}

/**
 Field::getAreaPoints:
 領域ポイント(mAllyAreaPoints, mEnemyAreaPoints)を返す。
 
 @param argTeam 得点を取得したいチームの番号(味方 Team::ALLY, 相手 Team::ENEMY)
 @return 領域ポイント
 */
int Field::getAreaPoints(const Team::Type argTeam) {
    if (argTeam == Team::ALLY) {    // 味方
        return mAllyAreaPoints;
    }
    else {                          // 敵
        return mEnemyAreaPoints;
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
    if (mFieldStatusArray[argStartY][argStartX] == TileStatus::ALLY) {
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
                if (mFieldStatusArray[y+gSearchTileDirections[j].y][x+gSearchTileDirections[j].x] != team) {
                    areThereOthersTiles = true;
                    break;
                }
            }
        }
        if (!areThereOthersTiles) {
            continue;
        }
        
        // 違う色のマスならcontinue
        if (mFieldStatusArray[y][x] != team) {
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
                        if (mFieldStatusArray[lineY][lineX] != team &&
                            ((team == TileStatus::ALLY && !mFieldAllyAreaSquaresArray[lineY][lineX]) ||
                             (team == TileStatus::ENEMY && !mFieldEnemyAreaSquaresArray[lineY][lineX]))) {
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
                                    areaSquares = mFieldAllyAreaSquaresArray;
                                }
                                else {
                                    areaSquares = mFieldEnemyAreaSquaresArray;
                                }
                                bool areThereAreaPoints = searchAreaPointsSquares(areaSquares, lineXBefore, lineYBefore, team, false);
                                
                                // 領域が確認できたら...
                                if (areThereAreaPoints) {
                                    // 領域ポイントに反映
                                    if (team == TileStatus::ALLY) {
                                        mAllyAreaPoints = 0;
                                        mAllyPoints -= mAllyAreaPoints;
                                        
                                        // 仮の領域への指定を反映
                                        mFieldAllyAreaSquaresArray = areaSquares;
                                        
                                    }
                                    else {
                                        mEnemyAreaPoints = 0;
                                        mEnemyPoints -= mEnemyAreaPoints;
                                        
                                        // 仮の領域への指定を反映
                                        mFieldEnemyAreaSquaresArray = areaSquares;
                                        
                                    }
                                    
                                    for (int j = 0; j < mFieldSizeH; j++) {
                                        for (int k = 0; k < mFieldSizeW; k++) {
                                            if (team == TileStatus::ALLY && mFieldAllyAreaSquaresArray[j][k]) {
                                                mAllyAreaPoints += abs(mFieldPointsArray[j][k]);
                                            }
                                            if (team == TileStatus::ENEMY && mFieldEnemyAreaSquaresArray[j][k]) {
                                                mEnemyAreaPoints += abs(mFieldPointsArray[j][k]);
                                            }
                                        }
                                    }
                                    
                                    mAllyPoints += mAllyAreaPoints;
                                    mEnemyPoints += mEnemyAreaPoints;
                                    
                                    // 領域を成す辺を反映
                                    Node* currentNode = &node;
                                    while(1) {
                                        mFieldAreaSideLinesArray[currentNode->getPosition().y][currentNode->getPosition().x] = true;
                                        
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
    int beAreaSquaresStatus = AreaStatus::NONE;
    Team::Type team;
    if (argBeTileStatus == TileStatus::ALLY) {
        beAreaSquaresStatus = AreaStatus::ALLY;
        team = Team::ALLY;
    }
    else {
        beAreaSquaresStatus = AreaStatus::ENEMY;
        team = Team::ENEMY;
    }
    
    // 領域内に指定
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
            break;
        }
        
        // 同じ色のマスにあたったら: continue
        if (mFieldStatusArray[y][x] == argBeTileStatus) {
            continue;
        }
        
        // 領域の探索のとき すでに領域内に指定されているマスなら: continue
        if (!argDeleteArea && (argAreaSquares[y][x] == true)) {
            continue;
        }
        
        // 領域の解除のとき 自チームの領域でなければ
        if (argDeleteArea && !argAreaSquares[y][x]) {
            if (mFieldStatusArray[y][x] == TileStatus::NONE) {
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
    if (mFieldStatusArray[argY][argX] == 1) {
        if (mAllyTeamColor == TeamColor::BLUE) {
            agentIDRectColor = Color(0, 128, 255); // 青
            rectColor = Color(0, 100, 200);
            
        }
        else if (mAllyTeamColor == TeamColor::RED) {
            agentIDRectColor = Color(255, 50, 0);  // 赤
            rectColor = Color(200, 10, 0);
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
        if (mFieldAgentsIDArray[argY][argX] != 0) {
            Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize/3*2, 20).draw(agentIDRectColor);
            mAgentIDTextFont(U"Ally").draw(argSquarePosition.x+2, argSquarePosition.y, Color(Palette::White));
        }
    }
    // 相手のタイルのとき
    if (mFieldStatusArray[argY][argX] == -1) {
        if (!mAllyTeamColor == TeamColor::BLUE) {
            agentIDRectColor = Color(0, 128, 255); // 青
            rectColor = Color(0, 100, 200);
        }
        else if (!mAllyTeamColor == TeamColor::RED) {
            agentIDRectColor = Color(255, 50, 0);  // 赤
            rectColor = Color(200, 10, 0);
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
        
        if (mFieldAgentsIDArray[argY][argX] != 0) {
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
    if (mFieldAllyAreaSquaresArray[argY][argX]) {
        if (mAllyTeamColor == TeamColor::BLUE) {
            rectColor = Color(150, 150, 255);   // 青
            
        }
        else if (mAllyTeamColor == TeamColor::RED) {
            rectColor = Color(255, 150, 150);   // 赤
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
    }
    // 相手の領域のとき
    if (mFieldEnemyAreaSquaresArray[argY][argX]) {
        if (!mAllyTeamColor == TeamColor::BLUE) {
            rectColor = Color(150, 150, 255);   // 青
        }
        else if (!mAllyTeamColor == TeamColor::RED) {
            rectColor = Color(255, 150, 150);   // 赤
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
    }
    // 両チームの領域のとき
    if (mFieldAllyAreaSquaresArray[argY][argX] && mFieldEnemyAreaSquaresArray[argY][argX]) {
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
    unsigned long int textLength = to_string(mFieldPointsArray[argY][argX]).length();
    
    Color textColor = Palette::Black;
    if (rectColor != Palette::White) {
        textColor = Palette::White;
    }
    
    if (mFieldPointsArray[argY][argX] >= mBigPointsBorder) {    // 得点が上位20％(=mBigPointsBorder)以上なら太字
        mPointTextFontBold(mFieldPointsArray[argY][argX])
        .draw(argSquarePosition.x+mFieldSquareSize/2-textLength*(mFieldSquareSize-POINT_TEXT_BIG_FONT_SIZE)/3, argSquarePosition.y+mFieldSquareSize/2-(mFieldSquareSize-POINT_TEXT_BIG_FONT_SIZE)/2-5, Color(textColor));
    }
    else {                                                      // 得点が上位20％(=mBigPointsBorder)未満なら普通の字
        mPointTextFont(mFieldPointsArray[argY][argX])
        .draw(argSquarePosition.x+mFieldSquareSize/2-textLength*(mFieldSquareSize-POINT_TEXT_FONT_SIZE)/3, argSquarePosition.y+mFieldSquareSize/2-(mFieldSquareSize-POINT_TEXT_FONT_SIZE)/2-5, Color(textColor));
    }
}

/**
 Field::agenrMovement:
 エージェントの行動
 エージェントの移動、タイルの除去などを行う。
 相手チームの領域の辺を成すタイルを除去するときは、相手チームの領域の解除ができるかどうかも調べる。
 最後に、エージェントの行動の番号を返す。
 
 @param argX        エージェントの移動先のマスのx座標
 @param argY        エージェントの移動先のマスのy座標
 @param argBeforeX  エージェントの移動前のマスのx座標
 @param argBeforeY  エージェントの移動前のマスのy座標
 @return エージェントの行動の番号（0~16）　field.hppのAgentActNumbersに定義済み
 */
int Field::agentMovement(int argX, int argY, int argBeforeX, int argBeforeY) {
    int team = mFieldStatusArray[argBeforeY][argBeforeX];
    int returnActNumber = 0;
    
    if (Vec2{argY, argX} != Vec2{-1, -1}) {
        // （動かすエージェントから見て）相手のタイルなら タイルを除去
        if (mFieldStatusArray[argY][argX] == (-1)*mFieldStatusArray[argBeforeY][argBeforeX] && mFieldAgentsIDArray[argY][argX] == 0) {
            mFieldStatusArray[argY][argX] = 0;
            
            // 相手から点数を引く
            if (mCurrentAgentID > 0) {
                mEnemyTilePoints -= mFieldPointsArray[argY][argX];
            }
            else {
                mAllyTilePoints -= mFieldPointsArray[argY][argX];
            }
            
            // 相手チームの領域に隣接していたら、相手チームの領域を再計算
            for (int i = 0; i < 4; i++) {
                if (argX+gSearchTileDirections[i].x < 0 || argY+gSearchTileDirections[i].y < 0 ||
                    argX+gSearchTileDirections[i].x > mFieldSizeW-1 || argY+gSearchTileDirections[i].y > mFieldSizeH-1) {
                    continue;
                }
                
                // 領域の辺を成すタイルなら、除去後も領域が成立するか確認
                // 成立しなければ、領域を解除する
                if ((team == TileStatus::ENEMY && mFieldAllyAreaSquaresArray[argY+gSearchTileDirections[i].y][argX+gSearchTileDirections[i].x]) ||
                    (team == TileStatus::ALLY && mFieldEnemyAreaSquaresArray[argY+gSearchTileDirections[i].y][argX+gSearchTileDirections[i].x])) {
                    if (mFieldAreaSideLinesArray[argY][argX]) {
                        
                        // 味方チームのエージェントが相手チームのタイルを除去するとき
                        if (mCurrentAgentID > 0) {
                            vector<vector<bool>> areaSquares = mFieldEnemyAreaSquaresArray;
                            bool areThereArea = searchAreaPointsSquares(areaSquares, argX+gSearchTileDirections[i].x, argY+gSearchTileDirections[i].y, TileStatus::ENEMY, true);
                            
                            if (!areThereArea) {
                                // 領域ポイントに反映
                                for (int j = 0; j < mFieldSizeH; j++) {
                                    for (int k = 0; k < mFieldSizeW; k++) {
                                        if (mFieldEnemyAreaSquaresArray[j][k] && !areaSquares[j][k]) {
                                            mEnemyAreaPoints -= abs(mFieldPointsArray[j][k]);
                                        }
                                    }
                                }
                                
                                mFieldEnemyAreaSquaresArray = areaSquares;
                                
                                // もう一度確認する（このタイルを除去しても領域が成立する場合もあるため）
                                // 成立するなら領域を解除しない
                                bool areaRecheck = searchAreaPointsSquares(areaSquares, argX, argY, TileStatus::ENEMY, false);
                                
                                if (areaRecheck) {
                                    // 領域ポイントに反映
                                    for (int j = 0; j < mFieldSizeH; j++) {
                                        for (int k = 0; k < mFieldSizeW; k++) {
                                            if (!mFieldEnemyAreaSquaresArray[j][k] && areaSquares[j][k]) {
                                                mEnemyAreaPoints += abs(mFieldPointsArray[j][k]);
                                            }
                                        }
                                    }
                                    
                                    mFieldEnemyAreaSquaresArray = areaSquares;
                                }
                            }
                        }
                        // 相手チームのエージェントが味方チームのタイルを除去するとき
                        else if (mCurrentAgentID < 0) {
                            vector<vector<bool>> areaSquares = mFieldAllyAreaSquaresArray;
                            bool areThereArea = searchAreaPointsSquares(areaSquares, argX+gSearchTileDirections[i].x, argY+gSearchTileDirections[i].y, TileStatus::ALLY, true);
                            
                            if (!areThereArea) {
                                // 領域ポイントに反映
                                for (int j = 0; j < mFieldSizeH; j++) {
                                    for (int k = 0; k < mFieldSizeW; k++) {
                                        if (mFieldAllyAreaSquaresArray[j][k] && !areaSquares[j][k]) {
                                            mAllyAreaPoints -= abs(mFieldPointsArray[j][k]);
                                        }
                                    }
                                }
                                
                                mFieldAllyAreaSquaresArray = areaSquares;
                                
                                // もう一度確認する（このタイルを除去しても領域が成立する場合もあるため）
                                // 成立するなら領域を解除しない
                                bool areaRecheck = searchAreaPointsSquares(areaSquares, argX, argY, TileStatus::ALLY, false);
                                
                                if (areaRecheck) {
                                    // 領域ポイントに反映
                                    for (int j = 0; j < mFieldSizeH; j++) {
                                        for (int k = 0; k < mFieldSizeW; k++) {
                                            if (!mFieldAllyAreaSquaresArray[j][k] && areaSquares[j][k]) {
                                                mAllyAreaPoints += abs(mFieldPointsArray[j][k]);
                                            }
                                        }
                                    }
                                    
                                    mFieldAllyAreaSquaresArray = areaSquares;
                                }
                            }
                        }
                    }
                    else {
                        if (mCurrentAgentID > 0) {
                            vector<vector<bool>> areaSquares = mFieldAllyAreaSquaresArray;
                            bool areThereArea = searchAreaPointsSquares(areaSquares, argX+gSearchTileDirections[i].x, argY+gSearchTileDirections[i].y, TileStatus::ALLY, false);
                            
                            if (areThereArea) {
                                mFieldAllyAreaSquaresArray = areaSquares;
                            }
                        }
                        else if (mCurrentAgentID < 0) {
                            vector<vector<bool>> areaSquares = mFieldEnemyAreaSquaresArray;
                            bool areThereArea = searchAreaPointsSquares(areaSquares, argX+gSearchTileDirections[i].x, argY+gSearchTileDirections[i].y, TileStatus::ENEMY, false);
                            
                            if (areThereArea) {
                                mFieldEnemyAreaSquaresArray = areaSquares;
                            }
                        }
                    }
                }
            }
            
            // 返す行動番号の取得
            if (argY-argBeforeY == -1) {
                if (argX-argBeforeX == -1) {
                    returnActNumber = AgentActNumbers::REMOVE_LEFT_TOP;     // 左上を除去
                }
                else if (argX-argBeforeX == 0) {
                    returnActNumber = AgentActNumbers::REMOVE_TOP;          // 上を除去
                }
                else if (argX-argBeforeX == 1) {
                    returnActNumber = AgentActNumbers::REMOVE_RIGHT_TOP;    // 右上を除去
                }
            }
            else if (argY-argBeforeY == 0) {
                if (argX-argBeforeX == -1) {
                    returnActNumber = AgentActNumbers::REMOVE_LEFT;         // 左を除去
                }
                else if (argX-argBeforeX == 1) {
                    returnActNumber = AgentActNumbers::REMOVE_RIGHT;        // 右を除去
                }
            }
            else if (argY-argBeforeY == 1) {
                if (argX-argBeforeX == -1) {
                    returnActNumber = AgentActNumbers::REMOVE_LEFT_BOTTOM;  // 左下を除去
                }
                else if (argX-argBeforeX == 0) {
                    returnActNumber = AgentActNumbers::REMOVE_BOTTOM;       // 下を除去
                }
                else if (argX-argBeforeX == 1) {
                    returnActNumber = AgentActNumbers::REMOVE_RIGHT_BOTTOM; // 右下を除去
                }
            }
        }
        // それ以外ならマスにエージェントを移動させタイルを置く
        else if (mFieldAgentsIDArray[argY][argX] == 0) {
            mCurrentSquarePosition = {argX, argY};
            
            if (mFieldStatusArray[argY][argX] != mFieldStatusArray[argBeforeY][argBeforeX]) {
                mFieldStatusArray[argY][argX] = mFieldStatusArray[argBeforeY][argBeforeX];
                
                if (mCurrentAgentID > 0) {
                    if (mFieldAllyAreaSquaresArray[argY][argX]) {
                        mFieldAllyAreaSquaresArray[argY][argX] = false;
                        mAllyAreaPoints -= abs(mFieldPointsArray[argY][argX]);
                    }
                    
                    mAllyTilePoints += mFieldPointsArray[argY][argX];       // タイルポイントにマスの得点を加算
                }
                else {
                    if (mFieldEnemyAreaSquaresArray[argY][argX]) {
                        mFieldEnemyAreaSquaresArray[argY][argX] = false;
                        mEnemyAreaPoints -= abs(mFieldPointsArray[argY][argX]);
                    }
                    
                    mEnemyTilePoints += mFieldPointsArray[argY][argX];      // タイルポイントにマスの得点を加算
                }
            }
            
            mFieldAgentsIDArray[argY][argX] = mCurrentAgentID;
            mFieldAgentsIDArray[argBeforeY][argBeforeX] = 0;
            
            // 領域ポイントができているか確認
            searchAreaPoints(argX, argY);
            
            // 返す行動番号の取得
            if (argY-argBeforeY == -1) {
                if (argX-argBeforeX == -1) {
                    returnActNumber = AgentActNumbers::GOTO_LEFT_TOP;     // 左上を除去
                }
                else if (argX-argBeforeX == 0) {
                    returnActNumber = AgentActNumbers::GOTO_TOP;          // 上を除去
                }
                else if (argX-argBeforeX == 1) {
                    returnActNumber = AgentActNumbers::GOTO_RIGHT_TOP;    // 右上を除去
                }
            }
            else if (argY-argBeforeY == 0) {
                if (argX-argBeforeX == -1) {
                    returnActNumber = AgentActNumbers::GOTO_LEFT;         // 左を除去
                }
                else if (argX-argBeforeX == 1) {
                    returnActNumber = AgentActNumbers::GOTO_RIGHT;        // 右を除去
                }
            }
            else if (argY-argBeforeY == 1) {
                if (argX-argBeforeX == -1) {
                    returnActNumber = AgentActNumbers::GOTO_LEFT_BOTTOM;  // 左下を除去
                }
                else if (argX-argBeforeX == 0) {
                    returnActNumber = AgentActNumbers::GOTO_BOTTOM;       // 下を除去
                }
                else if (argX-argBeforeX == 1) {
                    returnActNumber = AgentActNumbers::GOTO_RIGHT_BOTTOM; // 右下を除去
                }
            }
        }
    }
    
    mAllyPoints = mAllyTilePoints + mAllyAreaPoints;
    mEnemyPoints = mEnemyTilePoints + mEnemyAreaPoints;
    
    return returnActNumber;
}

/**
 Field::setCurrentAgent:
 選択するエージェントの変更処理
 
 @param argX カーソルで選択されたx座標
 @param argY カーソルで選択されたy座標
 */
void Field::setCurrentAgent(int argX, int argY) {
    // 新しくエージェントを選択するとき
    if (mFieldAgentsIDArray[argY][argX] != 0) {
        if (mCurrentAgentID == mFieldAgentsIDArray[argY][argX]) {
            mCurrentSquarePosition = {-1, -1};
            mCurrentAgentID = 0;
        }
        else {
            mCurrentSquarePosition = {argX, argY};
            mCurrentAgentID = mFieldAgentsIDArray[argY][argX];
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
    Vec2 currentSquarePositionBefore = mCurrentSquarePosition;
    
    for (int y = 0; y < mFieldSizeH; y++) {
        for (int x = 0; x < mFieldSizeW; x++) {
            // マスの描画位置
            int squarePositionX = mFieldLeftmostPoint+x*mFieldSquareSize;
            int squarePositionY = mFieldTopmostPoint+y*mFieldSquareSize;
            
            // マスの塗りつぶし（どちらかのチームのタイルの場合）
            rectColor = Palette::White;
            fillSquare(Vec2{squarePositionX, squarePositionY}, x, y, rectColor);
            
            // 領域の塗りつぶし（マスが囲まれている場合）
            if (mFieldStatusArray[y][x] == TileStatus::NONE) {
                fillAreaSquare(Vec2{squarePositionX, squarePositionY}, x, y);
            }
            
            // 点数の表示
            printSquarePoint(Vec2{squarePositionX, squarePositionY}, x, y, rectColor);
            
            // 枠の表示
            if (isCursorOnTheSquare(Vec2(mFieldLeftmostPoint+x*mFieldSquareSize, mFieldTopmostPoint+y*mFieldSquareSize))) {
                // マウスが左クリックの状態かつ 選択されていたマスから移動できるマスならば、選択されているマスに設定
                // エージェントを選択されているマスに移動
                if (MouseL.pressed() && !mMousePressing) {
                    mMousePressing = true;      // 左クリックが押された瞬間だけ
                    
                    // 周囲8方向のタイルの調査
                    for (int i=0; i<DIRECTIONS; i++) {
                        // エージェントの行動（移動・除去）を操作するとき
                        if (currentSquarePositionBefore + gMoveDirections[i] == Vec2{x, y}) {
                            if (currentSquarePositionBefore != Vec2{-1, -1}) {
                                // エージェントの行動に反映
                                // 移動, 削除
                                cout << "行動番号: " << agentMovement(x, y, currentSquarePositionBefore.x, currentSquarePositionBefore.y) << endl;
                            }
                            
                            break;
                        }
                    }
                    
                    // 新しくエージェントを選択するときの処理
                    setCurrentAgent(x, y);
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
    
    // フィールドの枠の描画
    Rect(mFieldLeftmostPoint, mFieldTopmostPoint, mFieldLineLengthW, mFieldLineLengthH).drawFrame(0, 4, Color(Palette::Black));
    
    return;
}


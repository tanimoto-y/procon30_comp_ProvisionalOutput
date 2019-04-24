#include "field.hpp"

// コンストラクタ（初期化も行う）
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

// フィールドの設定
void Field::setField(const vector<vector<int>> &argFieldPointsArray, const vector<vector<int>> &argFieldStatusArray) {
    mFieldPointsArray = argFieldPointsArray;    // フィールドの得点
    mFieldStatusArray = argFieldStatusArray;    // フィールドの状況
    
    int maxPoint = -16, minPoint = 16;
    int allyAgentsCount = 0, enemyAgentsCount = 0;
    
    for (int h = 0; h < mFieldSizeH; h++) {
        mFieldAgentsIDArray.push_back(vector<int>(mFieldSizeW, 0));
        
        mFieldAllyAreaSquaresArray.push_back(vector<bool>(mFieldSizeW, false));
        mFieldEnemyAreaSquaresArray.push_back(vector<bool>(mFieldSizeW, false));
        
        mFieldAreaSideLinesArray.push_back(vector<bool>(mFieldSizeW, false));
        for (int w = 0; w < mFieldSizeW; w++) {
            if (mFieldStatusArray[h][w] == 1) {
                allyAgentsCount ++;
                mFieldAgentsIDArray[h][w] = allyAgentsCount;    // エージェントにIDを振り分ける
                
                mAllyTilePoints += mFieldPointsArray[h][w];     // 初期段階の合計得点の算出
            }
            else if (mFieldStatusArray[h][w] == -1) {
                enemyAgentsCount ++;
                mFieldAgentsIDArray[h][w] = -enemyAgentsCount;  // エージェントにIDを振り分ける
                
                mEnemyTilePoints += mFieldPointsArray[h][w];    // 初期段階の合計得点の算出
            }
            
            // 最大値と最小値の検出
            if (mFieldPointsArray[h][w] > maxPoint) {
                maxPoint = mFieldPointsArray[h][w];
            }
            if (mFieldPointsArray[h][w] < minPoint) {
                minPoint = mFieldPointsArray[h][w];
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

// フィールドの1マスの大きさを返す
int Field::getFieldSquareSize() {
    return mFieldSquareSize;
}

// 合計得点を返す
int Field::getTotalPoints(const Team::Type argTeam) {
    if (argTeam == Team::ALLY) {    // 味方
        return mAllyPoints;
    }
    else {                          // 敵
        return mEnemyPoints;
    }
}

// タイルポイントを返す
int Field::getTilePoints(const Team::Type argTeam) {
    if (argTeam == Team::ALLY) {    // 味方
        return mAllyTilePoints;
    }
    else {                          // 敵
        return mEnemyTilePoints;
    }
}

// 領域ポイントを返す
int Field::getAreaPoints(const Team::Type argTeam) {
    if (argTeam == Team::ALLY) {    // 味方
        return mAllyAreaPoints;
    }
    else {                          // 敵
        return mEnemyAreaPoints;
    }
}

// Fieldクラスで使うフォントの設定
void Field::setFont(const Font argPointTextFont, const Font argPointTextFontBold, const Font argAgentIDTextFont) {
    mPointTextFont = argPointTextFont;
    mPointTextFontBold = argPointTextFontBold;
    mAgentIDTextFont = argAgentIDTextFont;
}

// マスの上にカーソルがあるか判断
bool Field::isCursorOnTheSquare(const Vec2 argPosition) {
    if (Cursor::Pos().x > argPosition.x && Cursor::Pos().y > argPosition.y
        && Cursor::Pos().x < argPosition.x+mFieldSquareSize && Cursor::Pos().y < argPosition.y+mFieldSquareSize) {
        return true;        // あればtrue
    }
    else {
        return false;       // なければfalse
    }
}

// 領域ポイントが存在するか（=マスを囲めているか）確認（再帰関数）
// (argStartW, argStartH)を始点とし同じチームのタイルをたどる
void Field::searchAreaPointsSide(vector<vector<bool>> argFieldMark, const int argStartW, const int argStartH, const int argW, const int argH, Node* argParentNode) {
    int w, h;
    int nodes = 0;
    Node node(argW, argH, argParentNode);
    
    if (argParentNode != nullptr) {
        argParentNode->setChildNode(&node);
    }
    
    // どちらのチームが対象か
    TileStatus::Type team;
    if (mFieldStatusArray[argStartH][argStartW] == TileStatus::ALLY) {
        team = TileStatus::ALLY;
    }
    else {
        team = TileStatus::ENEMY;
    }
    
    for (int i = 0; i < DIRECTIONS; i++) {
        w = argW;
        h = argH;
        
        // 「とどまる」は除く
        if (gMoveDirections[i].x == 0 && gMoveDirections[i].y == 0) {
            continue;
        }
            
        // 対象となるマスをずらす
        // フィールドから出たらcontinue
        if (w+gMoveDirections[i].x < 0 || h+gMoveDirections[i].y < 0 || w+gMoveDirections[i].x > mFieldSizeW-1 || h+gMoveDirections[i].y > mFieldSizeH-1) {
            continue;
        }
        
        w += gMoveDirections[i].x;
        h += gMoveDirections[i].y;
        
        // 周囲に自チームのものではないタイルがなければcontinue
        bool areThereOthersTiles = false;
        for (int j = 0; j < 4; j++) {
            if (w+gSearchTileDirections[j].x >= 0 && h+gSearchTileDirections[j].y >= 0 &&
                w+gSearchTileDirections[j].x <= mFieldSizeW-1 && h+gSearchTileDirections[j].y <= mFieldSizeH-1) {
                if (mFieldStatusArray[h+gSearchTileDirections[j].y][w+gSearchTileDirections[j].x] != team) {
                    areThereOthersTiles = true;
                    break;
                }
            }
        }
        if (!areThereOthersTiles) {
            continue;
        }
        
        // 違う色のマスならcontinue
        if (mFieldStatusArray[h][w] != team) {
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
                    
                if (parentNode->getPosition().x == w && parentNode->getPosition().y == h) {
                    if ((w == argStartW && h == argStartH && !argFieldMark[argStartH][argStartW]) ||
                        (w != argStartW && h != argStartH)) {
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
        if (argFieldMark[h][w]) {
            if (argFieldMark[argStartH][argStartW] && w == argStartW && h == argStartH && node.getTreeHigh() >= 3) {
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
                    if (searchingNode->getPosition().x == w || searchingNode->getPosition().y == h) {
                        slope = 0;      // 傾きがなければ0
                    }
                    else {
                        slope = (searchingNode->getPosition().y - h) / (searchingNode->getPosition().x - w);
                    }
                    
                    // 直線の向きを指定
                    int directionW, directionH;
                    if (searchingNode->getPosition().x > w) {
                        directionW = -1;            // 親ノードのタイルx座標 > このタイルのx座標
                    }
                    else if (searchingNode->getPosition().x < w) {
                        directionW = 1;             // 親ノードのタイルx座標 < このタイルのx座標
                    }
                    else {
                        directionW = 0;             // 親ノードのタイルx座標 == このタイルのx座標
                    }
                    
                    if (searchingNode->getPosition().y > h) {
                        directionH = -1;            // 親ノードのタイルy座標 > このタイルのy座標
                    }
                    else if (searchingNode->getPosition().y < h) {
                        directionH = 1;             // 親ノードのタイルy座標 < このタイルのy座標
                    }
                    else {
                        directionH = 0;             // 親ノードのタイルy座標 == このタイルのy座標
                    }
                    
                    // 直線の始点 = 親ノードのタイル
                    int lineW = searchingNode->getPosition().x, lineH = searchingNode->getPosition().y;
                    
                    whiteTiles = 0;
                    int lineWBefore, lineHBefore;
                    
                    // 直線をたどっていき、その間に自チームのタイルがないマスがいくつあるか数える
                    while(1) {
                        lineWBefore = lineW;
                        lineHBefore = lineH;
                        
                        if (directionW != 0) {
                            lineW += directionW;
                            lineH = slope * lineW + searchingNode->getPosition().y;
                        }
                        else {
                            lineH += directionH;
                        }
                        
                        // フィールドの外に出たらbreak
                        if (lineW < 0 || lineH < 0 || lineW > mFieldSizeW-1 || lineH > mFieldSizeH-1) {
                            break;
                        }
                        
                        // 自チームのタイルがなければカウント
                        // 条件: 自チームのタイルがなく、自チームの領域にも含まれていない
                        if (mFieldStatusArray[lineH][lineW] != team &&
                            ((team == TileStatus::ALLY && !mFieldAllyAreaSquaresArray[lineH][lineW]) ||
                            (team == TileStatus::ENEMY && !mFieldEnemyAreaSquaresArray[lineH][lineW]))) {
                            whiteTiles ++;
                        }
                        
                        // 目的のマスにたどり着いたときの処理
                        if (lineW == w && lineH == h) {
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
                                bool areThereAreaPoints = searchAreaPointsSquares(areaSquares, lineWBefore, lineHBefore, team, false);
                                
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
        if (!argFieldMark[argStartH][argStartW] && (abs(argStartW-w) >= 2 || abs(argStartH-h) >= 2)) {
            argFieldMark[argStartH][argStartW] = true;
        }
        
        // マスにマークを付ける
        argFieldMark[h][w] = true;
        
        searchAreaPointsSide(argFieldMark, argStartW, argStartH, w, h, &node);
        
        nodes ++;
    }
}

// 領域内のマスの探索
bool Field::searchAreaPointsSquares(vector<vector<bool>> &argAreaSquares, const int argStartW, const int argStartH, const int argBeTileStatus, const bool argDeleteArea) {
    int w, h;
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
        if (!argAreaSquares[argStartH][argStartW]) {
            argAreaSquares[argStartH][argStartW] = true;
        }
    }
    // 領域を解除（argDeleteArea == true）するとき
    else {
        argAreaSquares[argStartH][argStartW] = false;
    }
    
    // 周囲4方向のマスの確認
    for (int i = 0; i < 4; i++) {
        w = argStartW + gSearchTileDirections[i].x;
        h = argStartH + gSearchTileDirections[i].y;
        
        // フィールドから出たら: 領域ができていないためループから出てfalseを返す
        if (w < 0 || h < 0 || w > mFieldSizeW-1 || h > mFieldSizeH-1) {
            possible = false;
            break;
        }
        
        // 同じ色のマスにあたったら: continue
        if (mFieldStatusArray[h][w] == argBeTileStatus) {
            continue;
        }
        
        // 領域の探索のとき すでに領域内に指定されているマスなら: continue
        if (!argDeleteArea && (argAreaSquares[h][w] == true)) {
            continue;
        }
        
        // 領域の解除のとき 自チームの領域でなければ
        if (argDeleteArea && !argAreaSquares[h][w]) {
            if (mFieldStatusArray[h][w] == TileStatus::NONE) {
                possible = false;
            }
            continue;
        }
        
        // 周囲にまだ領域として指定していないマスがあったら: そのマスに移動
        // 領域の解除の場合は領域として指定されているマスがあったら
        bool nextTile = searchAreaPointsSquares(argAreaSquares, w, h, argBeTileStatus, argDeleteArea);
        
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

// 領域の探索
void Field::searchAreaPoints(const int argStartW, const int argStartH) {
    //cout << "-----------------------(" << argStartW << ',' << argStartH << ")-----------------------" << endl;
    vector<vector<bool>> fieldMark(mFieldSizeH, vector<bool>(mFieldSizeW, false));
    searchAreaPointsSide(fieldMark, argStartW, argStartH, argStartW, argStartH, nullptr);
}

// マスの塗りつぶし
void Field::fillSquare(const Vec2 argSquarePosition, const int argW, const int argH, Color& rectColor) {
    Color agentIDRectColor = Palette::White;
    
    // 味方のタイルのとき
    if (mFieldStatusArray[argH][argW] == 1) {
        if (mAllyTeamColor == TeamColor::BLUE) {
            agentIDRectColor = Color(0, 128, 255); // 青
            rectColor = Color(0, 100, 200);
            
        }
        else if (mAllyTeamColor == TeamColor::RED) {
            agentIDRectColor = Color(255, 50, 0);  // 赤
            rectColor = Color(200, 10, 0);
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
        if (mFieldAgentsIDArray[argH][argW] != 0) {
            Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize/3*2, 20).draw(agentIDRectColor);
            mAgentIDTextFont(U"Ally").draw(argSquarePosition.x+2, argSquarePosition.y, Color(Palette::White));
        }
    }
    // 相手のタイルのとき
    if (mFieldStatusArray[argH][argW] == -1) {
        if (!mAllyTeamColor == TeamColor::BLUE) {
            agentIDRectColor = Color(0, 128, 255); // 青
            rectColor = Color(0, 100, 200);
        }
        else if (!mAllyTeamColor == TeamColor::RED) {
            agentIDRectColor = Color(255, 50, 0);  // 赤
            rectColor = Color(200, 10, 0);
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
        
        if (mFieldAgentsIDArray[argH][argW] != 0) {
            Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize/3*2, 20).draw(agentIDRectColor);
            mAgentIDTextFont(U"Enemy").draw(argSquarePosition.x+2, argSquarePosition.y, Color(Palette::White));
        }
    }
}

// 領域の塗りつぶし
void Field::fillAreaSquare(const Vec2 argSquarePosition, const int argW, const int argH) {
    Color rectColor;
    
    // 味方の領域のとき
    if (mFieldAllyAreaSquaresArray[argH][argW]) {
        if (mAllyTeamColor == TeamColor::BLUE) {
            rectColor = Color(150, 150, 255);   // 青
            
        }
        else if (mAllyTeamColor == TeamColor::RED) {
            rectColor = Color(255, 150, 150);   // 赤
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
    }
    // 相手の領域のとき
    if (mFieldEnemyAreaSquaresArray[argH][argW]) {
        if (!mAllyTeamColor == TeamColor::BLUE) {
            rectColor = Color(150, 150, 255);   // 青
        }
        else if (!mAllyTeamColor == TeamColor::RED) {
            rectColor = Color(255, 150, 150);   // 赤
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
    }
    // 両チームの領域のとき
    if (mFieldAllyAreaSquaresArray[argH][argW] && mFieldEnemyAreaSquaresArray[argH][argW]) {
        rectColor = Color(200, 100, 200);       // 紫
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
    }
}

// タイル点数の表示
void Field::printSquarePoint(const Vec2 argSquarePosition, const int argW, const int argH, Color& rectColor) {
    unsigned long int textLength = to_string(mFieldPointsArray[argH][argW]).length();
    
    Color textColor = Palette::Black;
    if (rectColor != Palette::White) {
        textColor = Palette::White;
    }
    
    if (mFieldPointsArray[argH][argW] >= mBigPointsBorder) {    // 得点が上位20％(=mBigPointsBorder)以上なら太字
        mPointTextFontBold(mFieldPointsArray[argH][argW])
        .draw(argSquarePosition.x+mFieldSquareSize/2-textLength*(mFieldSquareSize-POINT_TEXT_BIG_FONT_SIZE)/3, argSquarePosition.y+mFieldSquareSize/2-(mFieldSquareSize-POINT_TEXT_BIG_FONT_SIZE)/2-5, Color(textColor));
    }
    else {                                                      // 得点が上位20％(=mBigPointsBorder)未満なら普通の字
        mPointTextFont(mFieldPointsArray[argH][argW])
        .draw(argSquarePosition.x+mFieldSquareSize/2-textLength*(mFieldSquareSize-POINT_TEXT_FONT_SIZE)/3, argSquarePosition.y+mFieldSquareSize/2-(mFieldSquareSize-POINT_TEXT_FONT_SIZE)/2-5, Color(textColor));
    }
}

// エージェントの行動
void Field::agentMovement(int argW, int argH, int argBeforeW, int argBeforeH) {
    int team = mFieldStatusArray[argBeforeH][argBeforeW];
    
    if (Vec2{argH, argW} != Vec2{-1, -1}) {
        // （動かすエージェントから見て）相手のタイルなら タイルを除去
        if (mFieldStatusArray[argH][argW] == (-1)*mFieldStatusArray[argBeforeH][argBeforeW] && mFieldAgentsIDArray[argH][argW] == 0) {
            mFieldStatusArray[argH][argW] = 0;
            
            // 相手から点数を引く
            if (mCurrentAgentID > 0) {
                mEnemyTilePoints -= mFieldPointsArray[argH][argW];
            }
            else {
                mAllyTilePoints -= mFieldPointsArray[argH][argW];
            }
            
            // 相手チームの領域に隣接していたら、相手チームの領域を再計算
            for (int i = 0; i < 4; i++) {
                if (argW+gSearchTileDirections[i].x < 0 || argH+gSearchTileDirections[i].y < 0 ||
                    argW+gSearchTileDirections[i].x > mFieldSizeW-1 || argH+gSearchTileDirections[i].y > mFieldSizeH-1) {
                    continue;
                }
                
                // 領域の辺を成すタイルなら、除去後も領域が成立するか確認
                // 成立しなければ、領域を解除する
                if ((team == TileStatus::ENEMY && mFieldAllyAreaSquaresArray[argH+gSearchTileDirections[i].y][argW+gSearchTileDirections[i].x]) ||
                    (team == TileStatus::ALLY && mFieldEnemyAreaSquaresArray[argH+gSearchTileDirections[i].y][argW+gSearchTileDirections[i].x])) {
                    if (mFieldAreaSideLinesArray[argH][argW]) {
                        
                        // 味方チームのエージェントが相手チームのタイルを除去するとき
                        if (mCurrentAgentID > 0) {
                            vector<vector<bool>> areaSquares = mFieldEnemyAreaSquaresArray;
                            bool areThereArea = searchAreaPointsSquares(areaSquares, argW+gSearchTileDirections[i].x, argH+gSearchTileDirections[i].y, TileStatus::ENEMY, true);
                            
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
                                bool areaRecheck = searchAreaPointsSquares(areaSquares, argW, argH, TileStatus::ENEMY, false);
                                
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
                            bool areThereArea = searchAreaPointsSquares(areaSquares, argW+gSearchTileDirections[i].x, argH+gSearchTileDirections[i].y, TileStatus::ALLY, true);
                            
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
                                bool areaRecheck = searchAreaPointsSquares(areaSquares, argW, argH, TileStatus::ALLY, false);
                                
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
                            bool areThereArea = searchAreaPointsSquares(areaSquares, argW+gSearchTileDirections[i].x, argH+gSearchTileDirections[i].y, TileStatus::ALLY, false);
                            
                            if (areThereArea) {
                                mFieldAllyAreaSquaresArray = areaSquares;
                            }
                        }
                        else if (mCurrentAgentID < 0) {
                            vector<vector<bool>> areaSquares = mFieldEnemyAreaSquaresArray;
                            bool areThereArea = searchAreaPointsSquares(areaSquares, argW+gSearchTileDirections[i].x, argH+gSearchTileDirections[i].y, TileStatus::ENEMY, false);
                            
                            if (areThereArea) {
                                mFieldEnemyAreaSquaresArray = areaSquares;
                            }
                        }
                    }
                }
            }
        }
        // それ以外ならマスにエージェントを移動させタイルを置く
        else if (mFieldAgentsIDArray[argH][argW] == 0) {
            mCurrentSquarePosition = {argW, argH};
            
            if (mFieldStatusArray[argH][argW] != mFieldStatusArray[argBeforeH][argBeforeW]) {
                mFieldStatusArray[argH][argW] = mFieldStatusArray[argBeforeH][argBeforeW];
                
                if (mCurrentAgentID > 0) {
                    if (mFieldAllyAreaSquaresArray[argH][argW]) {
                        mFieldAllyAreaSquaresArray[argH][argW] = false;
                        mAllyAreaPoints -= abs(mFieldPointsArray[argH][argW]);
                    }
                    
                    mAllyTilePoints += mFieldPointsArray[argH][argW];       // タイルポイントにマスの得点を加算
                }
                else {
                    if (mFieldEnemyAreaSquaresArray[argH][argW]) {
                        mFieldEnemyAreaSquaresArray[argH][argW] = false;
                        mEnemyAreaPoints -= abs(mFieldPointsArray[argH][argW]);
                    }
                    
                    mEnemyTilePoints += mFieldPointsArray[argH][argW];      // タイルポイントにマスの得点を加算
                }
            }
            
            mFieldAgentsIDArray[argH][argW] = mCurrentAgentID;
            mFieldAgentsIDArray[argBeforeH][argBeforeW] = 0;
            
            searchAreaPoints(argW, argH);
        }
    }
    
    mAllyPoints = mAllyTilePoints + mAllyAreaPoints;
    mEnemyPoints = mEnemyTilePoints + mEnemyAreaPoints;
}

// 選択するエージェントの変更処理
void Field::setCurrentAgent(int argW, int argH) {
    // 新しくエージェントを選択するとき
    if (mFieldAgentsIDArray[argH][argW] != 0) {
        if (mCurrentAgentID == mFieldAgentsIDArray[argH][argW]) {
            mCurrentSquarePosition = {-1, -1};
            mCurrentAgentID = 0;
        }
        else {
            mCurrentSquarePosition = {argW, argH};
            mCurrentAgentID = mFieldAgentsIDArray[argH][argW];
        }
    }
}

// フィールドの縦線・横線の描画
void Field::drawSquares() {
    for (int w = 0; w < mFieldSizeW; w++) {
        Line(w*mFieldSquareSize+mFieldLeftmostPoint, mFieldTopmostPoint,
             w*mFieldSquareSize+mFieldLeftmostPoint, mFieldBottommostPoint).draw(Color(Palette::Gray));
    }
    for (int h = 0; h < mFieldSizeH; h++) {
        Line(mFieldLeftmostPoint, h*mFieldSquareSize+mFieldTopmostPoint,
             mFieldRightmostPoint, h*mFieldSquareSize+mFieldTopmostPoint).draw(Color(Palette::Gray));
    }
}

// フィールドの描画
void Field::draw() {
    Color rectColor;
    
    // フィールドの縦線・横線の描画
    drawSquares();
    
    // 点数の表示
    // チーム色による塗りつぶし
    // エージェントIDの表示
    // マスの上にカーソルがあれば マスに枠を表示
    Vec2 currentSquarePositionBefore = mCurrentSquarePosition;
    
    for (int h = 0; h < mFieldSizeH; h++) {
        for (int w = 0; w < mFieldSizeW; w++) {
            // マスの描画位置
            int squarePositionX = mFieldLeftmostPoint+w*mFieldSquareSize;
            int squarePositionY = mFieldTopmostPoint+h*mFieldSquareSize;
            
            // マスの塗りつぶし（どちらかのチームのタイルの場合）
            rectColor = Palette::White;
            fillSquare(Vec2{squarePositionX, squarePositionY}, w, h, rectColor);
            
            // 領域の塗りつぶし（マスが囲まれている場合）
            if (mFieldStatusArray[h][w] == TileStatus::NONE) {
                fillAreaSquare(Vec2{squarePositionX, squarePositionY}, w, h);
            }
            
            // 点数の表示
            printSquarePoint(Vec2{squarePositionX, squarePositionY}, w, h, rectColor);
            
            // 枠の表示
            if (isCursorOnTheSquare(Vec2(mFieldLeftmostPoint+w*mFieldSquareSize, mFieldTopmostPoint+h*mFieldSquareSize))) {
                // マウスが左クリックの状態かつ 選択されていたマスから移動できるマスならば、選択されているマスに設定
                // エージェントを選択されているマスに移動
                if (MouseL.pressed() && !mMousePressing) {
                    mMousePressing = true;      // 左クリックが押された瞬間だけ
                    
                    // 周囲8方向のタイルの調査
                    for (int i=0; i<DIRECTIONS; i++) {
                        // エージェントの行動（移動・除去）を操作するとき
                        if (currentSquarePositionBefore + gMoveDirections[i] == Vec2{w, h}) {
                            if (currentSquarePositionBefore != Vec2{-1, -1}) {
                                // エージェントの行動に反映
                                // 移動, 削除
                                agentMovement(w, h, currentSquarePositionBefore.x, currentSquarePositionBefore.y);
                            }
                            
                            break;
                        }
                    }
                    
                    // 新しくエージェントを選択するときの処理
                    setCurrentAgent(w, h);
                }
                else {
                    // カーソルが上にあればマスに黄色い枠を表示
                    Rect(squarePositionX, squarePositionY, mFieldSquareSize, mFieldSquareSize).drawFrame(5, 0, Color(Palette::Yellow));
                }
            }
            
            // マスが選択されている状態ならば 橙色の枠を表示
            if (Vec2{w, h} == mCurrentSquarePosition) {
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

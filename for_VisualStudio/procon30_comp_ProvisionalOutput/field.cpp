#include "field.hpp"

// �R���X�g���N�^�i���������s���j
/**
 Field::Field
 Field�N���X�̃R���X�g���N�^�i���������s���j�B
 
 @param argFieldPointsArray �t�B�[���h�̓��_�̔z��
 @param argFieldStatusArray �t�B�[���h�̏�Ԃ������z��
 @param position            �t�B�[���h�̑傫���������z��i��F{10, 10}�j
 @param argAllyTeamColor    �����`�[���̐F�iTeamColor::RED or TeamColor::BLUE�j
 */
Field::Field(const vector<vector<int>> &argFieldPointsArray, const vector<vector<int>> &argFieldStatusArray, const Vec2 position, const bool argAllyTeamColor) {
    // �t�B�[���h�̑傫���̐ݒ�
    mFieldSizeW = position.x;
    mFieldSizeH = position.y;
    
    // �t�B�[���h�̓��_�A�󋵂ƃG�[�W�F���gID��ݒ�
    setField(argFieldPointsArray, argFieldStatusArray);
    
    // �����`�[���̐F
    mAllyTeamColor = argAllyTeamColor;
    
    // �t�B�[���h�̘g�̒����̌���
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
    
    // �t�B�[���h�̏c���E�����̕`��
    mFieldLeftmostPoint      = WINDOW_SIZE_W/2-mFieldLineLengthW/2;
    mFieldTopmostPoint       = WINDOW_SIZE_H/2-mFieldLineLengthH/2;
    mFieldRightmostPoint     = WINDOW_SIZE_W/2+mFieldLineLengthW/2;
    mFieldBottommostPoint    = WINDOW_SIZE_H/2+mFieldLineLengthH/2;
}

/**
 Field::setField
 �t�B�[���h�̐ݒ���s���B
 �t�B�[���h�̓��_�A�󋵂��v���C�x�[�g�z��ɃR�s�[���A�G�[�W�F���g��ID��U�蕪����B
 ID�̓t�B�[���h�̍��ォ�珇�ɐU�蕪�����A�����̃G�[�W�F���g��ID��1����オ���Ă����A����̃G�[�W�F���g��ID��-1���牺�����Ă����B
 ���̊֐��́A��{�I��Field�R���X�g���N�^����Ăяo�����B
 ��j�G�[�W�F���g���e�`�[��3�l�̂Ƃ��A�����̃G�[�W�F���gID��1?3�A����̃G�[�W�F���gID��-1?-3
 
 @param argFieldPointsArray �t�B�[���h�̓��_�̔z��
 @param argFieldStatusArray �t�B�[���h�̏�Ԃ������z��
 */
void Field::setField(const vector<vector<int>> &argFieldPointsArray, const vector<vector<int>> &argFieldStatusArray) {
    mFieldPointsArray = argFieldPointsArray;    // �t�B�[���h�̓��_
    mFieldStatusArray = argFieldStatusArray;    // �t�B�[���h�̏�
    
    // ���_�̏�����
    mAllyScore  = Score{0, 0};
    mEnemyScore = Score{0, 0};
    
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
                mFieldAgentsIDArray[y][x] = allyAgentsCount;    // �G�[�W�F���g��ID��U�蕪����
                
                mAllyScore.tile += mFieldPointsArray[y][x];     // �����i�K�̍��v���_�̎Z�o
            }
            else if (mFieldStatusArray[y][x] == -1) {
                enemyAgentsCount ++;
                mFieldAgentsIDArray[y][x] = -enemyAgentsCount;  // �G�[�W�F���g��ID��U�蕪����
                
                mEnemyScore.tile += mFieldPointsArray[y][x];    // �����i�K�̍��v���_�̎Z�o
            }
            
            // �ő�l�ƍŏ��l�̌��o
            if (mFieldPointsArray[y][x] > maxPoint) {
                maxPoint = mFieldPointsArray[y][x];
            }
            if (mFieldPointsArray[y][x] < minPoint) {
                minPoint = mFieldPointsArray[y][x];
            }
        }
    }
    
    // �傫���_���̊�����߂�i�ő�l-�ŏ��l�̏��20���j
    mBigPointsBorder = minPoint+(maxPoint-minPoint)*0.8;
    
    // ���v�G�[�W�F���g��
    mTotalTeamAgents = allyAgentsCount;

	mAllyAgentsActNumbers.resize(mTotalTeamAgents, AgentActNumbers::STAY);  // �����G�[�W�F���g�̍s���ԍ��̏�����
	mEnemyAgentsActNumbers.resize(mTotalTeamAgents, AgentActNumbers::STAY); // ����G�[�W�F���g�̍s���ԍ��̏�����
}


/**
 Field::getAgentsPosition:
 �t�B�[���h����G�[�W�F���g�̈ʒu���擾����B
 �v�f�ԍ��̓G�[�W�F���gID�Ɠ����ŁA����̃G�[�W�F���g�̏ꍇ�̓G�[�W�F���gID�̐�Βl���v�f�ԍ��ɂȂ�B
 ��: �����1�l�ڂ̃G�[�W�F���g�i�G�[�W�F���gID -1�j��x���W -> argEnemyAgentsPosition[1].first
 �e�z��͎Q�Ɠn���ł��邽�߁A���̊֐������s���邱�ƂŎ����I�ɍ��W����������B

 @param argAllyAgentsPosition  �����̃G�[�W�F���g�̍��W�z��
 @param argEnemyAgentsPosition ����̃G�[�W�F���g�̍��W�z��
 */
void Field::getAgentsPosition(vector<pair<int, int>> &argAllyAgentsPosition, vector<pair<int, int>> &argEnemyAgentsPosition) {
    int countAllyAgents = 0, countEnemyAgents = 0;
    
    argAllyAgentsPosition.resize(mTotalTeamAgents);
    argEnemyAgentsPosition.resize(mTotalTeamAgents);
    
    for (int y = 0; y < mFieldSizeH; y++) {
        for (int x = 0; x < mFieldSizeW; x++) {
            // �����̃G�[�W�F���g
            if (mFieldAgentsIDArray[y][x] > 0) {
                argAllyAgentsPosition[countAllyAgents] = {x, y};
                countAllyAgents ++;
            }
            // ����̃G�[�W�F���g
            if (mFieldAgentsIDArray[y][x] < 0) {
                argEnemyAgentsPosition[countEnemyAgents] = {x, y};
                countEnemyAgents ++;
            }
        }
    }
}

/**
 Field::getTotalTeamAgents:
 �e�`�[���̃G�[�W�F���g�̐l����Ԃ�

 @return �e�`�[���̃G�[�W�F���g�̐l��
 */
int Field::getTotalTeamAgents() {
    return mTotalTeamAgents;
}

/**
 Field::getFieldSquareSize:
 �t�B�[���h��1�}�X�̑傫���imFieldSquareSize�j��Ԃ��B
 
 @return mFieldSquareSize�i�t�B�[���h��1�}�X�̑傫���j
 */
int Field::getFieldSquareSize() {
    return mFieldSquareSize;
}

/**
 Field::getTotalPoints:
 ���v���_(mAllyScore.total, mEnemyScore.total)��Ԃ��B
 
 @param argTeam ���_���擾�������`�[���̔ԍ�(���� Team::ALLY, ���� Team::ENEMY)
 @return ���v���_
 */
int Field::getTotalPoints(const Team::Type argTeam) {
    if (argTeam == Team::ALLY) {    // ����
        return mAllyScore.total();
    }
    else {                          // �G
        return mEnemyScore.total();
    }
}

/**
 Field::getTilePoints:
 �^�C���|�C���g(mAllyScore.tile, mEnemyScore.tile)��Ԃ��B
 
 @param argTeam ���_���擾�������`�[���̔ԍ�(���� Team::ALLY, ���� Team::ENEMY)
 @return �^�C���|�C���g
 */
int Field::getTilePoints(const Team::Type argTeam) {
    if (argTeam == Team::ALLY) {    // ����
        return mAllyScore.tile;
    }
    else {                          // �G
        return mEnemyScore.tile;
    }
}

/**
 Field::getAreaPoints:
 �̈�|�C���g(mAllyScore.area, mEnemyScore.area)��Ԃ��B
 
 @param argTeam ���_���擾�������`�[���̔ԍ�(���� Team::ALLY, ���� Team::ENEMY)
 @return �̈�|�C���g
 */
int Field::getAreaPoints(const Team::Type argTeam) {
    if (argTeam == Team::ALLY) {    // ����
        return mAllyScore.area;
    }
    else {                          // �G
        return mEnemyScore.area;
    }
}

/**
 Field::setFont:
 Field�N���X�Ŏg���t�H���g�̐ݒ�
 
 @param argPointTextFont\       ���ʂ̃t�H���g
 @param argPointTextFontBold    �����̃t�H���g
 @param argAgentIDTextFont      �G�[�W�F���g��ID��\�����鎞�Ɏg���t�H���g
 */
void Field::setFont(const Font argPointTextFont, const Font argPointTextFontBold, const Font argAgentIDTextFont) {
    mPointTextFont = argPointTextFont;
    mPointTextFontBold = argPointTextFontBold;
    mAgentIDTextFont = argAgentIDTextFont;
}

/**
 Field::isCursorOnTheSquare
 �}�X�̏�ɃJ�[�\�������邩���f
 
 @param argPosition ���m����}�X�̍��W�iVec2�^, ��: {0, 0}�j
 @return �J�[�\���������true, �Ȃ����false
 */
bool Field::isCursorOnTheSquare(const Vec2 argPosition) {
    if (Cursor::Pos().x > argPosition.x && Cursor::Pos().y > argPosition.y
        && Cursor::Pos().x < argPosition.x+mFieldSquareSize && Cursor::Pos().y < argPosition.y+mFieldSquareSize) {
        return true;        // �����true
    }
    else {
        return false;       // �Ȃ����false
    }
}

/**
 Field::searchAreaPointsSide:
 *�ċA�֐�
 �t�B�[���h��ɗ̈悪���݂��邩�ǂ����𔻒f���A���݂�����̈�|�C���g��ݒ肷��B
 �����Ŏw�肵�����W�̃^�C�����瓯���F�̃^�C�������ǂ��Ă����A�����^�C�����x�ʂ邱�ƂȂ��ŏ��̃^�C���܂Ŗ߂��Ă���ꂽ��A�t�B�[���h��ɗ̈悪���݂���Ɖ��肷��B
 ���̌�A�̈�Ɖ��肵���^�C���̊ԂɁA����`�[�����邢�͔����^�C�������݂��邩�������A���݂�����̈�Ƃ��ĔF�肷��B
 �̈�̑��݂��F�߂�ꂽ��AsearchAreaPointsSquares�ŗ̈���̃}�X�ɗ̈���ł��邱�Ƃ�ݒ肷��B
 
 @param argFieldMark    �t�B�[���h�̗̈�ɔF�肵�����ǂ����i�ꎞ�I�Ȕz��j
 @param argStartX       �̈�T���̎n�_�̃}�X��x���W
 @param argStartY       �̈�T���̎n�_�̃}�X��y���W
 @param argX            ���ݒT�����̃}�X��x���W
 @param argY            ���ݒT�����̃}�X��y���W
 @param argParentNode   �e�m�[�h(Node�N���X)�̃|�C���^
 */
void Field::searchAreaPointsSide(vector<vector<bool>> argFieldMark, const int argStartX, const int argStartY, const int argX, const int argY, Node* argParentNode) {
    int x, y;
    int nodes = 0;
    Node node(argX, argY, argParentNode);
    
    if (argParentNode != nullptr) {
        argParentNode->setChildNode(&node);
    }
    
    // �ǂ���̃`�[�����Ώۂ�
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
        
        // �u�Ƃǂ܂�v�͏���
        if (gMoveDirections[i].x == 0 && gMoveDirections[i].y == 0) {
            continue;
        }
        
        // �ΏۂƂȂ�}�X�����炷
        // �t�B�[���h����o����continue
        if (x+gMoveDirections[i].x < 0 || y+gMoveDirections[i].y < 0 || x+gMoveDirections[i].x > mFieldSizeW-1 || y+gMoveDirections[i].y > mFieldSizeH-1) {
            continue;
        }
        
        x += gMoveDirections[i].x;
        y += gMoveDirections[i].y;
        
        // ���͂Ɏ��`�[���̂��̂ł͂Ȃ��^�C�����Ȃ����continue
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
        
        // �Ⴄ�F�̃}�X�Ȃ�continue
        if (mFieldStatusArray[y][x] != team) {
            continue;
        }
        
        //�@�e�m�[�h�Ɠ����ʒu�Ȃ�continue
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
        
        // ���łɒʂ����}�X�̏ꍇ:
        //  -> �n�_�̃}�X�Ȃ�͂������m����
        //  -> �����łȂ����continue
        if (argFieldMark[y][x]) {
            if (argFieldMark[argStartY][argStartX] && x == argStartX && y == argStartY && node.getTreeHigh() >= 3) {
                // �}�X���͂��ӂ��ǂ̕����ɂ��邩��������
                // ���݂̃m�[�h����e�m�[�h�����ǂ��Ă���
                Node* searchingNode = &node;
                int whiteTiles = 0;
                
                while(1) {
                    if (searchingNode->getParentNode() == nullptr) {
                        break;
                    }
                    
                    searchingNode = searchingNode->getParentNode();
                    // ���̃^�C������e�m�[�h�̃^�C���̊ԂɎ����̃`�[���ł͂Ȃ��^�C�������邩�m�F
                    // ���̃^�C���Ɛe�m�[�h�̃^�C���𒼐��Ō��сA���̊ԂɎ��`�[���̃^�C���ł͂Ȃ��}�X�i=����̃^�C���܂��͔����}�X�j�����邩�m�F
                    // ����Η̈悠��A�Ȃ���Η̈�Ȃ�
                    
                    // �����̌W�����Z�o
                    int slope;
                    if (searchingNode->getPosition().x == x || searchingNode->getPosition().y == y) {
                        slope = 0;      // �X�����Ȃ����0
                    }
                    else {
                        slope = (searchingNode->getPosition().y - y) / (searchingNode->getPosition().x - x);
                    }
                    
                    // �����̌������w��
                    int directionX, directionY;
                    if (searchingNode->getPosition().x > x) {
                        directionX = -1;            // �e�m�[�h�̃^�C��x���W > ���̃^�C����x���W
                    }
                    else if (searchingNode->getPosition().x < x) {
                        directionX = 1;             // �e�m�[�h�̃^�C��x���W < ���̃^�C����x���W
                    }
                    else {
                        directionX = 0;             // �e�m�[�h�̃^�C��x���W == ���̃^�C����x���W
                    }
                    
                    if (searchingNode->getPosition().y > y) {
                        directionY = -1;            // �e�m�[�h�̃^�C��y���W > ���̃^�C����y���W
                    }
                    else if (searchingNode->getPosition().y < y) {
                        directionY = 1;             // �e�m�[�h�̃^�C��y���W < ���̃^�C����y���W
                    }
                    else {
                        directionY = 0;             // �e�m�[�h�̃^�C��y���W == ���̃^�C����y���W
                    }
                    
                    // �����̎n�_ = �e�m�[�h�̃^�C��
                    int lineX = searchingNode->getPosition().x, lineY = searchingNode->getPosition().y;
                    
                    whiteTiles = 0;
                    int lineXBefore, lineYBefore;
                    
                    // ���������ǂ��Ă����A���̊ԂɎ��`�[���̃^�C�����Ȃ��}�X���������邩������
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
                        
                        // �t�B�[���h�̊O�ɏo����break
                        if (lineX < 0 || lineY < 0 || lineX > mFieldSizeW-1 || lineY > mFieldSizeH-1) {
                            break;
                        }
                        
                        // ���`�[���̃^�C�����Ȃ���΃J�E���g
                        // ����: ���`�[���̃^�C�����Ȃ��A���`�[���̗̈�ɂ��܂܂�Ă��Ȃ�
                        if (mFieldStatusArray[lineY][lineX] != team &&
                            ((team == TileStatus::ALLY && !mFieldAllyAreaSquaresArray[lineY][lineX]) ||
                             (team == TileStatus::ENEMY && !mFieldEnemyAreaSquaresArray[lineY][lineX]))) {
                                whiteTiles ++;
                            }
                        
                        // �ړI�̃}�X�ɂ��ǂ蒅�����Ƃ��̏���
                        if (lineX == x && lineY == y) {
                            // ���`�[���ȊO�̃^�C���̃J�E���g��1�ȏ�Ȃ�A�̈��F������
                            if (whiteTiles > 0) {
                                //cout << "�͂������m(" << team << "): ";
                                //cout << searchingNode->getPosition().x << ',' << searchingNode->getPosition().y << '(' << searchingNode << ')' << endl;
                                
                                // �{���Ɉ͂߂Ă��邩�AsearchAreaPointsSquares�֐��Ŋm�F
                                // �m�F�Ɠ����ɗ̈�ւ̎w������ɍs��
                                // �̈悪�m�F�ł�����AareThereAreaPoints��true �ł��Ȃ����false�i������false�ɂȂ邱�Ƃ͂قڂ��肦�Ȃ��j
                                // �̈悪�m�F�ł�����A���̗̈�ւ̎w��𔽉f���A�̈�̕ӂ𐬂��}�X��mFieldAreaSideLinesArray��true�ɂ���
                                vector<vector<bool>> areaSquares;
                                if (team == TileStatus::ALLY) {
                                    areaSquares = mFieldAllyAreaSquaresArray;
                                }
                                else {
                                    areaSquares = mFieldEnemyAreaSquaresArray;
                                }
                                bool areThereAreaPoints = searchAreaPointsSquares(areaSquares, lineXBefore, lineYBefore, team, false);
                                
                                // �̈悪�m�F�ł�����...
                                if (areThereAreaPoints) {
                                    // �̈�|�C���g�ɔ��f
                                    if (team == TileStatus::ALLY) {
                                        mAllyScore.area = 0;
                                        
                                        // ���̗̈�ւ̎w��𔽉f
                                        mFieldAllyAreaSquaresArray = areaSquares;
                                        
                                    }
                                    else {
                                        mEnemyScore.area = 0;
                                        
                                        // ���̗̈�ւ̎w��𔽉f
                                        mFieldEnemyAreaSquaresArray = areaSquares;
                                        
                                    }
                                    
                                    for (int j = 0; j < mFieldSizeH; j++) {
                                        for (int k = 0; k < mFieldSizeW; k++) {
                                            if (team == TileStatus::ALLY && mFieldAllyAreaSquaresArray[j][k]) {
                                                mAllyScore.area += abs(mFieldPointsArray[j][k]);
                                            }
                                            if (team == TileStatus::ENEMY && mFieldEnemyAreaSquaresArray[j][k]) {
                                                mEnemyScore.area += abs(mFieldPointsArray[j][k]);
                                            }
                                        }
                                    }
                                    
                                    // �̈�𐬂��ӂ𔽉f
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
            
            continue;       // ���łɒʂ����}�X�����n�_�ł͂Ȃ��Ƃ�
        }
        
        // �n�_����2�}�X�ȏ㗣��Ă�����A�n�_�ɂ��}�[�N��t����i������Ԃł͂��Ă��Ȃ��j
        if (!argFieldMark[argStartY][argStartX] && (abs(argStartX-x) >= 2 || abs(argStartY-y) >= 2)) {
            argFieldMark[argStartY][argStartX] = true;
        }
        
        // �}�X�Ƀ}�[�N��t����
        argFieldMark[y][x] = true;
        
        searchAreaPointsSide(argFieldMark, argStartX, argStartY, x, y, &node);
        
        nodes ++;
    }
}

/**
 searchAreaPointsSquares:
 *�ċA�֐�
 �̈�Ɖ��肵���}�X�̒T��
 Field::searchAreaPointsSide�ŗ̈悪���݂���Ɖ��肵�����ƂɁA�{���ɗ̈悪���݂��邩�m�F����B
 ���邢�͗̈�𐬂��^�C��������ɏ������ꂽ���ɁA���̌���̈悪�������邩�ǂ����m�F����B
 ��������4�����Ɉړ����Ȃ��璲������B
 �������������`�[���̃^�C���ɂԂ���܂ŒT���𑱂��A
 �����`�[���̃^�C���ɂԂ��邱�ƂȂ��t�B�[���h�̊O�ɑ��������o����A�̈悪�������Ȃ��Ɣ��f����B
 �t�B�[���h�̊O�ɏo�邱�ƂȂ��A���ׂĂ̑������������`�[���̃^�C���ɂԂ�������A�̈悪�������邱�Ƃ�F�߂�B
 �̈悪��������Ȃ�true, �������Ȃ��Ȃ�false��Ԃ��B
 
 @param argAreaSquares  �̈�Ƀ}�X���܂܂�Ă��邩�ǂ����������ꎞ�I�Ȕz��
 @param argStartX       �̈�T���̎n�_�̃}�X��x���W
 @param argStartY       �̈�T���̎n�_�̃}�X��y���W
 @param argBeTileStatus �^�C�����ǂ���̂��`�[���̗̈�ɂ��������iTileStatus::ALLY or TileStatus::ENEMY�j
 @param argDeleteArea   �̈�������������Ƃ���true, �̈��ݒ肵�����Ƃ���false
 @return �̈悪���������true, ���Ȃ��Ȃ�false�i�̈�̉����ɐ��������ꍇ��false�j
 */
bool Field::searchAreaPointsSquares(vector<vector<bool>> &argAreaSquares, const int argStartX, const int argStartY, const int argBeTileStatus, const bool argDeleteArea) {
    int x, y;
    bool possible = true;
    
    // �̈��T������Ώۂ̃`�[��
    // �̈�̉��������ł���΁A��������Ώۂ̃`�[��
    Team::Type team;
    if (argBeTileStatus == TileStatus::ALLY) {
        team = Team::ALLY;
    }
    else {
        team = Team::ENEMY;
    }
    
    // �̈���Ɏw��
    if (!argDeleteArea) {
        if (!argAreaSquares[argStartY][argStartX]) {
            argAreaSquares[argStartY][argStartX] = true;
        }
    }
    // �̈�������iargDeleteArea == true�j����Ƃ�
    else {
        argAreaSquares[argStartY][argStartX] = false;
    }
    
    // ����4�����̃}�X�̊m�F
    for (int i = 0; i < 4; i++) {
        x = argStartX + gSearchTileDirections[i].x;
        y = argStartY + gSearchTileDirections[i].y;
        
        // �t�B�[���h����o����: �̈悪�ł��Ă��Ȃ����߃��[�v����o��false��Ԃ�
        if (x < 0 || y < 0 || x > mFieldSizeW-1 || y > mFieldSizeH-1) {
            possible = false;
            break;
        }
        
        // �����F�̃}�X�ɂ���������: continue
        if (mFieldStatusArray[y][x] == argBeTileStatus) {
            continue;
        }
        
        // �̈�̒T���̂Ƃ� ���łɗ̈���Ɏw�肳��Ă���}�X�Ȃ�: continue
        if (!argDeleteArea && (argAreaSquares[y][x] == true)) {
            continue;
        }
        
        // �̈�̉����̂Ƃ� ���`�[���̗̈�łȂ����
        if (argDeleteArea && !argAreaSquares[y][x]) {
            if (mFieldStatusArray[y][x] == TileStatus::NONE) {
                possible = false;
            }
            continue;
        }
        
        // ���͂ɂ܂��̈�Ƃ��Ďw�肵�Ă��Ȃ��}�X����������: ���̃}�X�Ɉړ�
        // �̈�̉����̏ꍇ�͗̈�Ƃ��Ďw�肳��Ă���}�X����������
        bool nextTile = searchAreaPointsSquares(argAreaSquares, x, y, argBeTileStatus, argDeleteArea);
        
        if (!nextTile) {
            possible = false;   // �ړ���ŗ̈悪�������Ȃ����Ƃ��킩������: possible = false�i�̈�����s�j
        }
    }
    
    // �̈悪�������Ȃ����Ƃ��킩������false��Ԃ��i�̈悪�������Ȃ����Ƃ������j
    if (!possible) {
        return false;
    }
    
    // ���ɖ�肪�Ȃ����true��Ԃ�
    return true;
}


/**
 Field::searchAreaPoints:
 �̈悪���݂��邩�m�F����B
 ���̊֐��́A�ċA�֐��ł���searchAreaPointsSide�̌Ăяo���p�̊֐��ŁA
 ���̊֐����̂͗̈�̒T�����s��Ȃ��B
 
 @param argStartX �̈�T���̎n�_�̃}�X��x���W
 @param argStartY �̈�T���̎n�_�̃}�X��y���W
 */
void Field::searchAreaPoints(const int argStartX, const int argStartY) {
    vector<vector<bool>> fieldMark(mFieldSizeH, vector<bool>(mFieldSizeW, false));
    searchAreaPointsSide(fieldMark, argStartX, argStartY, argStartX, argStartY, nullptr);
}

/**
 Field::fillSquare:
 �}�X��h��Ԃ��i�f�[�^��œh��Ԃ��̂ł͂Ȃ��A�h��Ԃ����}�X��`�悷��j
 
 @param argSquarePosition   �}�X�̕`��ʒu Vec2�^
 @param argX                �}�X��x���W
 @param argY                �}�X��y���W
 @param rectColor           Color�^�i��̂܂܂ł��A�Ȃɂ��w�肵�Ă��Ă������ς�邱�Ƃ͂Ȃ��j
 */
void Field::fillSquare(const Vec2 argSquarePosition, const int argX, const int argY, Color& rectColor) {
    Color agentIDRectColor = Palette::White;
    
    // �����̃^�C���̂Ƃ�
    if (mFieldStatusArray[argY][argX] == 1) {
        if (mAllyTeamColor == TeamColor::BLUE) {
            agentIDRectColor = Color(0, 128, 255); // ��
            rectColor = Color(0, 100, 200);
            
        }
        else if (mAllyTeamColor == TeamColor::RED) {
            agentIDRectColor = Color(255, 50, 0);  // ��
            rectColor = Color(200, 10, 0);
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
        if (mFieldAgentsIDArray[argY][argX] != 0) {
            Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize/3*2, 20).draw(agentIDRectColor);
            mAgentIDTextFont(U"Ally").draw(argSquarePosition.x+2, argSquarePosition.y, Color(Palette::White));
        }
    }
    // ����̃^�C���̂Ƃ�
    if (mFieldStatusArray[argY][argX] == -1) {
        if (!mAllyTeamColor == TeamColor::BLUE) {
            agentIDRectColor = Color(0, 128, 255); // ��
            rectColor = Color(0, 100, 200);
        }
        else if (!mAllyTeamColor == TeamColor::RED) {
            agentIDRectColor = Color(255, 50, 0);  // ��
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
 �̈�}�X��h��Ԃ��i�f�[�^��œh��Ԃ��̂ł͂Ȃ��A�h��Ԃ����}�X��`�悷��j
 
 @param argSquarePosition   �}�X�̕`��ʒu Vec2�^
 @param argX                �}�X��x���W
 @param argY                �}�X��y���W
 */
void Field::fillAreaSquare(const Vec2 argSquarePosition, const int argX, const int argY) {
    Color rectColor;
    
    // �����̗̈�̂Ƃ�
    if (mFieldAllyAreaSquaresArray[argY][argX]) {
        if (mAllyTeamColor == TeamColor::BLUE) {
            rectColor = Color(150, 150, 255);   // ��
            
        }
        else if (mAllyTeamColor == TeamColor::RED) {
            rectColor = Color(255, 150, 150);   // ��
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
    }
    // ����̗̈�̂Ƃ�
    if (mFieldEnemyAreaSquaresArray[argY][argX]) {
        if (!mAllyTeamColor == TeamColor::BLUE) {
            rectColor = Color(150, 150, 255);   // ��
        }
        else if (!mAllyTeamColor == TeamColor::RED) {
            rectColor = Color(255, 150, 150);   // ��
        }
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
    }
    // ���`�[���̗̈�̂Ƃ�
    if (mFieldAllyAreaSquaresArray[argY][argX] && mFieldEnemyAreaSquaresArray[argY][argX]) {
        rectColor = Color(200, 100, 200);       // ��
        
        Rect(argSquarePosition.x, argSquarePosition.y, mFieldSquareSize, mFieldSquareSize).draw(rectColor);
    }
}

/**
 Field::printSquarePoint:
 �^�C���_���̕\��
 
 @param argSquarePosition   �\������}�X�̕`��ʒu Vec2�^
 @param argX                �\������}�X��x���W
 @param argY                �\������}�X��y���W
 @param rectColor           Color�^�i��̂܂܂ł��A�Ȃɂ��w�肵�Ă��Ă������ς�邱�Ƃ͂Ȃ��j
 */
void Field::printSquarePoint(const Vec2 argSquarePosition, const int argX, const int argY, Color& rectColor) {
    unsigned long int textLength = to_string(mFieldPointsArray[argY][argX]).length();
    
    Color textColor = Palette::Black;
    if (rectColor != Palette::White) {
        textColor = Palette::White;
    }
    
    if (mFieldPointsArray[argY][argX] >= mBigPointsBorder) {    // ���_�����20��(=mBigPointsBorder)�ȏ�Ȃ瑾��
        mPointTextFontBold(mFieldPointsArray[argY][argX])
        .draw(argSquarePosition.x+mFieldSquareSize/2-textLength*(mFieldSquareSize-POINT_TEXT_BIG_FONT_SIZE)/3, argSquarePosition.y+mFieldSquareSize/2-(mFieldSquareSize-POINT_TEXT_BIG_FONT_SIZE)/2-5, Color(textColor));
    }
    else {                                                      // ���_�����20��(=mBigPointsBorder)�����Ȃ畁�ʂ̎�
        mPointTextFont(mFieldPointsArray[argY][argX])
        .draw(argSquarePosition.x+mFieldSquareSize/2-textLength*(mFieldSquareSize-POINT_TEXT_FONT_SIZE)/3, argSquarePosition.y+mFieldSquareSize/2-(mFieldSquareSize-POINT_TEXT_FONT_SIZE)/2-5, Color(textColor));
    }
}

/**
 Field::removeTile:
 �^�C������������B
 �܂��A�̈�𐬂��^�C������������ꍇ�́A
 ������ɗ̈悪�������邩�m�F���邽�߁AsearchAreaPointsSquare�֐����Ăяo���B
 
 @param argX            �^�C������������}�X��x���W
 @param argY            �^�C������������}�X��y���W
 @param argTileStatus   �^�C������������`�[����TileStatus�i���������`�[���ł͂Ȃ��j
 */
void Field::removeTile(const int argX, const int argY, const int argTileStatus) {
    mFieldStatusArray[argY][argX] = 0;
    
    // ���肩��_��������
    if (mCurrentAgentID > 0) {
        mEnemyScore.tile -= mFieldPointsArray[argY][argX];
    }
    else {
        mAllyScore.tile -= mFieldPointsArray[argY][argX];
    }
    
    // ����`�[���̗̈�ɗאڂ��Ă�����A����`�[���̗̈���Čv�Z
    for (int i = 0; i < 4; i++) {
        if (argX+gSearchTileDirections[i].x < 0 || argY+gSearchTileDirections[i].y < 0 ||
            argX+gSearchTileDirections[i].x > mFieldSizeW-1 || argY+gSearchTileDirections[i].y > mFieldSizeH-1) {
            continue;
        }
        
        // �̈�̕ӂ𐬂��^�C���Ȃ�A��������̈悪�������邩�m�F
        // �������Ȃ���΁A�̈����������
        if ((argTileStatus == TileStatus::ENEMY && mFieldAllyAreaSquaresArray[argY+gSearchTileDirections[i].y][argX+gSearchTileDirections[i].x]) ||
            (argTileStatus == TileStatus::ALLY && mFieldEnemyAreaSquaresArray[argY+gSearchTileDirections[i].y][argX+gSearchTileDirections[i].x])) {
            if (mFieldAreaSideLinesArray[argY][argX]) {
                
                // �����`�[���̃G�[�W�F���g������`�[���̃^�C������������Ƃ�
                if (mCurrentAgentID > 0) {
                    vector<vector<bool>> areaSquares = mFieldEnemyAreaSquaresArray;
                    bool areThereArea = searchAreaPointsSquares(areaSquares, argX+gSearchTileDirections[i].x, argY+gSearchTileDirections[i].y, TileStatus::ENEMY, true);
                    
                    if (!areThereArea) {
                        // �̈�|�C���g�ɔ��f
                        for (int j = 0; j < mFieldSizeH; j++) {
                            for (int k = 0; k < mFieldSizeW; k++) {
                                if (mFieldEnemyAreaSquaresArray[j][k] && !areaSquares[j][k]) {
                                    mEnemyScore.area -= abs(mFieldPointsArray[j][k]);
                                }
                            }
                        }
                        
                        mFieldEnemyAreaSquaresArray = areaSquares;
                        
                        // ������x�m�F����i���̃^�C�����������Ă��̈悪��������ꍇ�����邽�߁j
                        // ��������Ȃ�̈���������Ȃ�
                        bool areaRecheck = searchAreaPointsSquares(areaSquares, argX, argY, TileStatus::ENEMY, false);
                        
                        if (areaRecheck) {
                            // �̈�|�C���g�ɔ��f
                            for (int j = 0; j < mFieldSizeH; j++) {
                                for (int k = 0; k < mFieldSizeW; k++) {
                                    if (!mFieldEnemyAreaSquaresArray[j][k] && areaSquares[j][k]) {
                                        mEnemyScore.area += abs(mFieldPointsArray[j][k]);
                                    }
                                }
                            }
                            
                            mFieldEnemyAreaSquaresArray = areaSquares;
                        }
                    }
                }
                // ����`�[���̃G�[�W�F���g�������`�[���̃^�C������������Ƃ�
                else if (mCurrentAgentID < 0) {
                    vector<vector<bool>> areaSquares = mFieldAllyAreaSquaresArray;
                    bool areThereArea = searchAreaPointsSquares(areaSquares, argX+gSearchTileDirections[i].x, argY+gSearchTileDirections[i].y, TileStatus::ALLY, true);
                    
                    if (!areThereArea) {
                        // �̈�|�C���g�ɔ��f
                        for (int j = 0; j < mFieldSizeH; j++) {
                            for (int k = 0; k < mFieldSizeW; k++) {
                                if (mFieldAllyAreaSquaresArray[j][k] && !areaSquares[j][k]) {
                                    mAllyScore.area -= abs(mFieldPointsArray[j][k]);
                                }
                            }
                        }
                        
                        mFieldAllyAreaSquaresArray = areaSquares;
                        
                        // ������x�m�F����i���̃^�C�����������Ă��̈悪��������ꍇ�����邽�߁j
                        // ��������Ȃ�̈���������Ȃ�
                        bool areaRecheck = searchAreaPointsSquares(areaSquares, argX, argY, TileStatus::ALLY, false);
                        
                        if (areaRecheck) {
                            // �̈�|�C���g�ɔ��f
                            for (int j = 0; j < mFieldSizeH; j++) {
                                for (int k = 0; k < mFieldSizeW; k++) {
                                    if (!mFieldAllyAreaSquaresArray[j][k] && areaSquares[j][k]) {
                                        mAllyScore.area += abs(mFieldPointsArray[j][k]);
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
}

/**
 Field::putTile:
 �^�C����u���B
 �܂��A�̈�|�C���g�̑��݂��m�F���邽�߁AsearchAreaPoints�֐����Ăяo���B

 @param argX            �^�C����u���}�X��x���W
 @param argY            �^�C����u���}�X��y���W
 @param argTileStatus   �^�C����u���`�[����TileStatus
 */
void Field::putTile(const int argX, const int argY, const int argTileStatus) {
    mFieldStatusArray[argY][argX] = argTileStatus;
        
    if (mCurrentAgentID > 0) {
        if (mFieldAllyAreaSquaresArray[argY][argX]) {
            mFieldAllyAreaSquaresArray[argY][argX] = false;
            mAllyScore.area -= abs(mFieldPointsArray[argY][argX]);
        }
            
        mAllyScore.tile += mFieldPointsArray[argY][argX];       // �^�C���|�C���g�Ƀ}�X�̓��_�����Z
    }
    else {
        if (mFieldEnemyAreaSquaresArray[argY][argX]) {
            mFieldEnemyAreaSquaresArray[argY][argX] = false;
            mEnemyScore.area -= abs(mFieldPointsArray[argY][argX]);
        }
            
        mEnemyScore.tile += mFieldPointsArray[argY][argX];      // �^�C���|�C���g�Ƀ}�X�̓��_�����Z
    }
    
    // �̈�|�C���g���ł��Ă��邩�m�F
    searchAreaPoints(argX, argY);
}

/**
 Field::agenrMovement:
 �G�[�W�F���g�̍s��
 �G�[�W�F���g�̈ړ��A�^�C���ݒu(Field::putTile�֐���)�A�^�C���̏���(Field::removeTile�֐���)�Ȃǂ��s���B
 �^�C���ݒu���ɂ́A�̈悪�������邩���m�F����(Field::searchAreaPoints�֐���)�B
 ����`�[���̗̈�̕ӂ𐬂��^�C������������Ƃ��́A����`�[���̗̈�̉������ł��邩�ǂ��������ׂ�B
 �Ō�ɁA�G�[�W�F���g�̍s���̔ԍ���Ԃ��B
 
 @param argX        �G�[�W�F���g�̈ړ���̃}�X��x���W
 @param argY        �G�[�W�F���g�̈ړ���̃}�X��y���W
 @param argBeforeX  �G�[�W�F���g�̈ړ��O�̃}�X��x���W
 @param argBeforeY  �G�[�W�F���g�̈ړ��O�̃}�X��y���W
 @return �G�[�W�F���g�̍s���̔ԍ��i0~16�j�@field.hpp��AgentActNumbers�ɒ�`�ς�
 */
int Field::agentMovement(int argX, int argY, int argBeforeX, int argBeforeY) {
    int beTileStatus = mFieldStatusArray[argBeforeY][argBeforeX];   // �{���Ώۂ̃G�[�W�F���g�̃`�[��
    int actNumber = 0;      // �s���̔ԍ�
    
    if (Vec2{argY, argX} != Vec2{-1, -1}) {
        // �i�������G�[�W�F���g���猩�āj����̃^�C���Ȃ� �^�C��������
        // ����̗̈�𐬂��^�C���Ȃ�A������ɂ��̗̈悪�������邩�m�F
        if (mFieldStatusArray[argY][argX] == (-1)*mFieldStatusArray[argBeforeY][argBeforeX] && mFieldAgentsIDArray[argY][argX] == 0) {
            // �^�C���̏����i����̗̈�𐬂��^�C���Ȃ�A������ɂ��̗̈悪�������邩�m�F�j
            removeTile(argX, argY, beTileStatus);
            
            // �Ԃ��s���ԍ��̎擾
            if (argY-argBeforeY == -1) {
                if (argX-argBeforeX == -1) {
                    actNumber = AgentActNumbers::REMOVE_UP_LEFT;      // ���������
                }
                else if (argX-argBeforeX == 0) {
                    actNumber = AgentActNumbers::REMOVE_UP;           // �������
                }
                else if (argX-argBeforeX == 1) {
                    actNumber = AgentActNumbers::REMOVE_UP_RIGHT;     // �E�������
                }
            }
            else if (argY-argBeforeY == 0) {
                if (argX-argBeforeX == -1) {
                    actNumber = AgentActNumbers::REMOVE_LEFT;         // ��������
                }
                else if (argX-argBeforeX == 1) {
                    actNumber = AgentActNumbers::REMOVE_RIGHT;        // �E������
                }
            }
            else if (argY-argBeforeY == 1) {
                if (argX-argBeforeX == -1) {
                    actNumber = AgentActNumbers::REMOVE_DOWN_LEFT;    // ����������
                }
                else if (argX-argBeforeX == 0) {
                    actNumber = AgentActNumbers::REMOVE_DOWN;         // ��������
                }
                else if (argX-argBeforeX == 1) {
                    actNumber = AgentActNumbers::REMOVE_DOWN_RIGHT;   // �E��������
                }
            }
        }
        // ����ȊO�Ȃ�}�X�ɃG�[�W�F���g���ړ������^�C����u��
        // �̈悪��������Ȃ�̈�̐ݒ���s��
        else if (mFieldAgentsIDArray[argY][argX] == 0) {
            mCurrentSquarePosition = {argX, argY};
            
            // �^�C����u�������i�܂��^�C�����u����Ă��Ȃ��ꍇ�̂݁j
            // �̈悪��������Ȃ�̈�̐ݒ���s��
            if (mFieldStatusArray[argY][argX] != beTileStatus) {
                putTile(argX, argY, beTileStatus);
            }
            
            // �G�[�W�F���g�̈ʒu���ړ�������
            mFieldAgentsIDArray[argY][argX] = mCurrentAgentID;
            mFieldAgentsIDArray[argBeforeY][argBeforeX] = 0;
            
            // �Ԃ��s���ԍ��̎擾
            if (argY-argBeforeY == -1) {
                if (argX-argBeforeX == -1) {
                    actNumber = AgentActNumbers::GO_UP_LEFT;        // ����ֈړ�
                }
                else if (argX-argBeforeX == 0) {
                    actNumber = AgentActNumbers::GO_UP;             // ��ֈړ�
                }
                else if (argX-argBeforeX == 1) {
                    actNumber = AgentActNumbers::GO_UP_RIGHT;       // �E��ֈړ�
                }
            }
            else if (argY-argBeforeY == 0) {
                if (argX-argBeforeX == -1) {
                    actNumber = AgentActNumbers::GO_LEFT;           // ���ֈړ�
                }
                else if (argX-argBeforeX == 1) {
                    actNumber = AgentActNumbers::GO_RIGHT;          // �E�ֈړ�
                }
            }
            else if (argY-argBeforeY == 1) {
                if (argX-argBeforeX == -1) {
                    actNumber = AgentActNumbers::GO_DOWN_LEFT;      // �����ֈړ�
                }
                else if (argX-argBeforeX == 0) {
                    actNumber = AgentActNumbers::GO_DOWN;           // ���ֈړ�
                }
                else if (argX-argBeforeX == 1) {
                    actNumber = AgentActNumbers::GO_DOWN_RIGHT;     // �E���ֈړ�
                }
            }
        }
    }
    
    // �s���̔ԍ��������o�z��Ɋi�[
    // �����G�[�W�F���g�̏ꍇ
    if (mCurrentAgentID > 0) {
        mAllyAgentsActNumbers[mCurrentAgentID-1] = actNumber;
    }
    // ����G�[�W�F���g�̏ꍇ
    else {
        mEnemyAgentsActNumbers[(-1)*mCurrentAgentID-1] = actNumber;
    }
    
    return actNumber;
}

/**
 Field::setCurrentAgent:
 �I������G�[�W�F���g�̕ύX����
 
 @param argX �J�[�\���őI�����ꂽx���W
 @param argY �J�[�\���őI�����ꂽy���W
 */
void Field::setCurrentAgent(int argX, int argY) {
    // �V�����G�[�W�F���g��I������Ƃ�
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
 �t�B�[���h�̏c���E�����̕`��
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
 �t�B�[���h�̕`��
 */
void Field::draw() {
    Color rectColor;
    
    // �t�B�[���h�̏c���E�����̕`��
    drawSquares();
    
    // �_���̕\��
    // �`�[���F�ɂ��h��Ԃ�
    // �G�[�W�F���gID�̕\��
    // �}�X�̏�ɃJ�[�\��������� �}�X�ɘg��\��
    Vec2 currentSquarePositionBefore = mCurrentSquarePosition;
    
    for (int y = 0; y < mFieldSizeH; y++) {
        for (int x = 0; x < mFieldSizeW; x++) {
            // �}�X�̕`��ʒu
            int squarePositionX = mFieldLeftmostPoint+x*mFieldSquareSize;
            int squarePositionY = mFieldTopmostPoint+y*mFieldSquareSize;
            
            // �}�X�̓h��Ԃ��i�ǂ��炩�̃`�[���̃^�C���̏ꍇ�j
            rectColor = Palette::White;
            fillSquare(Vec2{squarePositionX, squarePositionY}, x, y, rectColor);
            
            // �̈�̓h��Ԃ��i�}�X���͂܂�Ă���ꍇ�j
            if (mFieldStatusArray[y][x] == TileStatus::NONE) {
                fillAreaSquare(Vec2{squarePositionX, squarePositionY}, x, y);
            }
            
            // �_���̕\��
            printSquarePoint(Vec2{squarePositionX, squarePositionY}, x, y, rectColor);
            
            // �g�̕\��
            if (isCursorOnTheSquare(Vec2(mFieldLeftmostPoint+x*mFieldSquareSize, mFieldTopmostPoint+y*mFieldSquareSize))) {
                // �}�E�X�����N���b�N�̏�Ԃ��� �I������Ă����}�X����ړ��ł���}�X�Ȃ�΁A�I������Ă���}�X�ɐݒ�
                // �G�[�W�F���g��I������Ă���}�X�Ɉړ�
                if (MouseL.pressed() && !mMousePressing) {
                    mMousePressing = true;      // ���N���b�N�������ꂽ�u�Ԃ���
                    
                    // ����8�����̃^�C���̒���
                    for (int i=0; i<DIRECTIONS; i++) {
                        // �G�[�W�F���g�̍s���i�ړ��E�����j�𑀍삷��Ƃ�
                        if (currentSquarePositionBefore + gMoveDirections[i] == Vec2{x, y}) {
                            if (currentSquarePositionBefore != Vec2{-1, -1}) {
                                // �G�[�W�F���g�̍s���ɔ��f
                                // �ړ�, �폜
                                cout << "�s���ԍ�: " << agentMovement(x, y, currentSquarePositionBefore.x, currentSquarePositionBefore.y) << endl;
                            }
                            
                            break;
                        }
                    }
                    
                    // �V�����G�[�W�F���g��I������Ƃ��̏���
                    setCurrentAgent(x, y);
                }
                else {
                    // �J�[�\������ɂ���΃}�X�ɉ��F���g��\��
                    Rect(squarePositionX, squarePositionY, mFieldSquareSize, mFieldSquareSize).drawFrame(5, 0, Color(Palette::Yellow));
                }
            }
            
            // �}�X���I������Ă����ԂȂ�� ��F�̘g��\��
            if (Vec2{x, y} == mCurrentSquarePosition) {
                Rect(squarePositionX, squarePositionY, mFieldSquareSize, mFieldSquareSize).drawFrame(5, 0, Color(Palette::Orange));
            }
        }
        
        if (!MouseL.pressed()) {
            mMousePressing = false;
        }
    }
    
    // �t�B�[���h�̘g�̕`��
    Rect(mFieldLeftmostPoint, mFieldTopmostPoint, mFieldLineLengthW, mFieldLineLengthH).drawFrame(0, 4, Color(Palette::Black));
    
    return;
}


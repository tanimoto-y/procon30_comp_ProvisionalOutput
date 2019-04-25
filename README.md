# procon30_ProvisionalController

## 概要
競技部門用の入出力部です。採用されるかもしれないしされないかもしれません。

## 動作確認環境
- macOS 10.14 mojave
  - Xcode 10.2.1

## 使用環境
- C++
- OpenSiv3D v0.3.1

## 実行方法 
OpenSiv3Dのファイルはサイズが大きいため、このリポジトリにはXcodeのプロジェクトファイルのみ置いてあります。  
このままではビルドできません。Xcodeでビルドして動かすには、OpenSiv3Dの導入が必要です。  
（ただし、OpenSiv3Dを導入しなくても`procon30_comp_ProvisionalOutput/projects/ProvisionalOutput/bin/`にあるempty.appから実行することはできます）  

### OpenSiv3Dの導入方法
OpenSiv3Dの導入は簡単で、ファイルをコピーするだけです。  

#### macOSの場合
1. 以下のサイトから、OpenSiv3D プロジェクトテンプレート（zip形式）をダウンロードします。  
https://scrapbox.io/Siv3D/OpenSiv3D_%E3%82%92%E3%81%AF%E3%81%98%E3%82%81%E3%82%8B%E6%BA%96%E5%82%99_(macOS)
2. ダウンロードが完了したら、zipファイルを展開し、libフォルダとincludeフォルダをローカルリポジトリのフォルダ`procon30_comp_ProvisionalOutput`にコピーします。（examplesフォルダは不要）

#### Windowsの場合
（準備中）

### ビルド方法
Macの場合はXcodeで使用してください（CLionだと導入が難しい）。  
Windowsの場合はVisual Studio 2019を使用してください。

#### Xcodeでビルドする場合
1. `procon30_comp_ProvisionalOutput/projects/ProvisionalOutput/ProvisionalOutput.xcodeproj`をXcodeで開く。
2. 画面左上の再生ボタンのようなボタンを押す。

#### Visual Studioでビルドする場合
（準備中）

## 各変数・関数の説明
ここでは、よく使う変数や関数の説明を書きます。  
詳しいことはソースコードにドキュメントコメントで書いてあります。

ドキュメントコメントの見方(Xcode)
1. ソースコードを開きます。
2. 詳細を見たい関数の呼び出し部分を、optionキーまたはaltキーを押しながらクリックします。
3. ドキュメントコメントが表示されます。

### Main.cppの関数

#### centerText
指定した座標の中央に文字を表示します。文字列用と数値用があります。
  
- const String argStr       : 表示する文字列（文字列用）  
- const int argInt          : 表示する数値（数値用）  
- const int argLeftTopX     : 左上のx座標  
- const int argLeftTopY     : 左上のy座標  
- const int argRightBottomX : 右下のx座標  
- const int argRightBottomX : 右下のy座標  
- const Font& argFont       : 表示に使うフォントのポインタ
- const Color argColor      : 文字の色

#### printTotalPoints
点数を表示します。  
  
- const Team::Type argTeam            : どちらのチームを表示するか（Team::ALLY or Team::ENEMY）  
- const bool argTeamColor             : チームの色（TeamColor::RED or TeamColor::BLUE）  
- Field& argFieldData                 : フィールドクラスのポインタ  
- const Font& argFont16               : サイズ16のフォントのポインタ  
- const Font& argFont16Bold           : サイズ16の太字フォントのポインタ
- const Font& argFont28               : サイズ28のフォントのポインタ
- const Font& argTotalPointsTextFont  : サイズ28の太字フォントのポインタ

#### Main
Main関数です。ここからそれぞれの関数を呼び出します。

### Fieldクラス
#### プライベート変数・配列
- vector<vector<<int>>> mFieldPointsArray;              : フィールドの点数（setFieldで代入）  
- vector<vector<<int>>> mFieldStatusArray;              : フィールドの状況（setFieldで代入）  
- vector<vector<<int>>> mFieldAgentsIDArray;            : フィールド上のエージェントの場所とエージェントID（setFieldで設定）  
- vector<vector<<bool>>> mFieldAllyAreaSquaresArray;    : 味方の領域(領域内のマスはtrue, 外のマスはfalse)  
- vector<vector<<bool>>> mFieldEnemyAreaSquaresArray;   : 相手の領域(領域内のマスはtrue, 外のマスはfalse)  
- vector<vector<<bool>>> mFieldAreaSideLinesArray;      : 領域を成す辺（辺を成すマスはtrue, それ以外はfalse）  
  
- int mAllyPoints                                 : 味方チームの合計得点（タイルポイント＋領域ポイント）  
- int mEnemyPoints                                : 相手チームの合計得点（タイルポイント＋領域ポイント）  
- int mAllyTilePoints                             : 味方チームのタイルポイント  
- int mEnemyTilePoints                            : 相手チームのタイルポイント  
- int mAllyAreaPoints                             : 相手チームの領域ポイント
- int mEnemyAreaPoints                            : 相手チームの領域ポイント
  
- int mFieldSizeW, mFieldSizeH                    : フィールドの大きさ（wは横、hは縦）  
- int mFieldLineLengthW, mFieldLineLengthH        : フィールドの枠の長さ  
- int mFieldSquareSize                            : フィールドの1マスあたりの大きさ（正方形）  
  
- int mTotalTeamAgents                            : 各チームのエージェントの総数  
  
- int mFieldLeftmostPoint                         : フィールドの描画範囲の左端の座標  
- int mFieldRightmostPoint                        : フィールドの描画範囲の右端の座標  
- int mFieldTopmostPoint                          : フィールドの描画範囲の上端の座標  
- int mFieldBottommostPoint                       : フィールドの描画範囲の下端の座標  
  
- bool mAllyTeamColor                             : 味方チームの色（敵チームの色は!allyTeamColorで取得できる）  
  
- Vec2 mCurrentSquarePosition                     : 選択されているマスの座標（選択されていなければ(-1, -1)）  
- int mCurrentAgentID                             : 選択されているマスにいるエージェントのID（選択されていなければ 0）  
  
- bool mMousePressing                             : マウスが押されている間はtrue  
  
- Font mPointTextFont                             : 点数表示用のフォント  
- Font mPointTextFontBold                         : 点数表示用の太字フォント  
- Font mAgentIDTextFont                           : エージェントID表示用のフォント  
  
- int mBigPointsBorder                            : 大きい点数の基準（この値を超えると太字で点数を表示する）
    
#### Field::Field
Fieldクラスのコンストラクタです。フィールドの初期化を行います。
  
- const vector<vector<<int>>> &argFieldPointsArray    : フィールドの得点の配列  
- const vector<vector<<int>>> &argFieldStatusArray    : フィールドの状態を示す配列  
- const Vec2 position                               : フィールドの大きさを示す配列（例：Vec2{10, 10}）  
- const bool argAllyTeamColor                       : 味方チームの色（TeamColor::RED or TeamColor::BLUE）
    
#### void Field::setField
フィールドの得点、状況をプライベート配列にコピーし、エージェントにIDを振り分けます。  
IDはフィールドの左上から順に振り分けられ、味方のエージェントのIDは1から上がっていき、相手のエージェントのIDは-1から下がっていきます。  
この関数は、基本的にFieldコンストラクタから呼び出されます。  
例）エージェントが各チーム3人のとき、味方のエージェントIDは1〜3、相手のエージェントIDは-1〜-3  
  
- const vector<vector<<int>>> &argFieldPointsArray    : フィールドの得点の配列  
- const vector<vector<<int>>> &argFieldStatusArray    : フィールドの状態を示す配列
    
#### void Field::searchAreaPointsSide
再帰関数です。  
フィールド上に領域が存在するかどうかを判断し、存在したら領域ポイントを設定します。  
引数で指定した座標のタイルから同じ色のタイルをたどっていき、同じタイルを二度通ることなく最初のタイルまで戻ってこられたら、フィールド上に領域が存在すると仮定します。  
その後、領域と仮定したタイルの間に、相手チームあるいは白いタイルが存在するか調査し、存在したら領域として認定します。  
  
- vector<vector<bool>> argFieldMark     : フィールドの領域に認定したかどうか（一時的な配列）  
- const int argStartX                   : 領域探索の始点のマスのx座標  
- const int argStartY                   : 領域探索の始点のマスのy座標  
- const int argX                        : 現在探索中のマスのx座標  
- const int argY                        : 現在探索中のマスのy座標  
- Node* argParentNode                   : 親ノードのポインタ  

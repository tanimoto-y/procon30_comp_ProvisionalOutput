# procon30_ProvisionalController

## 概要
競技部門用の入出力部です。採用されるかもしれないしされないかもしれません。

## 動作確認環境
- macOS 10.14 mojave (64bit)
  - Xcode 10.2.1
- Windows 10 (64bit)
  - Visual Studio 2019

## 使用環境
- C++
- OpenSiv3D v0.3.1

## 実行方法 
OpenSiv3Dのファイルはサイズが大きいため、このリポジトリにはXcodeおよびVisual Studioのプロジェクトファイルのみ置いてあります。  
このままではビルドできません。ビルドして動かすには、OpenSiv3Dの導入が必要です。  
（ただし、macOSではOpenSiv3Dを導入しなくても`procon30_comp_ProvisionalOutput/projects/ProvisionalOutput/bin/`にあるempty.appから実行することはできます）  

### OpenSiv3Dの導入方法
OpenSiv3Dの導入は簡単で、macOSの場合はファイルをコピーするだけです。    
Windowsの場合はOpenSiv3D SDKのインストールが必要ですが、特に面倒な設定はありません。

#### macOSの場合
1. Xcodeをインストールします。
1. 以下のサイトから、OpenSiv3D プロジェクトテンプレート（zip形式）をダウンロードします。  
https://scrapbox.io/Siv3D/OpenSiv3D_%E3%82%92%E3%81%AF%E3%81%98%E3%82%81%E3%82%8B%E6%BA%96%E5%82%99_(macOS)
2. ダウンロードが完了したら、zipファイルを展開し、libフォルダとincludeフォルダをローカルリポジトリのフォルダ`procon30_comp_ProvisionalOutput`にコピーします。（examplesフォルダは不要）

#### Windowsの場合
参考 : https://scrapbox.io/Siv3D/OpenSiv3D_%E3%82%92%E3%81%AF%E3%81%98%E3%82%81%E3%82%8B%E6%BA%96%E5%82%99_(Windows)
1. Visual Studio 2019 Communityのインストーラをダウンロードします。
https://visualstudio.microsoft.com/ja/downloads/
2. インストーラを起動して、「C++ によるデスクトップ開発」を選択し、インストールします。
3. インストールが完了したら、OpenSiv3D SDKをダウンロードし、インストールします。
  https://siv3d.jp/downloads/Siv3D/OpenSiv3D(0.3.2)Installer.exe

### ビルド方法
Macの場合はXcodeで使用してください（CLionだと導入が難しい）。  
Windowsの場合はVisual Studio 2019を使用してください。  
OpenSiv3DはXcodeまたはVisual Studio 2019で使うことが前提なので、恐らく他のIDEでは完全には動かないと思います。  

#### Xcodeでビルドする場合
1. `procon30_comp_ProvisionalOutput/projects/ProvisionalOutput/ProvisionalOutput.xcodeproj`をXcodeで開く。
2. 画面左上の▶を押す。

#### Visual Studioでビルドする場合
1. `procon30_comp_ProvisionalOutput/for_VisualStudio/procon30_comp_ProvisionalOutput.sln`をVisual Studioで開く。
2. CtrlとF5を同時に押すか、または画面上部の「▶ローカル Windowsデバッガー」を押す。

## 使い方
### エージェントの選択
起動すると、フィールドがで表示されます。  
フィールドは現在のところ変更不可ですが、近い内に変更できるようにします。  
エージェントがいるマスには、マスの上側に、味方エージェントなら「Ally」相手エージェントなら「Enemy」と表示され、その後ろにエージェントの番号が続きます。  
エージェントの番号は自動で割り振られ、Fieldかクラス内の配列では、この順番通りにエージェントの情報が格納されます（要素番号は エージェント番号-1 で、0から始まる）。  
エージェントがいるマスの上で左クリックすると、エージェントが選択されます。  
エージェントが選択されると、マスの枠がオレンジ色になります。  
### エージェントの移動＆タイルの設置
エージェントのを選択したあと、エージェントの周囲8方向のマスを選択すると、エージェントが移動します。  　
このとき、相手チームのタイルがあるマスやエージェントがいるマスには移動できません。  
移動すると、自動的にタイルが設置されます。
### タイルの除去
エージェントの周囲8方向のマスにのうち、相手チームのタイルがあるマスを選択すると、相手チームのタイルを除去します。    　

また、味方チームのタイルを除去したい場合は、味方チームのタイルがあるマスを右クリックします。
### 領域ポイントについて
領域が成立すると、領域ポイントが得点に加算されます。  
領域内のマスは薄い色で塗られます。
### 得点の見方
左右端にある数字が得点です。右側の「Ally」が味方チーム、左側の「Enemy」が相手チームです。  
大きく表示されている数値が総得点で、その下の数値がタイルポイントと領域ポイントです。  
「タイル」「領域」の隣の●の色は、相手チームに比べて点数が高いか、低いかによって変わります。  
グレー：同点  
緑色：勝ち  
橙色 : 負け  
### ターン切り替え
ターンを切り替えるには、左下にある「確定」ボタンを押します。  
現在のターン数は、画面左上に表示されます。
### 探索の開始
探索を行う際は、右下にある「探索」ボタンを押します。  
探索は1ターンに付き1回まで行えます。  
また、「確定」ボタンの下にある「確定と同時に探索」に✓を入れると、確定ボタンが押された時に探索も行います。

## 定数・構造体・名前空間の説明
定数と構造体はheader.hppで定義しています。  

### 定数: ウィンドウの大きさ関連
#### WINDOW_SIZE_W
ウィンドウの横幅です。
#### WINDOW_SIZE_H
ウィンドウの縦幅です。

### 定数: フィールドの大きさ・幅関連
#### FIELD_LINE_MAX_LENGTH
フィールドの枠の最大長です。  
フィールドはこの長さに合わせて描画します。
#### FIELD_SIZE_MAX
フィールドの一列の最大のマスの数です。

### 定数: フォントサイズ関連
#### POINT_TEXT_FONT_SIZE
点数表示用のフォントの、フィールドの1マスあたりの大きさから引く数値です。
#### POINT_TEXT_BIG_FONT_SIZE
点数表示用の大きいフォントの、フィールドの1マスあたりの大きさから引く数値です。
#### AGENT_ID_TEXT_FONT_SIZE
エージェントID表示用のフォントの、フィールドの1マスあたりの大きさから引く数値です。

### 定数: 行動関連
#### DIRECTIONS
エージェントが移動できる方向の総数です。「とどまる」も含みます。

### グローバル定数: 周囲のマスの座標関連
#### const Vec2 gMoveDirections[9]
エージェントが移動できる座標を格納しています。
#### const Vec2 gSearchTileDirections[4]
周囲4方向の座標を格納しています。

### 構造体: 得点
#### struct Score
得点を保持するための構造体です。  
- int tile : タイルポイント
- int area: 領域ポイント
- int total(): 合計得点（tile + area）を返す関数

### 名前空間: チーム関連
チームや色、タイルの色の状態を示す番号を格納しています。
使用例）TeamColorのBLUEの値を取得したい場合 : TeamColor::BLUE
#### TeamColor
チームの色を示す番号を格納しています。  
- BLUE : 青チーム  
- RED : 赤チーム
#### Team
チーム自体を示す番号を格納しています。  
ここで言うチームとは、味方か相手のどちらかということを指します。  
- ALLY : 味方チーム  
- ENEMY : 相手チーム  
#### TileStatus
タイルの状態を示す番号を格納しています。  
- ALLY : 味方チームのタイル  
- NONE : どちらのチームのタイルでもない（白いタイル）  
- ENEMY : 相手チームのタイル
#### AreaStatus
特定のマスがどのチームの領域であるかを示す番号を格納しています。  
- NONE : どちらのチームの領域でもない

### 名前空間: エージェントの行動の番号
#### AgentActNumbers
エージェントの行動の番号を格納しています。  
使用例）右に移動 : AgentActNumbers::GOTO_RIGHT  
番号は以下の規定に従います:  
- とどまる : STAY = 0  
  
- 移動 左上 : GO\_UP\_LEFT = 1  
- 移動 上 : GO_UP = 2  
- 移動 右上 : GO\_UP\_RIGHT = 3  
- 移動 右 : GO_RIGHT = 4  
- 移動 右下 : GO\_DOWN\_RIGHT = 5  
- 移動 下 : GO_DOWN = 6  
- 移動 左下 : GO\_DOWN\_LEFT = 7  
- 移動 左 : GO_LEFT = 8  
  
- 除去 左上 : REMOVE\_UP\_LEFT = 9  
- 除去 上 : REMOVE_UP = 10  
- 除去 右上 : REMOVE\_UP\_RIGHT = 11  
- 除去 右 : REMOVE_RIGHT = 12  
- 除去 右下 : REMOVE\_DOWN\_RIGHT = 13  
- 除去 下 : REMOVE_DOWN = 14  
- 除去 左下 : REMOVE\_DOWN\_LEFT = 15  
- 除去 左 : REMOVE_LEFT = 16

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
- vector&lt;vector&lt;int&gt;&gt; mFieldPointsArray              : フィールドの点数（setFieldで代入）  
- vector&lt;vector&lt;int&gt;&gt; mFieldStatusArray              : フィールドの状況（setFieldで代入）  
- vector&lt;vector&lt;int&gt;&gt; mFieldAgentsIDArray            : フィールド上のエージェントの場所とエージェントID番号（setFieldで設定）  
- vector&lt;vector&lt;bool&gt;&gt; mFieldAllyAreaSquaresArray     : 味方の領域(領域内のマスはtrue, 外のマスはfalse)  
- vector&lt;vector&lt;bool&gt;&gt; mFieldEnemyAreaSquaresArray   : 相手の領域(領域内のマスはtrue, 外のマスはfalse)  
- vector&lt;vector&lt;bool&gt;&gt; mFieldAreaSideLinesArray      : 領域を成す辺（辺を成すマスはtrue, それ以外はfalse）  
  
- vector&lt;int&gt; mAllyAgentsActNumbers              : 味方エージェントの行動の番号  
- vector&lt;int&gt; mEnemyAgentsActNumbers             : 相手エージェントの行動の番号  
    
- Score mAllyScore                              : 味方チームの得点（合計点: mAllyScore.total, タイルポイント: mAllyScore.tile, 領域ポイント: mAllyScore.area）  
- Score mEnemyScore                              : 相手チームの得点（合計点: mEnemyScore.total, タイルポイント: mEnemyScore.tile, 領域ポイント: mEnemyScore.area）  
  
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
  
- const vector&lt;vector&lt;int&gt;&gt; &argFieldPointsArray    : フィールドの得点の配列  
- const vector&lt;vector&lt;int&gt;&gt; &argFieldStatusArray    : フィールドの状態を示す配列  
- const Vec2 position                               : フィールドの大きさを示す配列（例：Vec2{10, 10}）  
- const bool argAllyTeamColor                       : 味方チームの色（TeamColor::RED or TeamColor::BLUE）
    
#### void Field::setField
フィールドの得点、状況をプライベート配列にコピーし、エージェントにIDを振り分けます。  
IDはフィールドの左上から順に振り分けられ、味方のエージェントのIDは1から上がっていき、相手のエージェントのIDは-1から下がっていきます。  
この関数は、基本的にFieldコンストラクタから呼び出されます。  
例）エージェントが各チーム3人のとき、味方のエージェントIDは1〜3、相手のエージェントIDは-1〜-3  
  
- const vector&lt;vector&lt;int&gt;&gt; &argFieldPointsArray    : フィールドの得点の配列  
- const vector&lt;vector&lt;int&gt;&gt; &argFieldStatusArray    : フィールドの状態を示す配列
    
#### void Field::searchAreaPointsSide
再帰関数です。  
フィールド上に領域が存在するかどうかを判断し、存在したら領域ポイントを設定します。  
引数で指定した座標のタイルから同じ色のタイルをたどっていき、同じタイルを二度通ることなく最初のタイルまで戻ってこられたら、フィールド上に領域が存在すると仮定します。  
その後、領域と仮定したタイルの間に、相手チームあるいは白いタイルが存在するか調査し、存在したら領域として認定します。  
  
- vector&lt;vector&lt;bool&gt;&gt; argFieldMark     : フィールドの領域に認定したかどうか（一時的な配列）  
- const int argStartX                   : 領域探索の始点のマスのx座標  
- const int argStartY                   : 領域探索の始点のマスのy座標  
- const int argX                        : 現在探索中のマスのx座標  
- const int argY                        : 現在探索中のマスのy座標  
- Node* argParentNode                   : 親ノードのポインタ  

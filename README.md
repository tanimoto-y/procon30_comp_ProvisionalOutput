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

### Main.cpp
#### centerText
##### 概要
指定した座標の中央に文字を表示します。文字列用と数値用があります。

##### 引数
- const String argStr       : 表示する文字列（文字列用）  
- const int argInt          : 表示する数値（数値用）  
- const int argLeftTopX     : 左上のx座標  
- const int argLeftTopY     : 左上のy座標  
- const int argRightBottomX : 右下のx座標  
- const int argRightBottomX : 右下のy座標  
- const Font& argFont       : 表示に使うフォントのポインタ
- const Color argColor      : 文字の色

#### printTotalPoints
##### 概要
点数を表示します。

##### 引数
- const Team::Type argTeam            : どちらのチームを表示するか（Team::ALLY or Team::ENEMY）  
- const bool argTeamColor             : チームの色（TeamColor::RED or TeamColor::BLUE）  
- Field& argFieldData                 : フィールドクラスのポインタ  
- const Font& argFont16               : サイズ16のフォントのポインタ  
- const Font& argFont16Bold           : サイズ16の太字フォントのポインタ
- const Font& argFont28               : サイズ28のフォントのポインタ
- const Font& argTotalPointsTextFont  : サイズ28の太字フォントのポインタ

#### Main
##### 概要
- const Font& argFont28const Font& argTotalPointsTextFont
Main関数です。ここからそれぞれの関数を呼び出します。

#### 引数
なし

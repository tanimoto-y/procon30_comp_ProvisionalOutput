# procon30_ProvisionalController

## 概要
競技部門用の入出力部です。採用されるかもしれないされないかもしれません。

## 動作確認環境
- macOS 10.14 mojave
  - Xcode 10.2.1

## 使用環境
- C++
- OpenSiv3D v0.3.1

## 実行方法 
OpenSiv3Dのファイルはサイズが大きいため、このリポジトリにはXcodeのプロジェクトファイルのみ置いてあります。  
このままではビルドできません。Xcodeでビルドして動かすには、OpenSiv3Dのライブラリファイルなどが必要です。  
（ただし、OpenSiv3Dを導入しなくても`projects/ProvisionalOutput/bin/`にあるempty.appから実行することはできます）  

### OpenSiv3Dの導入方法
OpenSiv3Dの導入は簡単で、ファイルをコピーするだけです。  

#### macOSの場合
1. 以下のサイトから、OpenSiv3D プロジェクトテンプレート（zip形式）をダウンロードします。  
https://scrapbox.io/Siv3D/OpenSiv3D_%E3%82%92%E3%81%AF%E3%81%98%E3%82%81%E3%82%8B%E6%BA%96%E5%82%99_(macOS)
2. ダウンロードが完了したら、zipファイルを展開し、libフォルダとincludeフォルダをローカルリポジトリのフォルダ`procon30_comp_ProvisionalOutput`にコピーします。

#### Windowsの場合
（準備中）

### プロジェクトを開く方法
Macの場合はXcodeで使用してください（CLionだと導入が難しい）。  
Windowsの場合はVisual Studio 2019を使用してください。

### Xcodeで開く場合
1. `procon30_comp_ProvisionalOutput/projects/ProvisionalOutput/provisional_IO.xcodeproj`をXcodeで開く。
2. 画面左上の再生ボタンのようなボタンを押す。

### Visual Studioで開く場合
（準備中）

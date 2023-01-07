# PPT Breaker for X680x0

A breakout clone powered by PPTX for X680x0

---

## About This

これは X680x0 のアプリケーションをC言語で約30年ぶりに書いてみる練習の一環として作ったブロック崩しもどきのゲームです。
エミュレータ上でしか確認していませんが、一応10MHz機でも大丈夫にしたつもりです。

---

## 起動方法

PPTBRKxxx.ZIP をダウンロードし展開して、PPTBREAK.X を実行します。

---

## 遊び方

ジョイスティックもしくはキーボードでパドルを左右に動かしてボールを落ちないように打ち返します。
ラウンド3以降はブロックが動く場合があります。ラウンド4が最終ステージです。

---

## 開発環境

* クロスコンパイル
  * [xdev68k](https://github.com/yosshin4004/xdev68k/) ([gcc 12.2.0に変更](https://github.com/tantanGH/distribution/blob/main/INSTALL_xdev68k_M1Mac.md)) Thanks to ファミべのよっしんさん
  * HAS060.X & LK.X on run68mac Thanks to Yunkさん / M.Kamadaさん / GOROmanさん
  * SHARP XC 2.1 ライブラリ
  * Microsoft Visual Studio Code
  * Apple MacBoor Air (2020, Apple Silicon, RAM8GB, SSD512GB)

* スプライトデータ作成
  * 透過PNG作成 Microsoft PowerPoint
  * データコンバート [png2sp](https://github.com/tantanGH/png2sp/)

* タイトルロゴデータ作成
  * [Cool Text](https://ja.cooltext.com/)
  * データコンバート [pngdeband](https://github.com/tantanGH/pngdeband/), [png2sp](https://github.com/tantanGH/png2sp/)

* サウンドデータ(SE)作成
  * [peko-step 効果音メーカー](https://www.peko-step.com/tool/soundeffect/)
  * データコンバート [wav2adpcm](https://github.com/tantanGH/wav2adpcm/)

* サウンドデータ(MUSIC)フリー素材
  * [DOVA-SYNDROME](https://dova-s.jp/) "Piece of a Dream" Thanks to FLASH☆BEATさん
  * データコンバート [wav2adpcm](https://github.com/tantanGH/wav2adpcm/)

* 動作確認
  * [XEiJ](https://stdkmd.net/xeij/) Thanks to M.Kamadaさん

ソースコードのコンパイルには xdev68k 環境が必須です。X680x0/Human68k 上の gcc/gcc2 コンパイラではコンパイルできません。
HLK301.X は run68上で Out of memory が多発するので純正の LK.X を使っています。

---

## 動作デモ

![](images/15.gif)

![](images/16.gif)

---

## 実装と設計

これから X680x0 C言語ゲームプログラミングを再開したい、新たに始めたいという方に向けて、実装と設計について覚書を残しておきます。
ただし、あくまで素人が約30年ぶりに思い出しながら、あるいは新たに試行錯誤しながら作成したものですので、ベストプラクティスというわけではなく、あくまで一例ということでお願いします。また、オールアセンブラでガリガリというのとは対極で、ゆるく楽にをモットーにC言語での実装です。
ただし、オブジェクト指向的なデザインを取り入れて、なるべく保守性を高めるように気を遣ってあります。

### 全体デザイン

![](images/design1.png)


# PPT Breaker for X680x0

A breakout clone powered by PPTX for X680x0

---

## About This

ブロック崩しもどきのゲームです。ゲーム制作素人が X680x0 のプログラムを約30年ぶりにC言語で書いてみる練習として作ったものです。
なお実機を所有していないので、エミュレータ上でしか確認していません。

* xdev68k Cソースコード解説付
* 384x256画面モード
* 1/60 VSYNC 割込
* 画面スクロール
* ジョイスティック
* スプライトデザインにPowerPoint使用

動作報告・不具合報告歓迎

---

## 起動方法

PPTBKxxx.ZIP をダウンロードし展開して、PPTBREAK.X を実行します。

---

## 遊び方

ジョイスティックもしくはキーボードでパドルを左右に動かしてボールを落ちないように打ち返します。
1番上の段のブロックは2回当てないと消せません。
ラウンド3以降は一部のブロックが動く場合があります。ラウンド4が最終ステージです。

---

## 開発環境

* クロスコンパイル
  * [xdev68k](https://github.com/yosshin4004/xdev68k/) ([gcc 12.2.0に変更](https://github.com/tantanGH/distribution/blob/main/INSTALL_xdev68k_M1Mac.md)) Thanks to ファミべのよっしんさん
  * HAS060.X on run68mac Thanks to yunkさん / M.Kamadaさん / GOROmanさん
  * SHARP XC 2.1 ライブラリ & LK.X
  * Microsoft Visual Studio Code
  * Apple MacBook Air (2020, Apple Silicon, RAM8GB, SSD512GB)

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

---

## 動作デモ

![](images/15a.gif)

![](images/16a.gif)

![](images/17a.gif)

---

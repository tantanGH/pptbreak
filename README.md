# PPT Breaker for X680x0

A breakout clone powered by PPTX for X680x0

---

## About This

これは X680x0 のアプリケーションをC言語で約30年ぶりに書いてみる練習として作ったブロック崩しもどきのゲームです。
エミュレータ上でしか確認していませんが、一応10MHz機でも大丈夫にしたつもりです。

---

## 起動方法

PPTBRKxxx.ZIP をダウンロードし展開して、PPTBREAK.X を実行します。

---

## 遊び方

---

## 開発環境

* クロスコンパイル環境
  * MacBoor Air (Apple Silicon, 2020)
  * [xdev68k](https://guthub.com/yosshin4004/xdev68/) (gcc 12.2.0)
  * XC 2.1 ライブラリ

* スプライトデータ作成
  * 透過PNG作成 Microsoft PowerPoint
  * データコンバート [png2sp](https://github.com/tantanGH/png2sp/)

* タイトルロゴデータ作成
  * [Cool Text](https://ja.cooltext.com/)
  * データコンバート [png2sp](https://github.com/tantanGH/png2sp/)

* サウンドデータ作成

ソースコードのコンパイルには xdev68k 環境が必須です。X680x0/Human68k 上の gcc/gcc2 コンパイラではコンパイルできません。
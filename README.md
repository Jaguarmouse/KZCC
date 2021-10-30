# KZCC
C言語＆低レイヤ学習を目的とした個人制作Cコンパイラ

inspired by https://www.sigbus.info/compilerbook

## Features
- 整数の四則演算
- 整数の比較演算
- `if...else`文
- `for`, `while`文
- ブロック
- `return`文
- `;`(セミコロン)区切りの複数文
- 整数型ローカル変数の定義
- 6つまでの引数をとる関数呼び出し
- 暗黙的な`main`関数

## Techniques Learned
- 再帰下降構文解析
- スタックマシン
- アセンブリ言語(x86-64)

#### Sub-techniques
- Docker & Linux
- Makefile & 分割コンパイル
- 生成規則 & BNF記法

# 湯呑(YuNomi)

## これは何?
データ圧縮に関するユーティリティを収録したライブラリ集（になる予定のもの）です。

## 実装されているもの
現在はランダムアクセス可能なフィボナッチコーディング配列のみ実装されています。
これは、以下の論文に記載されています。
* Kimmo Fredriksson and Fedor Nikitin. 2007. Simple compression code supporting random access and fast string matching. In Proc. of the 6th international conference on Experimental algorithms (WEA'07). Springer-Verlag. <http://dl.acm.org/citation.cfm?id=1551895>
* Kimmo Fredriksson and Fedor Nikitin. 2009. Simple Random Access Compression. Fundam. Inf. 92, 1-2 (January 2009). <http://dl.acm.org/citation.cfm?id=1768592>

内部で作成しているselect辞書は、darrayを採用しました。
darrayは以下の論文に記載されています。
* "Practical Entropy-Compressed Rank/Select Dictionary.", Daisuke Okanohara and Kunihiko Sadakane. In the Proc. of ALENEX 2007. <http://hillbig.cocolog-nifty.com/do/2006/12/bit_arrayrankse_036c.html>

## 実装されていないもの（実装するつもりのもの）
darrayの内部で線形探索する際に、以下の論文に従えば高速化できるはずですが、まだ実装できていません。（現時点ではシンプルに線形探索しているため重いです。）

* Sebastiano Vigna. 2008. Broadword implementation of rank/select queries. In Proc. of the 7th international conference on Experimental algorithms (WEA'08). Springer-Verlag. <http://dl.acm.org/citation.cfm?id=1788900>

## インストール
wafのビルド定義を付属しています。
下記サイトよりwafをダウンロードしてください。
* <http://code.google.com/p/waf/>

コンパイルには、以下のコマンドを実行します。
> ./waf configure build install --prefix=インストール先

コードはすべてヘッダに記載されています。includeディレクトリをヘッダの探索パスに追加してください。
ライブラリの使い方はサンプルコードを参照してください。

以上

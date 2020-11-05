# Hisui をビルドする

Ubuntu 20.04 x86_64 でのみビルドを確認しています。

## 事前インストール

必要なライブラリをインストールします。

```
apt install cmake clang libc6-dev yasm
```

## ビルド


```
./build.bash ubuntu-20.04_x86_64
```

## バイナリ

release ディレクトリに hisui バイナリが生成されます。

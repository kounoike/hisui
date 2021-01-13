# 変更履歴

- CHANGE
    - 下位互換のない変更
- UPDATE
    - 下位互換がある変更
- ADD
    - 下位互換がある追加
- FIX
    - バグ修正

## develop

## 2021.1

- [ADD] OpenH264 を利用した WebM 中の H.264 の decode
    - @haruyama
- [ADD] cpp-mp4 を利用した MP4 の出力
    - @haruyama
- [ADD] libfdk-aac を利用した MP4 への AAC の出力
    - @haruyama
- [UPDATE] `boost` を `1.75.0` に
    - @haruyama
- [UPDATE] `fmt` を `7.1.3` に
    - @haruyama
- [UPDATE] `spdlog` を `1.8.2` に
    - @haruyama
- [CHANGE] `nlohmann::json` から `boost::json` への切り替え
    - @haruyama

## 2020.1.1

- [FIX] Video のない WebM ファイルを利用した場合の取り扱いを修正
    - @haruyama

## 2020.1

**祝リリース**

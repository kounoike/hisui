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

- [ADD] [実験的機能] 合成成功時にレポートを出力する
    - @haruyama
    - https://github.com/shiguredo/hisui/pull/30
- [UPDATE] `cpp-mp4` を `2021.2` に
    - @haruyama
- [ADD] 音声の mix のみを行なう --audio-only コマンドラインオプションの追加
    - @haruyama
    - https://github.com/shiguredo/hisui/pull/26
- [UPDATE] --libvp9-tile-columns コマンドラインオプションの追加
    - @haruyama
    - https://github.com/shiguredo/hisui/pull/24
    - https://github.com/shiguredo/hisui/pull/28
- [UPDATE] WebM/MP4 Muxer の mux() を共通化
    - @haruyama
    - https://github.com/shiguredo/hisui/pull/23

## 2021.1.1

- [FIX] std::async で作った Future を get() し例外を伝播させる
    - @haruyama
    - https://github.com/shiguredo/hisui/pull/22
- [FIX] 解像度の変更が入っている H.264 の WebM を合成しようとすると落ちるのを修正
    - https://github.com/shiguredo/hisui/pull/21
    - @haruyama

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

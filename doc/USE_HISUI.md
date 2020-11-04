# Hisui を利用してみる

## docker 経由で利用する

Hisui は docker image を用意しています。これを使うことで x86_64 環境であれば気軽に Hisui を利用可能です。

- https://hub.docker.com/r/shiguredo/hisui
- https://github.com/orgs/shiguredo/packages/container/package/hisui


```
docker run -v /home/shiguredo/sora-2020.3/archive:/hisui ghcr.io/shiguredo/hisui:2020.1 -f /hisui/CSX77QY9F57V5BT72S62C28VS4.json
```

- -v で Sora の録画データがある archive フォルダを指定して下さい
    - docker 側のフォルダはどこでも良いですがここでは /hisui を利用しています
- -f で合成したい recording.report が生成するファイルを指定して下さい


### docker pull だけしておきたい場合

```
docker pull ghcr.io/shiguredo/hisui:2020.1
```

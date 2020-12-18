# Hisui を利用してみる

## docker 経由で利用する

Hisui は docker image を用意しています。これを使うことで x86_64 環境であれば気軽に Hisui を利用可能です。

- https://hub.docker.com/r/shiguredo/hisui
- https://github.com/orgs/shiguredo/packages/container/package/hisui

```
docker run -v /home/shiguredo/sora-2020.3/archive:/hisui ghcr.io/shiguredo/hisui:2020.1.1 -f /hisui/CSX77QY9F57V5BT72S62C28VS4.json
```

- -v で Sora の録画データがある archive フォルダを指定して下さい
    - docker 側のフォルダはどこでも良いですがここでは /hisui を利用しています
- -f で合成したい recording.report が生成するファイルを指定して下さい


## docker 経由で help を見る

```
$ docker run ghcr.io/shiguredo/hisui:2020.1.1 hisui --help
hisui
Usage: /usr/local/bin/hisui [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--in-metadata-file       Metadata filename (REQUIED)
  --out-video-codec           Video codec (VP8/VP9) default: VP9
  --out-video-frame-rate      Video frame rate (INTEGER/RATIONAL) default: 25)
  --out-webm-file             Output filename
  --max-columns               Max columns (POSITIVE INTEGER) default: 3
  --libvpx-cq-level           libvpx Constrained Quality level (NON NAGATIVE INTEGER) default: 10
  --libvpx-min-q              libvpx minimum (best) quantizer (NON NEGATIVE INTEGER) default: 3
  --libvpx-max-q              libvpx maximum (worst) quantizer (NON NEGATIVE INTEGER) default: 40
  --verbose                   Verbose mode
  --show-progress-bar         Toggle to show progress bar. default: true
```



## 自前ビルドで利用したい場合

-f で合成したい recording.report が生成するファイルを指定して下さい。

```
./hisui -f CSX77QY9F57V5BT72S62C28VS4.json
```

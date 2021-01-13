#compdef hisui

function _hisui() {
    local context curcontext=$curcontext state line
    typeset -A opt_args
    local ret=1

    _arguments -C \
        '(-h --help)'{-h,--help}'[help]' \
        '(-f --in-metadata-file)'{-f,--in-metadata-file}:'[input file]':_files \
        '--out-container':'[output container (WebM)]:(WebM MP4)' \
        '--out-video-codec':'[output video codec (VP9)]:(VP8 VP9)' \
        '--out-audio-codec':'[output audio codec (Opus)]:(Opus AAC)' \
        '--out-video-frame-rate':'[output video frame rate (25)]' \
        '--out-file':'[output filename]':_files \
        '--openh264':'[openh264 dynamic library path]':_files \
        '--max-columns':'[max columns (3)]:($(seq 1 10))' \
        '--libvpx-cq-level':'[libvpx Constrained Quality level (10)]:($(seq 0 63))' \
        '--libvpx-min-q':'[libvpx minimum (best) quantizer (3)]:($(seq 0 63))' \
        '--libvpx-max-q':'[libvpx maximum (worst) quantizer (40)]:($(seq 0 63))' \
        '--mp4-muxer':'[mp4 muxer (Faststart/Simple)]:(Faststart Simple)' \
        '--verbose':'[verbose mode]' \
        '--show-progress-bar':'[toggle to show progress bar]' \
        '--out-video-bit-rate':'[output video bit rate (200 x (number of inputs))]' \
        '--out-opus-bit-rate':'[output opus bit rate]' \
        '--out-aac-bit-rate':'[output aac bit rate]' \
        '--libvpx-threads':'[libvpx max number of threads (0, use system default)]' \
        '--libvpx-cpu-used':'[libvpx cpu used (4)]:($(seq -16 16))' \
        '--libvp9-frame-parallel':'[libvpx frame parallel (0)]:((0 1))' \
        '(-)*:: :->null_state' \
        && ret=0
    return ret
}

_hisui "$@"

#!/usr/bin/env bash
_hisui() {
    local cur prev
    _init_completion || return

    case $prev in
        -f|--in-metadata-file)
            _filedir
            return
            ;;
        --openh264)
            _filedir
            return
            ;;
        --out-file)
            _filedir
            return
            ;;
        --out-container)
            mapfile -t COMPREPLY < <(compgen -W 'WebM MP4' -- "$cur")
            return
            ;;
        --out-video-codec)
            mapfile -t COMPREPLY < <(compgen -W 'VP8 VP9' -- "$cur")
            return
            ;;
        --out-audio-codec)
            mapfile -t COMPREPLY < <(compgen -W 'Opus AAC' -- "$cur")
            return
            ;;
        --max-columns)
            mapfile -t COMPREPLY < <(compgen -W '{1..10}' -- "$cur")
            return
            ;;
        --libvpx-cq-level|--libvpx-min-q|--libvpx-max-q)
            mapfile -t COMPREPLY< <(compgen -W '{0..63}' -- "$cur")
            return
            ;;
        --libvpx-threads)
            mapfile -t COMPREPLY< <(compgen -W "{1..$(( $(_ncpus)+1 ))}" -- "$cur")
            return
            ;;
        --mp4-muxer)
            mapfile -t COMPREPLY < <(compgen -W 'Faststart Simple' -- "$cur")
            return
            ;;
        -h|--help|--verbose|--show-progress-bar|--out-video-frame-rate|--out-video-bit-rate|--out-opus-bit-rate|--out-aac-bit-rate)
            return
            ;;
    esac

    mapfile -t COMPREPLY< <(compgen -W '-h --help -f --in-metadata-file --openh264 --out-container --out-video-codec --out-audio-codec --out-video-frame-rate --out-file --out-file --max-columns --libvpx-cq-level --libvpx-min-q --libvpx-max-q --mp4-muxer --verbose --show-progress-bar --out-video-bit-rate --out-opus-bit-rate --out-aac-bit-rate --libvpx-threads' -- "$cur")
}

complete -F _hisui hisui

#!/usr/bin/env bash
_hisui() {
    local cur prev
    _init_completion || return

    case $prev in
        -f|--in-metadata-file)
            _filedir
            return
            ;;
        --out-webm-file)
            _filedir
            return
            ;;
        --out-video-codec)
            mapfile -t COMPREPLY < <(compgen -W 'VP8 VP9' -- "$cur")
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
        -h|--help|--verbose|--show-progress-bar|--out-video-frame-rate|--out-video-bit-rate)
            return
            ;;
    esac

    mapfile -t COMPREPLY< <(compgen -W '-h --help -f --in-metadata-file --out-video-codec --out-video-frame-rate --out-webm-file --max-columns --libvpx-cq-level --libvpx-min-q --libvpx-max-q --verbose --show-progress-bar --out-video-bit-rate --libvpx-threads' -- "$cur")
}

complete -F _hisui hisui

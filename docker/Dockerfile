FROM ubuntu:20.04
LABEL maintainer="HARUYAMA Seigo <haruyama@tankyu.net>"

# webm の mux
# docker run -v /home/haruyama/src/Sora/sora-2020.1.1/archive:/home/haruyama/src/Sora/sora-2020.1.1/archive hisui-phase0.5:0.1 -f /home/haruyama/src/Sora/sora-2020.1.1/archive/test.json

WORKDIR /work
COPY release/hisui /work
ENTRYPOINT ["/work/hisui"]

FROM  alpine:latest
RUN   adduser -S -D -H -h /xmrig miner
RUN   apk --no-cache upgrade && \
      apk --no-cache add \
        git \
        cmake \
        libuv-dev \
        build-base && \
      git clone https://github.com/dbiesecke/xmrig && \
      cd xmrig && \
      sh ./build-alpine.sh \
      apk del \
        build-base \
        cmake \
        git
#USER miner
WORKDIR    /xmrig
ENTRYPOINT  ["/bin/sh"]

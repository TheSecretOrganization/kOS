# Name: krash
# Tag: 0.0.1

FROM docker.io/alpine:3.22.1@sha256:4bcff63911fcb4448bd4fdacec207030997caf25e9bea4045fa6c8c44de311d1

ENV PIPX_HOME="/pipx"
ENV PIPX_BIN_DIR="$PIPX_HOME/bin"
ENV PATH="/kos_cross/bin:${PIPX_BIN_DIR}:${PATH}"
SHELL ["/bin/ash", "-eo", "pipefail", "-c"]

RUN apk add --no-cache curl=8.14.1-r1 \
    && mkdir -p /kos_cross/bin \
    && curl -fsSL --proto =https --proto-redir =https https://newos.org/toolchains/i386-elf-7.5.0-Linux-x86_64.tar.xz \
    | tar -xJ --strip-components=1 -C /kos_cross

RUN apk add --no-cache pipx=1.7.1-r0 \
    && mkdir -p "$PIPX_HOME" "$PIPX_BIN_DIR" \
    && pipx install clang-format==20.1.8

RUN apk add --no-cache \
    bash=5.2.37-r0 \
    gcompat=1.1.0-r4 \
    make=4.4.1-r3 \
    nasm=2.16.03-r0

ENTRYPOINT [ "/bin/bash" ]

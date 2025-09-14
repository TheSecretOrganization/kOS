#!/bin/bash

set -e

NAME="clang-format"
V_REQUIRED="20.1.8"
PACKAGE="$NAME==$V_REQUIRED"
CLANG_FORMAT_CMD="$(command -v $NAME)"
PIPX_RUN_CMD="pipx run $PACKAGE"

usage() {
    printf "%s:
\t--help                  \tprints this message.
\t--install               \tinstalls %s with pipx if possible.
\t--override </path/to/bin>\tallows to use an arbitrary binary and bypass the validation.
\tAnything else will be passed as argurments to %s.
" "$0" "$NAME" "$NAME"
}

install() {
    if ! command -v pipx >/dev/null 2>&1; then
        echo "pipx is not installed, please install pipx and try again."
        exit 1
    fi

    pipx install $PACKAGE
}

check_version() {
    V_INSTALLED="$($NAME --version 2>&1 | grep -oE '[0-9]+(\.[0-9]+)+')"
    if [[ -z "${V_INSTALLED}" ]]; then
        echo "A error occured, please try again later."
        exit 1
    fi

    if [ "$(printf '%s\n' "$V_INSTALLED" "$V_REQUIRED" | sort -V | tail -n1)" = "$V_INSTALLED" ]; then
        echo "Installed version: $V_INSTALLED >= required version: $V_REQUIRED, all good!"
    else
        echo "Installed version: $V_INSTALLED < required version: $V_REQUIRED :("
        install
        CLANG_FORMAT_CMD="$PIPX_RUN_CMD"
    fi
}

if [[ "$1" == "--help" ]]; then
    usage
    exit 0
elif [[ "$1" == "--install" ]]; then
    install
    exit 0
fi

if [[ "$1" == "--override" ]] && [[ ! -z "${2}" ]]; then
    echo "Overriding checks with: $2"
    CLANG_FORMAT_CMD="$2"
    shift 2
elif [[ -z "${CLANG_FORMAT_CMD}" ]]; then
    echo "$NAME not installed, installing with pipx..."
    install
else
    echo "$NAME already installed, checking the version..."
    check_version
fi

echo "Executing command: '$CLANG_FORMAT_CMD', bye!"
exec $CLANG_FORMAT_CMD "$@"

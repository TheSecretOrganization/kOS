#!/bin/bash

set -e

CLANG_FORMAT="$(command -v clang-format)"
V_REQUIRED="20.1.8"

check_pipx_install() {
    if command -v pipx >/dev/null 2>&1; then
        echo "pipx is installed and will install clang-format==$V_REQUIRED!"
        CLANG_FORMAT="pipx run clang-format==$V_REQUIRED"
    else
        echo "pipx is not installed, please install clang-format==$V_REQUIRED directly or install pipx and try again."
        exit 1
    fi
}

check_installed_version() {
    V_INSTALLED="$(clang-format --version 2>&1 | grep -oE '[0-9]+(\.[0-9]+)+')"
    [[ -z "${V_INSTALLED}" ]] && return 1

    echo "clang-format installation detected! Checking version..."

    if [ "$(printf '%s\n' "$V_INSTALLED" "$V_REQUIRED" | sort -V | tail -n1)" = "$V_INSTALLED" ]; then
        echo "Installed version: $V_INSTALLED >= required version: $V_REQUIRED, all good!"
    else
        echo "Installed version: $V_INSTALLED < required version: $V_REQUIRED :("
        check_pipx_install
    fi
}

if [[ -z "${CLANG_FORMAT}" ]]; then
    check_pipx_install
else
    check_installed_version
fi

echo "Executing formatter: '$CLANG_FORMAT', bye!"
exec $CLANG_FORMAT "$@"

#!/bin/bash

set -e
set -o pipefail
set -u

PS4='+(${BASH_SOURCE}:${LINENO}): ${FUNCNAME[0]:+${FUNCNAME[0]}(): }'

sudo apt-get update
sudo apt-get install -y --no-install-recommends \
    build-essential \
    ca-certificates \
    cmake \
    curl \
    git \
    gnupg \
    ninja-build \
    libasound2-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libvulkan-dev \
    libx11-dev \
    libxcursor-dev \
    libxext-dev \
    libxi-dev \
    libxinerama-dev \
    libxrandr-dev \
    lsb-release \
    mingw-w64 \
    nsis \
    pkg-config \
    rpm \
    sudo \
    vulkan-tools \
    vulkan-validationlayers-dev \
    wget \
    wine

mkdir -p "$HOME/dev"

# https://apt.llvm.org
wget https://apt.llvm.org/llvm.sh -O "$HOME/dev/llvm.sh"
chmod +x "$HOME/dev/llvm.sh"
sudo "$HOME/dev/llvm.sh" 20 all

find /usr/lib/llvm-20/bin \( -type f -o -type l \) -print0 \
    | xargs -0 -I %% \
        bash -c 'sudo update-alternatives --install /usr/bin/$(basename "${1}") $(basename "${1}") "${1}" 100' -- '%%'

# https://apt.kitware.com/
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null \
    | gpg --dearmor - \
    | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' \
    | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null
sudo apt-get update -qq
sudo apt-get install -y --no-install-recommends \
    cmake

# emscripten
if [ ! -d "$HOME/emsdk" ]; then
    git clone https://github.com/emscripten-core/emsdk.git "$HOME/emsdk"
fi

pushd "$HOME/emsdk" || exit
git pull
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
popd || exit

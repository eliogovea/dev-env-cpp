FROM ubuntu:bionic

ARG USERNAME=user
ARG USER_UID=1000
ARG USER_GID=${USER_UID}

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update -qq \
    && apt-get install -y --no-install-recommends \
        build-essential \
        ccache \
        cppcheck \
        gcovr \
        gdb \
        git \
        gpg-agent \
        file \
        python3 \
        python3-pip \
        software-properties-common \
        ssh \
        sudo \
        valgrind \
        wget

# gcc-[7, 8, 9, 10, 11] 
ARG GCC_VERSION="11"

# Add gcc-${GCC_VERSION}
RUN export DEBIAN_FRONTEND=noninteractive \
    && add-apt-repository -y ppa:ubuntu-toolchain-r/test \
    && apt-get update -qq \
    && apt-get install -y --no-install-recommends \
        gcc-${GCC_VERSION} \
        g++-${GCC_VERSION} \
    && update-alternatives --install /usr/bin/gcc gcc $(which gcc-${GCC_VERSION}) 100 \
    && update-alternatives --install /usr/bin/g++ g++ $(which g++-${GCC_VERSION}) 100


# clang-[10, 11, 12]
ARG LLVM_VERSION="12"

# Add clang-${LLVM_VERSION}
RUN export DEBIAN_FRONTEND=noninteractive \
    && wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - 2>/dev/null \
    && add-apt-repository -y "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-${LLVM_VERSION} main" \
    && apt-get update -qq \
    && apt-get install -y --no-install-recommends \
        clang-${LLVM_VERSION} \
        lldb-${LLVM_VERSION} \
        lld-${LLVM_VERSION} \
        clangd-${LLVM_VERSION} \
        llvm-${LLVM_VERSION}-dev \
        libclang-${LLVM_VERSION}-dev \
        clang-tidy-${LLVM_VERSION} \
        clang-format-${LLVM_VERSION} \
    && update-alternatives --install /usr/bin/clang clang $(which clang-${LLVM_VERSION}) 100 \
    && update-alternatives --install /usr/bin/clang++ clang++ $(which clang++-${LLVM_VERSION}) 100 \
    && update-alternatives --install /usr/bin/clang-tidy clang-tidy $(which clang-tidy-${LLVM_VERSION}) 1 \
    && update-alternatives --install /usr/bin/clang-format clang-format $(which clang-format-${LLVM_VERSION}) 1

ENV CC="clang"
ENV CXX="clang++"

# Add cmake/ccmake
RUN export DEBIAN_FRONTEND=noninteractive \
    && wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null \
        | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null \
    && apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main' \
    && apt-get update -qq \
    && apt-get install -y --no-install-recommends \
        cmake \
        cmake-curses-gui

# Install include-what-you-use
ENV IWYU /home/iwyu
ENV IWYU_BUILD ${IWYU}/build
ENV IWYU_SRC ${IWYU}/include-what-you-use
RUN export DEBIAN_FRONTEND=noninteractive \
    && mkdir -p ${IWYU_BUILD} \
    && git clone --branch clang_${LLVM_VERSION} \
        https://github.com/include-what-you-use/include-what-you-use.git \
        ${IWYU_SRC}

RUN export DEBIAN_FRONTEND=noninteractive \
    && CC=clang-${LLVM_VERSION} CXX=clang++-${LLVM_VERSION} cmake \
        -S ${IWYU_SRC} \
        -B ${IWYU_BUILD} \
        -G "Unix Makefiles" -DCMAKE_PREFIX_PATH=/usr/lib/llvm-${LLVM_VERSION} \
    && cmake --build ${IWYU_BUILD} -j \
    && cmake --install ${IWYU_BUILD} \
    # https://github.com/include-what-you-use/include-what-you-use#how-to-install
    && mkdir -p $(include-what-you-use -print-resource-dir 2>/dev/null) \
    && ln -s $(readlink -f /usr/lib/clang/${LLVM_VERSION}/include) \
        $(include-what-you-use -print-resource-dir 2>/dev/null)/include

# Clean
RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Create the user
RUN export DEBIAN_FRONTEND=noninteractive \
    && groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

USER $USERNAME
WORKDIR /home/$USERNAME
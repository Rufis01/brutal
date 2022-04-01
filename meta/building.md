# Building

bruwutal can be built by using either LLVM or GNU toolchains.

In both cases you will need `nasm` and `make`.

On Arch Linux this can be done by doing `pacman -S make nasm`

> Run `git submodule update --init --recursive` to install the submodules before building

## With LLVM

Before trying to build bruwutal with LLVM you need to install `lld` and `clang-12`.

On Arch Linux this can be done by doing `pacman -S lld clang`.

Build bruwutal via the LLVM toolchain:

`make all`

> On some distros you might want to change the llvm version using `LLVM_VERSION=-12` or any other version.

## With GNU

In order to build bruwutal with the GNU toolchain you will need to build the x86_64-elf binutils. This can be done by running the script in `build/toolchain/gnu/build.sh`.

After building the binutils you can build bruwutal by running:

`make all TOOLCHAIN=gnu`

## Note About Windows

*Just switch to GNU+Linux*

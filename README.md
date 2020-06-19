# Cryptopals

This repository contains my personal solutions to the
[Cryptopals](https://cryptopals.com/) challenges. See `docs/challenges.md` for
my notes about the challenges.

## Building

This project uses the [Meson](https://mesonbuild.com/) build system. To build
the code, use the following commands:

``` shell
$ meson setup build/ -Demilk-loguru:loguru_replace_glog=true
$ ninja -C build/ all
```

I'm using `gcc-10` to take advantage of the latest C++20 standards. You will
need to patch build errors in the `abseil-cpp` project (it doesn't support C++20
yet).

## Testing

Tests are written with [Googletest](https://github.com/google/googletest) and
executed from Meson. To run the tests, use the following commands:

``` shell
$ ninja -C build/ test
```

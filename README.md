# libsass for Dart

![libsass+dart](http://i.imgur.com/kh8QIRG.png)

A Dart VM Native Extension for libsass.

This package provides Dart language bindings for the [libsass](http://libsass.org/) library, the C version of the popular stylesheet preprocessor, Sass.

## Build Instructions

Because this library contains a Dart VM Native Extension, you will need to compile the extension before using the library. Currently, the build script only supports Linux and Mac (untested):

```shell
git clone https://github.com/oddrationale/dart_libsass.git
cd dart_libsass
pub get
dart tool/build.dart
```

This script will pull the libsass submodule, compile libsass, and compile the extenstion. Support for Windows is coming soon!

## Command Line

This package also provides a `sassc` compatible executable:

```shell
dart bin/sassd.dart input.scss output.css
```

#!/bin/bash

set -e

cd $(dirname $0)
PROJECT_ROOT=$(pwd)/..
DART_BIN=$(which dart)
DART_SDK_DIR=$(dirname $DART_BIN)/..
PLATFORM="$(uname -s)"
DART_VERSION=$(dart --version 2>&1)
LIBSASS_SRC=$PROJECT_ROOT/lib/src/libsass
case "$DART_VERSION" in
  (*32*)
    MACOS_ARCH="i386"
    LINUX_ARCH="32"
    ;;
  (*64*)
    MACOS_ARCH="x86_64"
    LINUX_ARCH="64"
    ;;
  (*)
    echo Unsupported dart architecture $DART_VERSION.  Exiting ... >&2
    exit 3
    ;;
esac

# see https://www.dartlang.org/articles/native-extensions-for-standalone-dart-vm/
cd $PROJECT_ROOT
pub install

# Check if libsass submodule exists, if not pull submodules
if [ ! -e "$LIBSASS_SRC/Readme.md" ]; then
  echo Pulling libsass submodule
  git submodule update --init --recursive
fi

# Compile libsass
if [ ! -e "$LIBSASS_SRC/lib/libsass.a" ]; then
  echo Compiling libsass
  cd $LIBSASS_SRC
  make -C libsass -j5 
fi

cd $PROJECT_ROOT/lib/src
echo Building libsass for platform $PLATFORM/$MACOS_ARCH
case "$PLATFORM" in
#  (Darwin)
#    g++ -fPIC -I $DART_SDK_DIR/include -c sync_socket_extension.cc -arch $MACOS_ARCH
#    gcc -shared -Wl,-install_name,libsync_socket_extension.dylib,-undefined,dynamic_lookup,-arch,$MACOS_ARCH -o \
#      ../libsync_socket_extension.dylib sync_socket_extension.o
#    ;;
  (Linux)
    g++ -fPIC -I$DART_SDK_DIR -Ilibsass/lib -c sass_extension.cc -lsass -m$LINUX_ARCH
    gcc -shared -fPIC -Wl,-soname,libsass_extension.so -o \
      ../libsass_extension.so sass_extension.o libsass/*.o -lc
    ;;
  (*)
    echo Unsupported platform $PLATFORM.  Exiting ... >&2
    exit 3
    ;;
esac

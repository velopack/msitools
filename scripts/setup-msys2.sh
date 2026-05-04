#!/bin/bash
set -e

# Setup MSYS2 environment for building wixl (both x86 and x64)
# Run this from any MSYS2 shell

pacman -Syu --noconfirm
pacman -S --noconfirm --needed \
  mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-meson \
  mingw-w64-ucrt-x86_64-pkgconf \
  mingw-w64-ucrt-x86_64-vala \
  mingw-w64-ucrt-x86_64-glib2 \
  mingw-w64-ucrt-x86_64-libxml2 \
  mingw-w64-i686-gcc \
  mingw-w64-i686-meson \
  mingw-w64-i686-pkgconf \
  mingw-w64-i686-glib2 \
  mingw-w64-i686-libxml2 \
  bison

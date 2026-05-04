#!/bin/bash
set -e

# Setup MSYS2 UCRT64 environment for building wixl
# Run this from an MSYS2 UCRT64 shell

pacman -Syu --noconfirm
pacman -S --noconfirm --needed \
  mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-meson \
  mingw-w64-ucrt-x86_64-pkg-config \
  mingw-w64-ucrt-x86_64-vala \
  mingw-w64-ucrt-x86_64-glib2 \
  mingw-w64-ucrt-x86_64-libgsf \
  mingw-w64-ucrt-x86_64-gcab \
  mingw-w64-ucrt-x86_64-libxml2 \
  bison

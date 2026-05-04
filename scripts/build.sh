#!/bin/bash
set -e

cd "$(dirname "$0")/.."

ARCH="${1:-both}"

build_arch() {
  local arch="$1"
  local prefix="$2"
  local builddir="build-${arch}"

  echo "=== Building ${arch} ==="
  # valac is only available in ucrt64, but it just generates C code (arch-agnostic)
  export PATH="/${prefix}/bin:/ucrt64/bin:/usr/bin"

  if [ ! -d "$builddir" ]; then
    meson setup "$builddir" --default-library=static --prefer-static
  else
    meson setup "$builddir" --reconfigure --default-library=static --prefer-static
  fi

  meson compile -C "$builddir"

  echo ""
  echo "Built: ${builddir}/wixl.exe"
  ldd "${builddir}/wixl.exe" 2>/dev/null | grep -v -i "windows/system32" || echo "(no non-system DLL dependencies)"
  echo ""
}

case "$ARCH" in
  x86)    build_arch x86 mingw32 ;;
  x64)    build_arch x64 ucrt64 ;;
  both)
    build_arch x64 ucrt64
    build_arch x86 mingw32
    ;;
  *)
    echo "Usage: $0 [x86|x64|both]"
    exit 1
    ;;
esac

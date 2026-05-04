#!/bin/bash
set -e

export PATH="/ucrt64/bin:$PATH"

# Build wixl from MSYS2 UCRT64 shell
cd "$(dirname "$0")/.."

if [ ! -d builddir ]; then
  meson setup builddir --default-library=static --prefer-static
else
  meson setup builddir --reconfigure --default-library=static --prefer-static
fi

meson compile -C builddir

echo ""
echo "Built: builddir/wixl.exe"
echo ""
echo "Checking dependencies:"
ldd builddir/wixl.exe 2>/dev/null | grep -v -i "windows/system32" || echo "(no non-system DLL dependencies)"

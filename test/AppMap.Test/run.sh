#!/bin/sh -e

cd "`dirname $0`"

export APPMAP_OUTPUT_DIR="`mktemp -td appmap.test.XXX`"
dotnet appmap test -c Release

./normalize.rb "$APPMAP_OUTPUT_DIR"
diff -urN expected "$APPMAP_OUTPUT_DIR"

rm -rf "$APPMAP_OUTPUT_DIR"

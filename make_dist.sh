#!/usr/bin/sh

# cd into repo root
SCRIPT_DIR=$(realpath $(dirname $0))
cd $SCRIPT_DIR

# copy all git tracked files
git ls-files | xargs -i dirname {} | xargs -i mkdir -p dist/$1/{}
git ls-files | xargs -i cp {} dist/$1/{}

cd dist

tar czf $1.tar.gz $1
rm -rf $1
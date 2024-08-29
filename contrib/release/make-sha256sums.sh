#!/usr/bin/env bash

export LC_ALL=C

set -euo pipefail

help_message() {
  echo "Output sha256sums from Gitian build output."
  echo "Usage: $0 AssetDirectory > sha256sums"
}

if [ "$#" -ne 1 ]; then
  echo "Error: Expects 1 argument: AssetDirectory"
  exit 1
fi

case $1 in
  -h|--help)
    help_message
    exit 0
    ;;
esac

# Trim off preceding whitespace that exists in the manifest
trim() {
  sed 's/^\s*//'
}

# Get the hash of the source tarball and output that first
grep -Eh "lambda-abc-[0-9.]+.tar.gz" "$1"/linux/lambda-abc-*linux-res.yml | trim

# Output hashes of all of the binaries
grep -Eh -- "lambda-abc-[0-9.]+.*-linux-.*.tar.gz" "$1"/linux/lambda-abc-*linux-res.yml | trim
grep -Eh -- "lambda-abc-[0-9.]+-win.*.(exe|tar.gz|zip)" "$1"/win/lambda-abc-*win-res.yml | trim
grep -Eh -- "lambda-abc-[0-9.]+-osx.*.(dmg|tar.gz)" "$1"/osx/lambda-abc-*osx-res.yml | trim

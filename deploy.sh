#!/bin/bash

EXE_NAME="pong"
RES_DIR="resources"
EXE_DIR="build"
DEPLOY_DIR="deploy"

if [ -d "$DEPLOY_DIR" ]; then
    rm -rf "$DEPLOY_DIR"
fi

if [ ! -f "$EXE_DIR/$EXE_NAME" ]; then
    echo "ERROR: executable not found."
    exit 1
fi

mkdir -p "$DEPLOY_DIR"
cp "$EXE_DIR/$EXE_NAME" "$DEPLOY_DIR/"

if [ -d "$RES_DIR" ]; then
    cp -r "$RES_DIR" "$DEPLOY_DIR/"
else
    echo "Folder \"$RES_DIR\" not found."
fi

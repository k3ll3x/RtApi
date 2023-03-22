#!/bin/bash
wasm-pack build --target web
cp -r shaders pkg/
cp web/index.html pkg/
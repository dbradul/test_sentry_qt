#!/bin/bash


BUILD_DIR=./build/Desktop_Qt_5_15_2_GCC_64bit-Release/
#BUILD_DIR=./build/Desktop_Qt_5_15_2_GCC_64bit-Debug/

objcopy --only-keep-debug --compress-debug-sections=zlib ${BUILD_DIR}/test_sentry_qt ${BUILD_DIR}/test_sentry_qt.debug
#sentry-cli login
#sentry-cli ... --auth-token <token>
sentry-cli debug-files upload -o <organization> -p <project> ${BUILD_DIR}/test_sentry_qt
sentry-cli debug-files upload -o <organization> -p <project> ${BUILD_DIR}/test_sentry_qt.debug

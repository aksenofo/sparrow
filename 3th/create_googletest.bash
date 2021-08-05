#!/bin/bash

rm -rf googletest-build

mkdir -p googletest-build
(cd googletest-build; cmake -DCMAKE_INSTALL_PREFIX=${PWD}/../googletest-release ../googletest .)
(cd googletest-build; make)
(cd googletest-build; make install)
rm -rf googletest-build

mkdir -p googletest-build
(cd googletest-build; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=${PWD}/../googletest-debug ../googletest .)
(cd googletest-build; make)
(cd googletest-build; make install)

rm -rf googletest-build

##########################################################################
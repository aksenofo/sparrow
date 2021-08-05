#!/bin/bash

rm -rf libev-build

mkdir libev-build

(cd libev; ./autogen.sh)

(cd libev-build; ../libev/configure --prefix=${PWD}/../libev-release)

(cd libev-build; make)

(cd libev-build; make install)

rm -rf libev-build

####################################################################################
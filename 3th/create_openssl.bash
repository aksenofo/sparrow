#!/bin/bash

OpenSSL=OpenSSL_1_1_1-stable

# Add perl template
sudo apt-get install libtext-template-perl

rm -rf openssl-build

####################################################################################
mkdir openssl-build

(cd openssl-build; ../$OpenSSL/config --prefix=${PWD}/../OpenSSL-release)
(cd openssl-build; make)
(cd openssl-build; make install)
rm -rf openssl-build

####################################################################################
mkdir openssl-build

(cd openssl-build; ../$OpenSSL/config --debug no-shared --prefix=${PWD}/../OpenSSL-debug)
(cd openssl-build; make)
(cd openssl-build; make install)

rm -rf openssl-build

####################################################################################
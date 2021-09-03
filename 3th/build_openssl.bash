#!/bin/bash

#       Author: Alexander Ksenofontov
#       License: MIT
#       All right reserved

OPENSSL=openssl

# Add perl template

echo =====================================================
echo Used Just add 'libtext-template-perl' if not exist
sudo apt-get install libtext-template-perl

rm -rf openssl-build

####################################################################################
mkdir openssl-build

(cd openssl-build; ../$OPENSSL/config --prefix=${PWD}/../openssl-release)
(cd openssl-build; make)
(cd openssl-build; make install)
rm -rf openssl-build

####################################################################################
mkdir openssl-build

(cd openssl-build; ../$OPENSSL/config --debug --prefix=${PWD}/../openssl-debug)
(cd openssl-build; make)
(cd openssl-build; make install)

rm -rf openssl-build

####################################################################################
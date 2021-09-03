#!/bin/bash
#       Author: Alexander Ksenofontov
#       License: MIT
#       All right reserved

openssl genrsa -des3 -passout pass:ABCD -out server.pass.key 2048
openssl rsa -passin pass:ABCD -in server.pass.key -out server.key
rm -f server.pass.key

openssl req -new -key server.key -out server.csr
openssl x509 -req -sha256 -days 365 -in server.csr -signkey server.key -out server.crt
rm -f server.csr

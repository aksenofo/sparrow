       Author: Alexander Ksenofontov
       License: MIT
       All right reserved
 
# Sparrow
You may use __openssl__ with __libev__ (minimalistic idea)  
See example folder.  
Project is unsing some socket wrapper only for simplify unitest and example as well.  
You may use your own wrapers for you projects.  

# Download

1. Use git to clone.
2. Load submodules  
       'git submodule add'  
       'git submodule init'  
       'git submodule update'  
       
3. Use cmake to build 

__You will get two libraries__  
libsparrow-base.a - basic classes  
libsparrow-ssl.a - ssl classes  

# Run example

for __server__  
./server -c ../../.rsa/server.crt -k ../../.rsa/server.key  
( see file example/main_server.cpp for detail)

for __client__  
./client

# Map

src/base - basic classes   
src/ssl - ssl wrappers  
utest/base - unitests for basic classes   
utest/ssl - unitests for ssl wrappers  

ftest/auxillary - auxillary classes for funtional tests (tcp,.. etc)  
ftest - funtional tests itself(client and server modules)  

example/auxillary - auxillary classes for example (tcp,.. etc)  
ftest - example itself(client and server modules)  

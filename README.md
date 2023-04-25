       Author: Alexander Ksenofontov
       License: MIT
       All right reserved
 
# Sparrow
You may use __openssl__ with __libev__ (minimalistic idea)  
See example folder.  
Project is using some socket wrapper only to simplify unitests and examples.  
You may use your own wrapers for your projects.  

# Download

1. Use git to clone.

2. Load submodules  
       'git submodule init'  
       'git submodule update'  

3. Build 3th libraries  
    a. cd 3th/  
    b. run __./build_openssl.bash__ ( ATT! you need to enter root password to install 'libtext-template-perl')  
       You may install it __outside__ script by running _sudo apt-get install libtext-template-perl_ and commenting appropriate string in script  
    c. run __./build_googletest.bash__ to build google test  
    d. run __./build_libev.bash__ to build libev  

5. Use cmake to build  
    a. run __mkdir__ _any_location_you_want_  
    b. run __cd__ _any_location_you_want_  
    c. run __cmake__ _absolute_or_relative_path_to_source_location_  
    d. run __make__  
    
__You will get two libraries__  
libsparrow-base.a - basic classes  
libsparrow-ssl.a - ssl classes  

# Run example

for __server__  
./server -c _absolute_or_relative_path_to_source_location_/.rsa/server.crt -k _absolute_or_relative_path_to_source_location_/.rsa/server.key  
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

example/auxillary - auxillary classes for samples (tcp,.. etc)  
ftest - example itself(client and server modules)  

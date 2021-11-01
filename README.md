       Author: Alexander Ksenofontov
       License: MIT
       All right reserved
 
# sparrow
You may use openssl with libev (minimalistic idea)  
See example forlder.  
Project is unsing some socket wrapper only for simplify unitest and example as well.  
You may use your own wrapers for you projects.  

# Downloading

1. Use git to clone.
2. Load submodules  
       'git submodule add'  
       'git submodule init'  
       'git submodule update'  
       
3. Use cmake to build 

You will get two libraries  
libsparrow-base.a - basic classes
libsparrow-ssl.a - ssl classes

# Map

src/base - basic classes   
src/ssl - ssl wrappers  
utest/base - unitests for basic classes   
utest/ssl - unitests for ssl wrappers  

ftest/auxillary - auxillary classes for funtional tests (tcp,.. etc)  
ftest - funtional tests itself(client and server modules)  

example/auxillary - auxillary classes for example (tcp,.. etc)  
ftest - example itself(client and server modules)  

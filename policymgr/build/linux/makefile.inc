#target
SUBTARGET=libpolicymgr


#define
ccpflag += 

#add include path
ccpflag += -I./include \
           -I../commonlib/include \


#library path
ccpflag += 

#library
ccpflag += 
           
SRC=$(wildcard  ./src/*.cpp)

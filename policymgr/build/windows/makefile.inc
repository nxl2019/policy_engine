
#target
SUBTARGET=policymgr

#DYNAMICCRT: yes/no
DYNAMICCRT=yes

#add include path
compilerflags += /I./include \
                 /I"$(BOOSTDIR)" \
                 /I$(OPENSSLDIR)/OpenSSL-1.0.1e-Win32/include \
                 /I../commonlib/include
           
SRC=$(wildcard  ./src/*.cpp)


#library path
ifeq ($(TARGETENVARCH), x64)
linkerflags += /LIBPATH:$(BOOSTDIR)/lib64-msvc-14.0 \
               /LIBPATH:$(OPENSSLDIR)/OpenSSL-1.0.1e-Win64/lib
else
linkerflags += /LIBPATH:$(BOOSTDIR)/lib32-msvc-14.0 \
               /LIBPATH:$(OPENSSLDIR)/OpenSSL-1.0.1e-Win32/lib
endif

linkerflags += /LIBPATH:../commonlib/win_$(TARGETENVARCH)_$(BUILDTYPE) \

compilerflags += /D_WIN32_WINNT=0x0501



#library
linkerflags += ssleay32.lib libeay32.lib commonlib.lib Shell32.lib

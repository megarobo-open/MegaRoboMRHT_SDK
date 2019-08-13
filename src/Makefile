## ARM Linux##
#CROSS_COMPILE=/opt/raspi/arm-rpi3-linux-gnueabihf/bin/arm-rpi3-linux-gnueabihf-

MajorVersion = 1
MinorVersion = 0.0
LIB_NAME = MegaGateway
LIB_SO_NAME = $(LIB_NAME).so
LIB_SO	= lib$(LIB_SO_NAME).$(MajorVersion).$(MinorVersion)
LIB_A	= lib$(LIB_NAME).a

CC	= $(CROSS_COMPILE)gcc
CXX	= $(CROSS_COMPILE)g++
AR	= $(CROSS_COMPILE)ar

CFLAGS	+= -g -Wall #-fPIC

INCLUDE = -I.
COBJS	= $(patsubst %.c, %.o, $(wildcard *.c))
CPPOBJS	= $(patsubst %.cpp, %.o, $(wildcard *.cpp))

OS=$(shell uname)

ifeq (,$(findstring MINGW,$(OS)))

COBJS	+= $(patsubst %.c, %.o, $(wildcard vxi11/*.c))
INCLUDE += -I./vxi11
CFLAGS += -D_VXI11_

ifneq (,$(findstring arm,$(CC)))
### ARM Linux
OUTPUTDIR=../lib/armlinux
else
### x86_linux
OUTPUTDIR = ../lib/linux
endif

else
#### MingW
LIBDIR	= ../win
INCLUDE += -I$(LIBDIR)

LIBS	+= $(LIBDIR)/libws2_32.a
LIBS	+= $(LIBDIR)/libiphlpapi.a
LIBS	+= $(LIBDIR)/libmsvcr100.a
LIBS	+= $(LIBDIR)/visa32.lib

OUTPUTDIR=../lib/windows

LIB_SO = $(LIB_NAME)-$(MajorVersion).dll
LIB_A	= lib$(LIB_NAME)-$(MajorVersion).a

endif

TARGET:$(LIB_SO) $(LIB_A)

$(LIB_SO):$(COBJS) $(CPPOBJS)
	$(CXX) $(CFLAGS) -shared -o $@ $^ $(INCLUDE) $(LIBS)

$(LIB_A):$(COBJS) $(CPPOBJS)
	$(AR) -r $@ $^ $(LIBS)

$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^ $(INCLUDE)

$(CPPOBJS): %.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $^ $(INCLUDE)


.PHONY:clean
clean:
	rm -rf $(LIB_SO) $(LIB_A) $(COBJS) $(CPPOBJS)

install:
	mkdir -p $(OUTPUTDIR)
	cp -rf $(LIB_A) $(OUTPUTDIR)

ifeq (,$(findstring MINGW,$(OS)))
	cp -rf $(LIB_SO) $(OUTPUTDIR)/lib$(LIB_SO_NAME)
else
	cp -rf $(LIB_SO) $(OUTPUTDIR)
endif


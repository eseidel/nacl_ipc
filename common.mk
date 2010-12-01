CC = $(NACL_SDK_ROOT)/bin/nacl-gcc
CPP = $(NACL_SDK_ROOT)/bin/nacl-g++
AR = $(NACL_SDK_ROOT)/bin/nacl-ar

LDR ?= $(NACL_SDK_ROOT)/bin/sel_ldr

CFLAGS = -Wall -Wno-long-long -pthread -DXP_UNIX -Werror -DUNIT_TEST -O2

TESTING_INCLUDES = -I$(PROJECT_ROOT)/testing/gtest \
                   -I$(PROJECT_ROOT)/testing/gtest/include \
                   -I$(PROJECT_ROOT)/testing/gmock/include \

INCLUDES = -I$(PROJECT_ROOT) \
           -I$(NACL_SDK_ROOT) \
           $(TESTING_INCLUDES) \

LDFLAGS = -lgoogle_nacl_imc \
          -lgoogle_nacl_npruntime \
          -lpthread \
          -lsrpc \
          -lnosys \
          $(ARCH_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -m32 $(INCLUDES) -c -o $@ $<

%.o: %.cc
	$(CPP) $(CFLAGS) -m32 $(INCLUDES) -c -o $@ $<

%.o: %.cpp
	$(CPP) $(CFLAGS) -m32 $(INCLUDES) -c -o $@ $<

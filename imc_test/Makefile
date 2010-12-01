.PHONY: all clean

OBJECTS = imc_test.o \

LDFLAGS = -lgoogle_nacl_imc \
          -lgoogle_nacl_npruntime \
          -lpthread \
          -lsrpc \
          -lnosys \

PROJECT_ROOT = ..
PROJECT_NAME = imc_test

all: $(PROJECT_NAME).nexe imc_test_host

include ../common.mk

HOST_LIB_PATH = /Projects/native_client/native_client/scons-out/dbg-mac-x86-32/lib

HOST_IMC_LIBARIES = -lgoogle_nacl_imc \
			        -lgoogle_nacl_imc_c \
			        -lnrd_xfer \
			        -lplatform \
			        -lgio

imc_test_host: imc_test_host.cc $(IMC_LIBARY)
	/usr/bin/g++ -m32 $< -o $@ -L$(HOST_LIB_PATH) $(HOST_IMC_LIBARIES)

$(PROJECT_NAME).nexe: $(OBJECTS)
	$(CPP) $^ $(LDFLAGS) -m32 -o $@

run: $(PROJECT_NAME).nexe imc_test.py imc_test_host
	python imc_test.py $(PROJECT_NAME).nexe

clean:
	$(RM) $(OBJECTS) $(PROJECT_NAME).nexe imc_test_host

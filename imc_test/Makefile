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

imc_test_host: imc_test_host.cc
	g++ $< -o $@

$(PROJECT_NAME).nexe: $(OBJECTS)
	$(CPP) $^ $(LDFLAGS) -m32 -o $@

run: $(PROJECT_NAME).nexe imc_test.py imc_test_host
	python imc_test.py $(PROJECT_NAME).nexe

clean:
	$(RM) $(OBJECTS) $(PROJECT_NAME).nexe

.PHONY: all clean

OBJECTS = imc_test.o \

LDFLAGS = -lgoogle_nacl_imc \
          -lgoogle_nacl_npruntime \
          -lpthread \
          -lsrpc \
          -lnosys \

PROJECT_ROOT = ..
PROJECT_NAME = imc_test

all: $(PROJECT_NAME).nexe

include ../common.mk

$(PROJECT_NAME).nexe: $(OBJECTS)
	$(CPP) $^ $(LDFLAGS) -m32 -o $@

run: $(PROJECT_NAME).nexe
	$(LDR) -- $(PROJECT_NAME).nexe

clean:
	$(RM) $(OBJECTS) $(PROJECT_NAME).nexe

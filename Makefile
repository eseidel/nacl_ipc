.PHONY: all clean

OBJECTS = hello_world.o \

LDFLAGS = -lgoogle_nacl_imc \
          -lgoogle_nacl_npruntime \
          -lpthread \
          -lsrpc \
          -lnosys \

PROJECT_ROOT = .
PROJECT_NAME = hello_world

all: $(PROJECT_NAME).nexe

include common.mk
include ipc/ipc.mk
include base/base.mk
include ppapi/ppapi.mk
include testing/testing.mk

$(PROJECT_NAME).nexe: $(OBJECTS) $(TEST_OBJECTS)
	$(CPP) $^ $(LDFLAGS) -m32 -o $@

run: $(PROJECT_NAME).nexe
	$(LDR) -- $(PROJECT_NAME).nexe

clean:
	$(RM) $(OBJECTS) $(TEST_OBJECTS) $(PROJECT_NAME).nexe

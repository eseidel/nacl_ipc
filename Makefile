# Copyright 2010, The Native Client SDK Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can
# be found in the LICENSE file.

# Makefile for the Hello World example.

.PHONY: all clean

MODULES = base \
          ipc \
          ppapi \
          testing \

LIBRARIES = $(MODULES:%=lib%.a)

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

libbase.a:
	cd base; make

libipc.a:
	cd ipc; make

libppapi.a:
	cd ppapi; make

libtesting.a:
	cd testing; make

$(PROJECT_NAME).nexe: $(OBJECTS) $(LIBRARIES)
	$(CPP) $^ $(LDFLAGS) -m32 -o $@ $(LIBRARIES)

run: $(PROJECT_NAME).nexe
	$(LDR) -- $(PROJECT_NAME).nexe

clean:
	$(RM) $(OBJECTS) $(PROJECT_NAME).nexe
	cd base; make clean
	cd ipc; make clean
	cd ppapi; make clean
	cd testing; make clean

all: $(PROJECT_ROOT)/lib$(LIBRARY_NAME).a

$(PROJECT_ROOT)/lib$(LIBRARY_NAME).a: $(OBJECTS) $(TEST_OBJECTS)
	$(AR) cr $@ $^ 

depend: $(OBJECTS:%.o=%.cc) $(TEST_OBJECTS:%.o=%.cc)
	rm -f ./.depend
	$(CC) $(CFLAGS) $(INCLUDES) -MM $^>>./.depend;

.depend: depend

include .depend

clean:
	$(RM) $(OBJECTS) $(TEST_OBJECTS) $(PROJECT_ROOT)/lib$(LIBRARY_NAME).a

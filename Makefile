CC := clang
INCLUDES := -Iinclude \
            -I../contrib/glad/include \
            -I../contrib/cglm/ \
            -I../contrib/stb
CFLAGS := -fPIC -g -std=c99 -pedantic \
          -Wall -Werror=implicit-function-declaration \
          $(INCLUDES)
LFLAGS := -shared -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
TESTFLAGS := -Lobj -lengine0

DYNAMIC_LIB := obj/libengine0.so
OBJS := obj/glad.o $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))

.PHONY: all dynamic-lib clean lint doc tests

dynamic-lib: $(DYNAMIC_LIB)

$(DYNAMIC_LIB): $(OBJS)
	$(CC) -o $(DYNAMIC_LIB) $^ $(CFLAGS) $(LFLAGS)

obj/%.o: src/%.c include/engine0/%.h
	@mkdir -p obj
	$(CC) $< -o $@ $(CFLAGS) -c

obj/glad.o: ../contrib/glad/src/glad.c
	@mkdir -p obj
	$(CC) $< -o $@ $(CFLAGS) -Wno-pedantic -c

lint:
	cppcheck --enable=all src/

clean:
	$(RM) -r obj/* doc/* tests/obj/*

doc:
	@mkdir -p doc
	doxygen doxygen.conf

# Testing
tests: $(patsubst tests/%.c, tests/obj/%, $(wildcard tests/*.c))

tests/obj/%: tests/%.c $(DYNAMIC_LIB)
	@mkdir -p tests/obj
	-$(CC) $< -o $@ -g -lm $(INCLUDES) $(realpath $(DYNAMIC_LIB))

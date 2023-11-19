TARGET:=brainfuck
CFLAGS:=-g -Wall -std=c17 -fsanitize=address
RFLAGS:=-std=c17 -DNDEBUG -O3

SRC:=$(wildcard src/*.c)
DOBJ:=$(patsubst src/%.c, obj/debug/%.o, $(SRC))
ROBJ:=$(patsubst src/%.c, obj/release/%.o, $(SRC))

-include dep.d

.DEFAULT_GOAL:=debug

.PHONY: debug release install clean rel deb bench


debug:
	mkdir -p obj/debug
	clang -MM $(SRC) | sed -r 's/^.*:.*$$/obj\/debug\/\0/' > dep.d
	$(MAKE) deb

release:
	mkdir -p obj/release
	clang -MM $(SRC) | sed -r 's/^.*:.*$$/obj\/release\/\0/' > dep.d
	$(MAKE) rel

bench: release FORCE
	mproc ./brainfuck ../speed-test.b < ../input > res
	diff ../expected res
	./brainfuck -T ../speed-test.b -o test.c
	$(CC) test.c -O3 -o test.out
	mproc ./test.out < ../input > res
	diff ../expected res

deb: $(DOBJ)
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LDFLAGS)

rel: $(ROBJ)
	$(CC) $(RFLAGS) $^ -o $(TARGET) $(LDFLAGS)

obj/release/%.o: src/%.c
	$(CC) $(RFLAGS) -c -o $@ $<

obj/debug/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

install:
	sudo cp -i $(TARGET) /usr/bin/$(TARGET)

uninstall:
	sudo rm -i /usr/bin/$(TARGET)

clean:
	-rm obj/debug/*.o obj/release/*.o $(TARGET) dep.d

FORCE:

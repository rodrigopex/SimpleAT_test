MAJOR=0
MINOR=2
BUILD=$(shell cat version.build)
CFLAGS+=-DMAJOR=$(MAJOR) -DMINOR=$(MINOR) -DBUILD=$(BUILD) -DDEBUG
SOURCES=$(filter-out main.c, $(wildcard *.c))

all: test

test: $(addprefix build/, $(SOURCES:.c=.o)) version
	$(CC) $(CFLAGS) $(filter-out version, $^) main.c -o build/$@

build/%.o: %.c | build
	$(COMPILE.c) -o $@ $<

build:
	mkdir -p $@

clean:
	$(RM) -r build/

version:
	@echo $$(($(BUILD)+1)) > version.build
	@echo Current firmware version $(MAJOR).$(MINOR).$(BUILD)

run: test
	@echo Running binary: build/test
	@build/test << test.cmd

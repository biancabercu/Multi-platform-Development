all: build

.PHONY: build
build: so-cpp.exe

so-cpp.exe: main.obj xreadwrite.obj
	cl /Feso-cpp.exe main.obj xreadwrite.obj

main.obj: main.c HashMap.h

.PHONY: clean

clean:
	del so-cpp.exe main.obj xreadwrite.obj

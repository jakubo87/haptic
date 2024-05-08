SHELL := /bin/bash

all: pndbg


pndbg: main.cc places.hh rules.hh current_input.hh tags.hh
	clang++ -std=c++23 -march=native -pthread -Wall -Wpedantic -fsanitize=address -g -O2 main.cc -o pn

pn: main.cc places.hh rules.hh current_input.hh tags.hh
	clang++ -std=c++23 -march=native -pthread -Wall -Wpedantic -ffast-math -O3 -flto main.cc -o pn	

.PHONY: run clean

run:
	./pn

clean:
	rm -rf pn

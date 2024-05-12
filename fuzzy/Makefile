SHELL := /bin/bash

all: pndbg


pndbg: main.cc places.hh rules.hh current_input.hh tags.hh threading.hh
	g++ -std=c++23 -march=native  -Wall -Wpedantic -fsanitize=address,undefined -fno-omit-frame-pointer -g -O0 -ltbb -pthread main.cc -o pn
	#when O0 then with -ltbb -pthread
	#clang++ does not work!

pn: main.cc places.hh rules.hh current_input.hh tags.hh threading.hh
	g++ -std=c++23 -march=native -Wall -ffast-math -O3 -flto main.cc -o pn	

pn2: main.cc places.hh rules.hh current_input.hh tags.hh threading.hh
	g++ -std=c++23 -march=native -Wall -ffast-math -O3 -flto -DNTH=2 main.cc -o pn	

.PHONY: run clean

run:
	./pn

clean:
	rm -rf pn

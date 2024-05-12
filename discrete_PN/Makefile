SHELL := /bin/bash

all: pn


pn: main.cc
	g++ -std=c++23 -march=native -Wall -O3 -flto main.cc -o pn

.PHONY: echo run clean

echo:
	echo "hello World"

run:
	./pn

fuzz: fuzzy.cc
	g++ -std=c++23 -march=native -Wall -O3 -flto fuzzy.cc -o foo

clean:
	rm -rf pn

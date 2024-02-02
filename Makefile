SHELL := /bin/bash

all: pn


pn: main.cc
	g++ -std=c++2b -march=native -Wall -O3 -flto main.cc -o pn

.PHONY: echo run clean

echo:
	echo "hello World"

run:
	./pn

clean:
	rm -rf pn

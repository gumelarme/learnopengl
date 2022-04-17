run: build
	./build/main

build: main.cpp
	clang++ -o ./build/main main.cpp glad.c -lGL -lglfw

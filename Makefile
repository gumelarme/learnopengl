run: build
	./build/main
build:
	clang++ main.cpp glad.c -lGL -lglfw -o build/main



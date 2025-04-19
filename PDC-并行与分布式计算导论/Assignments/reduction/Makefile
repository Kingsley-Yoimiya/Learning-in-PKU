all: build run

build:
	nvcc -o main.exe main.cu reduction.cu

run:
	./main.exe

clean:
	rm -f main.exe

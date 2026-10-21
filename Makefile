subosc: src/oscillator.c src/file_io.c src/main.c src/oscillator.h src/file_io.h
	gcc -Wall -Wextra -std=c11 src/oscillator.c src/file_io.c src/main.c -o subosc -lm

.PHONY: clean
clean: 
	rm -f subosc

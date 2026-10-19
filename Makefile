subosc: src/oscillator.c src/file_io.c src/main.c
	gcc -Wall -Wextra -std=c11 src/oscillator.c src/file_io.c src/main.c -o subosc -lm

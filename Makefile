c_files := src/oscillator.c src/file_io.c src/main.c src/miniaudio_impl.c
h_files := src/oscillator.h src/file_io.h third_party/miniaudio.h

subosc: $(c_files) $(h_files)
	gcc -Wall -Wextra -std=c11 $(c_files) $(h_files) -o subosc -lm

.PHONY: clean
clean: 
	rm -f subosc

c_files := src/oscillator.c src/file_io.c src/main.c src/miniaudio_impl.c src/synth.c src/envelope.c
h_files := src/oscillator.h src/file_io.h third_party/miniaudio.h src/synth.h src/envelope.h

subosc: $(c_files) $(h_files)
	gcc -Wall -Wextra -std=c11 $(c_files) -o subosc -lm -lpthread -ldl

.PHONY: clean
clean:
	rm -f subosc

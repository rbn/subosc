c_files := \
	src/oscillator.c \
	src/file_io.c \
	src/main.c \
	src/miniaudio_impl.c \
	src/synth.c \
	src/envelope.c

h_files := \
	src/oscillator.h \
	src/file_io.h third_party/miniaudio.h \
	src/synth.h \
	src/envelope.h \

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -fdiagnostics-color=always

subosc: $(c_files) $(h_files)
	$(CC) $(CFLAGS) -I/usr/local/include $(c_files) -o subosc -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
.PHONY: clean
clean:
	rm -f subosc

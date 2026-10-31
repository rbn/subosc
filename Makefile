c_files := \
	src/oscillator.c \
	src/file_io.c \
	src/main.c \
	src/miniaudio_impl.c \
	src/synth.c \
	src/envelope.c \
	src/scope.c \
	src/note.c \
	src/midi.c \
	src/event_queue.c \
	src/filter.c

h_files := \
	src/oscillator.h \
	src/file_io.h third_party/miniaudio.h \
	src/synth.h \
	src/envelope.h \
	src/scope.h \
	src/note.h \
	src/midi.h \
	src/event_queue.h \
	src/filter.h


CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L -fdiagnostics-color=always

subosc: $(c_files) $(h_files)
	$(CC) $(CFLAGS) -I/usr/local/include $(c_files) -o subosc -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lasound
.PHONY: clean
clean:
	rm -f subosc

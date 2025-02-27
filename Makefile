CC = cc 
CFLAGS =						\
	-Wall						\
	-Wextra						\
	-Werror						\
	-std=c99					\
	-O3
IFLAGS =						\
	-I./inc/
LFLAGS =						\
	-lGL						\
	-lGLX						\
	-lX11
SRCS =							\
	./src/zoom-window.c			\
	./src/zoom-render.c			\
	./src/zoom-draw.c			\
	./src/zoom-opengl.c			\
	./src/zoom-screenshot.c		\
	./src/zoom-math.c			\
	./src/zoom-input.c			\
	./src/zoom.c
OBJS = $(SRCS:.c=.o)
TARGET = zoom

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(IFLAGS)

.PHONY: install uninstall clean

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

install: $(TARGET)
	install $(TARGET) $(PREFIX)/bin

uninsall: 
	rm -f /usr/local/bin/$(TARGET)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

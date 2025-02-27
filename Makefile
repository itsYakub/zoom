CC = cc 
CFLAGS =						\
	-Wall						\
	-Wextra						\
	-Werror						\
	-std=c99					\
	-O3
LFLAGS =						\
	-lGL						\
	-lGLX						\
	-lX11
SRCS =							\
	./zoom-window.c				\
	./zoom-render.c				\
	./zoom-draw.c				\
	./zoom-opengl.c				\
	./zoom-screenshot.c			\
	./zoom-math.c				\
	./zoom-input.c				\
	./zoom.c
OBJS = $(SRCS:.c=.o)
TARGET = zoom

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

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

CC = cc 
CFLAGS =						\
	-Wall						\
	-Wextra						\
	-Werror						\
	-std=c99					\
	-g							\
	-O3
LFLAGS =						\
	-lGL						\
	-lGLX						\
	-lX11
SRCS =							\
	./xzoom-window.c			\
	./xzoom-render.c			\
	./xzoom-draw.c				\
	./xzoom-opengl.c			\
	./xzoom-screenshot.c		\
	./xzoom-math.c				\
	./xzoom-input.c				\
	./xzoom.c
OBJS = $(SRCS:.c=.o)
TARGET = xzoom

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

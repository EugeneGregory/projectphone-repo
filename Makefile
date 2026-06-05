CC = gcc
CFLAGS = $(shell pkg-config --cflags gtk4)
LIBS = $(shell pkg-config --libs gtk4)
TARGET = projectphone

# Пути к файлам репозитория ведут в подпапку src/
SRCS = src/main.c src/ui.c src/ui_ios.c src/ui_android.c src/ui_common.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

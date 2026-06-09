CC = gcc
CFLAGS = $(shell pkg-config --cflags gtk4) -Isrc -Isrc/core -Isrc/ui
LIBS = $(shell pkg-config --libs gtk4) -lsqlite3
TARGET = projectphone

# Beautiful and clean structural file tree
SRCS = src/main.c \
       src/ui/ui.c \
       src/ui/ui_usb_timer.c \
       src/ui/ui_dependencies.c \
       src/ui/ui_dependencies_row.c \
       src/ui/ui_dependencies_sections.c \
       src/ui/ui_dependencies_callbacks.c \
       src/core/dependencies.c \
       src/core/usb_detector.c \
       src/core/database.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

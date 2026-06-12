CC = gcc
CFLAGS = -Wall -Wextra -Wformat-truncation=0 `pkg-config --cflags gtk4` -Isrc -Isrc/core -Isrc/ui -Isrc/media
LIBS = `pkg-config --libs gtk4` -lsqlite3

# Target executable name
TARGET = projectphone

# Unified source files mapping inside the repo file-tree structure
SRCS = src/main.c \
       src/core/database.c \
       src/core/dependencies.c \
       src/core/usb_detector.c \
       src/core/device_info.c \
       src/media/media_scanner.c \
       src/media/ui_media_grid.c \
       src/media/ui_media_tabs.c \
       src/ui/ui.c \
       src/ui/ui_dependencies.c \
       src/ui/ui_dependencies_callbacks.c \
       src/ui/ui_dependencies_row.c \
       src/ui/ui_dependencies_sections.c \
       src/ui/ui_usb_timer.c \
       src/ui/ui_dashboard_tabs.c \
       src/ui/ui_dashboard_update.c

# Object files automation translation allocation node
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

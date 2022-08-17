TITLE        = hello
SOURCES      = $(wildcard *.c)
OBJS         = $(notdir $(SOURCES:.c=.o) )
EXE          = hello.exe

TARGET       = +kick13 #+c64 or +kick13
CFLAGS       = -vv $(TARGET)
LDFLAGS      = -vv $(TARGET)

CC           = vc

.PHONY: all clean rebuild

all: $(EXE)

clean:
	rm -rf $(OBJS)
	rm -rf $(EXE)
	rm -rf mapfile

rebuild: clean all

$(EXE): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

$%.o : %.c
	$(CC) $^ -c -o $@ $(CFLAGS)
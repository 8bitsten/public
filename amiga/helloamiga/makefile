TITLE        = hello
OUTPUT_DIR   = output
SOURCES      = $(wildcard *.c)
OBJS         = $(notdir $(SOURCES:.c=.o) )
EXE          = hello.exe

CFLAGS       = -vv +c64 #+c64 or +kick13
LDFLAGS      =  

CC           = vc

.PHONY: all clean rebuild

all: $(EXE)

clean:
	rm -rf $(OBJS)
	rm -rf $(EXE)
	rm -rf mapfile

rebuild: clean all

$(EXE): $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

$%.o : %.c
	$(CC) $^ -c -o $@ $(CFLAGS)
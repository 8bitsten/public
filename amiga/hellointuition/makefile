TITLE        = helloint
SOURCES      = $(wildcard *.c)
OBJS         = $(notdir $(SOURCES:.c=.o))
EXE          = $(TITLE).exe

TARGET       = +kick13 
CFLAGS       = -vv $(TARGET) -I"%VBCC%"/NDK_1_3/Includes1_3/include_h 
LDFLAGS      = -vv $(TARGET) -lamiga

CC           = vc

.PHONY: all clean rebuild

all: $(EXE)

clean:
	rm -rf $(OBJS)
	rm -rf $(EXE)

rebuild: clean all

$(EXE): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

$%.o : %.c
	$(CC) $^ -c -o $@ $(CFLAGS)
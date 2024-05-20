VERSION  =1.00
CC   =gcc
DEBUG   =-DUSE_DEBUG
CFLAGS = -Wall -std=gnu99 -fPIC -g

INCLUDES   =-I./inc -I../ -I./inc/8021x -I./inc/lldp
LIB_NAMES = -lm

SOURCE  := $(wildcard ./src/*.c) $(wildcard ./src/*.cpp) $(wildcard ./*.c) $(wildcard ./src/8021x/*.c) $(wildcard ./src/lldp/*.c)

OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

# TARGET  =libuoecom.a
TARGET  =app

.PHONY : clean

all:$(TARGET)
	@rm -rf $(OBJS)

#links
$(TARGET):$(OBJS)
# $(CC) -shared -o $@ $^ $(INCLUDES) $(LIB_NAMES)
	$(CC) -o $@ $^ $(INCLUDES) $(LIB_NAMES)

#compile
%.o: %.c
	$(CC) $(INCLUDES) $(DEBUG) -c $(CFLAGS) $< -o $@


clean:
	@echo "Remove linked and compiled files......"
	rm -rf $(OBJ) $(TARGET) ./src/8021x/*.o ./src/lldp/*.o ./*.o output out

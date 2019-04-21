CC = g++

CFLAGS = -pthread -Wall
INCLUDES = -I./lib/RakNet/Source -I./lib/RakNet/DependentExtensions
SRC = ./ragemp-udp/ragemp-udp.cpp ./lib/RakNet/Source/*.cpp

OUT_PATH = ./bin/
OUTPUT = ragemp-udp

all: app

app:
	mkdir $(OUT_PATH)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUT_PATH)/$(OUTPUT) $(SRC)

clean:
	rm $(OUT_PATH) -rf


INCLUDES 	:= -I./include/
CFLAGS		:= -std=c++11
SOURCES		:= $(wildcard ./src/*.cpp)
OBJECTS		:= $(patsubst %.cpp, %.o, $(SOURCES))

all:
	$(MAKE) -C ./src
	g++ $(INCLUDES) -c data.cpp -o data.o
	g++ $(OBJECTS) ./data.o -o data

clean:
	$(MAKE) clean -C ./src
	rm data.o 
	rm data
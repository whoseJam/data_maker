
SOURCES = $(wildcard ./*.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

all: $(OBJECTS)

$(OBJECTS): %.o : %.cpp
	g++ $(CFLAGS) -c $< -o $@

clean:
	rm *.o
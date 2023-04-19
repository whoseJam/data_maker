
SOURCES = $(wildcard ./*.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))
CFLAGS  = -std=c++11 -fPIC

all: $(OBJECTS)

$(OBJECTS): %.o : %.cpp
	g++ $(CFLAGS) -c $< -o ../build/$@

clean:
	rm ../build/*.o
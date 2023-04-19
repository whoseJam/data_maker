
INCLUDES 	:= -I./include/
SOURCES		:= $(wildcard ./src/*.cpp)
OBJECTS		:= $(patsubst %.cpp, %.o, $(SOURCES))
B_OBJECTS	:= $(subst src,build, $(OBJECTS))

lib:
	$(MAKE) -C ./src
	ar cr ./dist/libdata.a $(B_OBJECTS)

dmaker: ./dist/libdata.a
	g++ ./maker/$(SOURCE) -o data -L./dist/ -ldata

clean:
	$(MAKE) clean -C ./src
	rm data
COMPILER = g++
COMPILER_FLAGS = -c -g -O0 -Wall -w
EXEC=teapot_contest
LD = g++
LINKER = g++

teapot_contest : teapot_contest.o teapot.o  
	$(LD) teapot.o teapot_contest.o -lSOIL -L./usr/lib -lGL -lGLU -lglut -lGLEW  -o teapot_contest 

teapot.o : teapot.cpp teapot.h
	$(COMPILER) $(COMPILER_FLAGS) teapot.cpp

teapot_contest.o : teapot_contest.cpp teapot.h 
	$(COMPILER) $(COMPILER_FLAGS) teapot_contest.cpp 

clean:
	-rm -f *.o teapot_contest

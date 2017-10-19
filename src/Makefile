
object = random_16807.o Draw_Path.o SHarmonic.o

CC = g++
LLIB = -lm -lGL -lGLU -lglut

all : clean $(object)
	$(CC) -o SHarmonic $(LLIB) $(object)

draw : clean $(object) draw_sub
	$(CC) -o SHarmonic $(LLIB) $(object) 

draw_sub :  random_16807.h SHarmonic.cpp
	$(CC) -c SHarmonic.cpp -D DRAW

random_16807.o : random_16807.h random_16807.cpp
	$(CC) -c random_16807.cpp

Draw_Path.o : Draw_Path.h Draw_Path.cpp
	$(CC) -c Draw_Path.cpp

SHarmonic.o : random_16807.h SHarmonic.cpp
	$(CC) -c SHarmonic.cpp


.PHONY : clean

clean :
	rm *.o

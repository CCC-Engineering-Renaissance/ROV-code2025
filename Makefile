a.out: main.o thruster.o controller.o
	g++ main.o -llgpio -lSDL2

main.o: main.cpp
	g++ -c main.cpp

thruster.o: thruster.cpp
	g++ -c thruster.cpp

controller.o: controller.cpp
	g++ -c controller.cpp

clean:
	rm *.o

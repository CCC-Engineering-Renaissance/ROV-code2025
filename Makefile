a.out: out/main.o out/pca9685.o
	g++ out/main.o out/pca9685.o -llgpio -lSDL2

out/main.o: src/main.cpp
	g++ -o out/main.o -c -I ./include src/main.cpp -I /usr/include/eigen3

out/pca9685.o: include/pca9685.cpp include/pca9685.h
	g++ -o out/pca9685.o -c include/pca9685.cpp

out/thruster.o: include/thruster.cpp
	g++ -o out/pca9685.o -c include/thruster.cpp

out/controller.o: include/controller.cpp
	g++ -o out/controller.o -c include/controller.cpp

clean:
	rm *.o

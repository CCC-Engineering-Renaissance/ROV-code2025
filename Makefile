a.out: out/main.o out/pca9685.o
	g++ -g out/main.o out/pca9685.o -llgpio -lSDL2 -lpaho-mqtt3a -lpaho-mqttpp3 -lboost_system

out/main.o: src/main.cpp
	g++ -g -o out/main.o -c -I ./include src/main.cpp -I /usr/include/eigen3

out/pca9685.o: include/pca9685.cpp include/pca9685.h
	g++ -g -o out/pca9685.o -c include/pca9685.cpp

out/thruster.o: include/thruster.cpp
	g++ -g -o out/pca9685.o -c include/thruster.cpp

out/controller.o: include/controller.cpp
	g++ -g -o out/controller.o -c include/controller.cpp

#clean:
	rm *.o

clean:
	rm -f a.out core *.o

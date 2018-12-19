Bird-i : main.cpp
	g++ -o Bird-i main.cpp `pkg-config opencv --cflags --libs` -lwiringPi -lpthread
clean : 
	rm -rf *.o Bird-i

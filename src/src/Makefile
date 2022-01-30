CC = g++ -std=c++11

main: main.o ipmatchimp.o
	$(CC) main.o ipmatchimp.cpp -o solve

debug: main.o ipmatchimp.o 
	$(CC) -D DEBUG main.o ipmatchimp.cpp -o debug

test: test.cpp 
	$(CC) test.cpp -o test

main.o: main.cpp
	$(CC) main.cpp -c -o main.o
ipmatchimp.o: ipmatchimp.cpp ipmatch.h
	$(CC) ipmatchimp.cpp -c -o ipmatchimp.o

clean:
	rm solve *.o

cleandebug:
	rm debug *.o

cleantest:
	rm test
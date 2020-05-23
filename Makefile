all: neoranger

neoranger: main.o filesystem.o
	g++ -Wall -Wextra -g $^ -o $@ -lncurses

main.o: main.cpp filesystem.h
	g++ -Wall -Wextra -g -c main.cpp -o $@

filesystem.o: filesystem.cpp filesystem.h
	g++ -Wall -Wextra -g -c filesystem.cpp -o $@

clean:
	rm *.o neoranger

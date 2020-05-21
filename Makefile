all: neoranger

neoranger: main.o
	g++ -Wall -Wextra -g $^ -o $@ -lncurses

main.o: main.cpp
	g++ -Wall -Wextra -g -c main.cpp -o $@

clean:
	rm *.o neoranger

all: neoranger

neoranger: main.o filesystem.o region.o
	g++ -Wall -Wextra -g $^ -o $@ -lncurses

main.o: main.cpp filesystem.h
	g++ -Wall -Wextra -g -c main.cpp -o $@

filesystem.o: filesystem.cpp filesystem.h
	g++ -Wall -Wextra -g -c filesystem.cpp -o $@

region.o: region.cpp region.h filesystem.h
	g++ -Wall -Wextra -g -c region.cpp -o $@

clean:
	rm *.o neoranger

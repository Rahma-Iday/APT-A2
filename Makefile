.default: all

all: ppd

clean:
	rm -rf ppd *.o *.dSYM

ppd: Coin.o Node.o LinkedList.o DoublyLinkedList.o ppd.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c $^

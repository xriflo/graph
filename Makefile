all:
	g++ -std=c++11 -I . tests.cpp -o tests
clean:
	rm -f tests
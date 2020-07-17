all: q1 q2 

q1: q1.cpp	
	g++ -Wreturn-type -Wc++11-extensions -std=c++1y -o q1 q1.cpp 
q2: q2.cpp
	g++ -Wreturn-type -Wc++11-extensions -std=c++1y -o q2 q2.cpp 
clean:
	rm -f q1 q2 *.o

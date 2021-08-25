CXX=g++ -std=c++17
OPT=-O3

KSP: main.o test.o graph.o tools.o H2H.o eKSP.o DAPrune.o DA.o
	$(CXX) -g -o KSP main.o graph.o tools.o H2H.o eKSP.o DAPrune.o DA.o -lboost_system -lboost_thread -lpthread
	$(CXX) -g -o test test.o graph.o tools.o H2H.o -lboost_system -lboost_thread -lpthread

test: test.o graph.o tools.o H2H.o
	$(CXX) -g -o test test.o graph.o tools.o H2H.o -lboost_system -lboost_thread -lpthread

graph.o:graph.cpp
	$(CXX) -g -c $(OPT) graph.cpp
main.o:main.cpp
	$(CXX) -g -c $(OPT) main.cpp
test.o:test.cpp
	$(CXX) -g -c $(OPT) test.cpp 
tools.o:tools.cpp
	$(CXX) -g -c $(OPT) tools.cpp
H2H.o:H2H.cpp
	$(CXX) -g -c $(OPT) H2H.cpp
eKSP.o:eKSP.cpp
	$(CXX) -g -c $(OPT) eKSP.cpp	
DA.o:DA.cpp
	$(CXX) -g -c $(OPT) DA.cpp
DAPrune.o:DAPrune.cpp
	$(CXX) -g -c $(OPT) DAPrune.cpp

clean:
	rm *.o
	rm KSP
	rm test

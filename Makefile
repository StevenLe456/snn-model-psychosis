CXX = g++

BIN_DIR = bin

create_data: create_data.o
	$(CXX) -std=c++0x create_data.0 -o $(BIN_DIR)/create_data -lpthread

create_data.o: create_data.cpp
	$(CXX) -std=c++0x -c create_data.cpp
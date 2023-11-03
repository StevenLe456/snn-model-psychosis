CXX=g++
CXX_FLAGS=-std=c++0x
OBJ_DIR=obj
BIN_DIR=bin

create_data: create_data.o
	$(CXX) $(CXX_FLAGS) -lpthread $(OBJ_DIR)/create_data.o -o $(BIN_DIR)/create_data

create_data.o: create_data.cpp
	$(CXX) $(CXX_FLAGS) -I. -Icereal/ -O2 -c -pthread create_data.cpp -o $(OBJ_DIR)/create_data.o

setup:
	mkdir $(BIN_DIR)
	mkdir $(OBJ_DIR)
	mkdir data
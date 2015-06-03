SRC_C = app/controller
SRC_M = app/models
SRC_T = tmpl_build
C_H = $(SRC_C)/headers
M_H = $(SRC_M)/headers
INC = -I$(C_H) -I$(M_H)
OBJ_DIR = objects
FLAGS = -std=c++11 -Wall -lcppcms -lbooster -lpqxx -lpq

all: main run

main: $(OBJ_DIR)/main.o $(OBJ_DIR)/generic.o $(OBJ_DIR)/view.o $(OBJ_DIR)/database.o $(OBJ_DIR)/user.o $(OBJ_DIR)/layout.o
	@echo
	# Linking...
	@echo
	g++ $(INC) $(FLAGS) \
		objects/main.o objects/generic.o objects/view.o objects/database.o objects/user.o objects/layout.o -o main

$(OBJ_DIR)/main.o: main.cpp $(C_H)/generic.hpp
	@echo
	# main.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c main.cpp -o objects/main.o

$(OBJ_DIR)/generic.o: $(SRC_C)/generic.cpp $(C_H)/view.hpp $(M_H)/user.hpp $(C_H)/database.hpp
	@echo
	# Generic.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c $(SRC_C)/generic.cpp -o objects/generic.o

$(OBJ_DIR)/view.o: $(SRC_C)/view.cpp
	@echo
	# View.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c $(SRC_C)/view.cpp -o objects/view.o

$(OBJ_DIR)/database.o: $(SRC_C)/database.cpp
	@echo
	# Database.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c $(SRC_C)/database.cpp -o objects/database.o

$(OBJ_DIR)/user.o: $(SRC_M)/user.cpp
	@echo
	# User.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c $(SRC_M)/user.cpp -o objects/user.o

$(OBJ_DIR)/layout.o: $(SRC_T)/layout.cpp
	@echo
	# Layout.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c $(SRC_T)/layout.cpp -o objects/layout.o

run:
	./main -c config/config.js

clean:
	rm main objects/*
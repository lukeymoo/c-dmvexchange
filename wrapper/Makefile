SRC_C = app/controller
CTRL_CPP = $(SRC_C)/generic.cpp $(SRC_C)/view.cpp $(SRC_C)/database.cpp $(SRC_C)/base64.cpp
CTRL_HPP = $(C_H)/generic.hpp $(C_H)/view.hpp $(C_H)/database.hpp $(C_H)/base64.hpp
SRC_M = app/models
SRC_T = tmpl_build
C_H = $(SRC_C)/headers
M_H = $(SRC_M)/headers
INC = -I$(C_H) -I$(M_H)
OBJ_DIR = objects
OBJ = $(OBJ_DIR)/main.o $(OBJ_DIR)/generic.o $(OBJ_DIR)/view.o $(OBJ_DIR)/database.o
FLAGS = -std=c++11 -Wall -lcppcms -lbooster -lboost_regex -lpqxx -lpq

all: main

main: $(OBJ_DIR)/main.o $(OBJ_DIR)/generic.o $(OBJ_DIR)/view.o $(OBJ_DIR)/database.o $(OBJ_DIR)/base64.o $(OBJ_DIR)/user.o $(OBJ_DIR)/layout.o
	@echo
	# Linking...
	@echo
	g++ $(INC) $(FLAGS) \
		objects/main.o objects/generic.o objects/view.o objects/database.o objects/base64.o objects/user.o objects/layout.o -o main

$(OBJ_DIR)/main.o: main.cpp $(SRC_C)/generic.cpp $(C_H)/generic.hpp
	@echo
	# main.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c main.cpp -o objects/main.o

$(OBJ_DIR)/generic.o: $(SRC_C)/database.cpp $(C_H)/database.hpp $(SRC_M)/user.cpp $(M_H)/user.hpp $(SRC_C)/view.cpp $(C_H)/view.hpp $(SRC_C)/generic.cpp $(C_H)/generic.hpp
	@echo
	# Generic.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c $(SRC_C)/generic.cpp -o objects/generic.o

$(OBJ_DIR)/view.o: $(SRC_C)/view.cpp $(C_H)/view.hpp
	@echo
	# View.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c $(SRC_C)/view.cpp -o objects/view.o

$(OBJ_DIR)/database.o: $(SRC_C)/database.cpp $(C_H)/database.hpp
	@echo
	# Database.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c $(SRC_C)/database.cpp -o objects/database.o

$(OBJ_DIR)/base64.o:
	@echo
	# Base64.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c $(SRC_C)/base64.cpp -o objects/base64.o

$(OBJ_DIR)/user.o: $(SRC_M)/user.cpp $(M_H)/user.hpp
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
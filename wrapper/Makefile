SRC_C = app/controller
SRC_M = app/models
SRC_T = tmpl_build

C_H = $(SRC_C)/headers
M_H = $(SRC_M)/headers

OBJ_DIR = objects

CTRL_CPP = $(SRC_C)/controller.cpp $(SRC_C)/view.cpp $(SRC_C)/methods.cpp $(SRC_C)/base64.cpp
CTRL_HPP = $(C_H)/controller.hpp $(C_H)/view.hpp $(C_H)/methods.hpp $(C_H)/base64.hpp
MDL_CPP = $(SRC_M)/user.cpp
MDL_HPP = $(M_H)/user.hpp
INC = -I$(C_H) -I$(M_H)
OBJ = $(OBJ_DIR)/main.o $(OBJ_DIR)/controller.o $(OBJ_DIR)/view.o $(OBJ_DIR)/methods.o $(OBJ_DIR)/user.o
FLAGS = -std=c++11 -Wall -lcppcms -lbooster -lboost_regex -lpqxx -lpq -lcrypto

all: main

main: $(OBJ_DIR)/main.o $(OBJ_DIR)/controller.o $(OBJ_DIR)/view.o $(OBJ_DIR)/methods.o $(OBJ_DIR)/base64.o $(OBJ_DIR)/user.o $(OBJ_DIR)/layout.o
	@echo
	# Linking...
	@echo
	g++ objects/main.o objects/controller.o objects/view.o objects/methods.o objects/base64.o objects/user.o objects/layout.o -o main \
		$(INC) $(FLAGS)

$(OBJ_DIR)/main.o: main.cpp $(SRC_C)/controller.cpp $(C_H)/controller.hpp
	@echo
	# main.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c main.cpp -o objects/main.o

$(OBJ_DIR)/controller.o: $(SRC_C)/methods.cpp $(C_H)/methods.hpp $(SRC_M)/user.cpp $(M_H)/user.hpp $(SRC_C)/view.cpp $(C_H)/view.hpp $(SRC_C)/controller.cpp $(C_H)/controller.hpp
	@echo
	# controller.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c $(SRC_C)/controller.cpp -o objects/controller.o

$(OBJ_DIR)/view.o: $(SRC_C)/view.cpp $(C_H)/view.hpp
	@echo
	# View.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c $(SRC_C)/view.cpp -o objects/view.o

$(OBJ_DIR)/methods.o: $(SRC_C)/methods.cpp $(C_H)/methods.hpp
	@echo
	# methods.cpp
	@echo
	g++ $(INC) $(FLAGS) \
		-c $(SRC_C)/methods.cpp -o objects/methods.o

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

$(SRC_T)/layout.cpp: app/tmpl_src/layout/layout.tmpl
	@echo
	# Generating html from .tmpl
	@echo
	cppcms_tmpl_cc app/tmpl_src/layout/layout.tmpl -o tmpl_build/layout.cpp

run:
	./main -c config/config.js

clean:
	rm main objects/*
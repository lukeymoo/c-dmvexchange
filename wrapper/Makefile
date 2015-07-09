SRC_C = app/controller
SRC_M = app/models
SRC_T = tmpl_build

C_H = $(SRC_C)/headers
M_H = $(SRC_M)/headers

OBJ_DIR = objects

CTRL_CPP = $(SRC_C)/controller.cpp $(SRC_C)/view.cpp $(SRC_C)/methods.cpp $(SRC_C)/api.cpp $(SRC_C)/base64.cpp
CTRL_HPP = $(C_H)/controller.hpp $(C_H)/view.hpp $(C_H)/methods.hpp $(C_H)/api.hpp $(C_H)/base64.hpp
MDL_CPP = $(SRC_M)/user.cpp $(SRC_M)/product.cpp $(SRC_M)/comment.hpp
MDL_HPP = $(M_H)/user.hpp $(M_H)/product.hpp $(M_H)/comment.hpp
INC = -I$(C_H) -I$(M_H)
OBJ = $(OBJ_DIR)/main.o $(OBJ_DIR)/controller.o $(OBJ_DIR)/view.o $(OBJ_DIR)/methods.o $(OBJ_DIR)/user.o $(OBJ_DIR)/product.o $(OBJ_DIR)/comment.o $(OBJ_DIR)/api.o $(OBJ_DIR)/base64.o $(OBJ_DIR)/layout.o
FLAGS = -std=c++11 -Wall -lcppcms -lbooster -lboost_regex -lpqxx -lpq -lcrypto

all: main

main: $(OBJ)
	@echo
	# Linking...
	@echo
	g++ $^ -o main \
		$(INC) $(FLAGS)

$(OBJ_DIR)/main.o: main.cpp $(SRC_C)/api.cpp $(C_H)/api.hpp
	@echo
	# main.cpp
	@echo
	g++ -c $< -o $@ \
		$(INC) $(FLAGS)

$(OBJ_DIR)/controller.o: $(SRC_C)/controller.cpp $(C_H)/controller.hpp
	@echo
	# controller.cpp
	@echo
	g++ -c $< -o $@ \
		$(INC) $(FLAGS)

$(OBJ_DIR)/view.o: $(SRC_C)/view.cpp $(C_H)/view.hpp
	@echo
	# View.cpp
	@echo
	g++ -c $< -o $@ \
		$(INC) $(FLAGS)

$(OBJ_DIR)/methods.o: $(SRC_C)/methods.cpp $(C_H)/methods.hpp
	@echo
	# methods.cpp
	@echo
	g++ -c $< -o $@ \
		$(INC) $(FLAGS)

$(OBJ_DIR)/api.o: $(SRC_C)/api.cpp $(C_H)/api.hpp
	@echo
	# api.cpp
	@echo
	g++ -c $< -o $@ \
		$(INC) $(FLAGS)

$(OBJ_DIR)/base64.o: $(SRC_C)/base64.cpp $(C_H)/base64.hpp
	@echo
	# Base64.cpp
	@echo
	g++ -c $< -o $@ \
		$(INC) $(FLAGS)

$(OBJ_DIR)/user.o: $(SRC_M)/user.cpp $(M_H)/user.hpp
	@echo
	# User.cpp
	@echo
	g++ -c $< -o $@ \
		$(INC) $(FLAGS)

$(OBJ_DIR)/product.o: $(SRC_M)/product.cpp $(M_H)/product.hpp
	@echo
	# Product.cpp
	@echo
	g++ -c $< -o $@ \
		$(INC) $(FLAGS)

$(OBJ_DIR)/comment.o: $(SRC_M)/comment.cpp $(M_H)/comment.hpp
	@echo
	# Comment.cpp
	@echo
	g++ -c $< -o $@ \
		$(INC) $(FLAGS)

$(OBJ_DIR)/layout.o: $(SRC_T)/layout.cpp
	@echo
	# Layout.cpp
	@echo
	g++ -c $< -o $@ \
		$(INC) $(FLAGS)

$(SRC_T)/layout.cpp: app/tmpl_src/layout/layout.tmpl
	@echo
	# Generating html from .tmpl
	@echo
	cppcms_tmpl_cc app/tmpl_src/layout/layout.tmpl -o tmpl_build/layout.cpp

run:
	./main -c config/config.js

clean:
	rm main objects/*
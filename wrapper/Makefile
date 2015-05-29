CC 					= g++
SRC_DIR 			= app/controller
SRC_CONTROLLER 		= $(SRC_DIR)/generic.cpp $(SRC_DIR)/view.cpp
SRC_MODELS 			= $(SRC_DIR)/models/user/user.cpp
PRE_SRC_TMPL		= app/tmpl_src/layout/layout.tmpl
TMPL_DEST 			= tmpl_build/layout.cpp
BUILD_TMPL_DIR 		= tmpl_build
PARAMS 				= -Wall -lcppcms -lbooster
INCLUDES 			= -Iapp/controller/headers -Iapp/models/headers
OBJ					= objects/main.o objects/generic.o objects/view.o objects/static.o objects/layout.o  


all:
	# Generate template source
	cppcms_tmpl_cc $(PRE_SRC_TMPL) -o $(TMPL_DEST)
	#
	#
	# Compile
	$(CC) -std=c++11 main.cpp $(SRC_CONTROLLER) $(TMPL_DEST) $(PARAMS) $(INCLUDES) -o main

run:
	# Now running...
	./main -c config/config.js

clean:
	rm main objects/*
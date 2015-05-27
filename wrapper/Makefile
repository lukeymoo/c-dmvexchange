CC 					= g++
SRC_DIR 			= app
SRC_CONTROLLER 		= $(SRC_DIR)/controller/generic/generic.cpp tmpl_build/layout.cpp $(SRC_DIR)/controller/view/view.cpp
SRC_MODELS 			= $(SRC_DIR)/models/user/user.cpp
SRC_TMPL			= app/tmpl_src/layout/layout.tmpl
BUILD_TMPL_DIR 		= tmpl_build
BUILD_TMPL_MASTER 	= $(BUILD_TMPL_DIR)/layout.cpp
PARAMS 				= -Wall -O3 -lcppcms -lbooster
INCLUDES 			= -Iapp/controller/generic -Iapp/controller/view -Iapp/models/user


all:
	# Generate template source
	cppcms_tmpl_cc $(SRC_TMPL) -o $(BUILD_TMPL_MASTER)
	# Compile & Link
	$(CC) main.cpp app/controller/generic/generic.cpp app/controller/view/view.cpp tmpl_build/layout.cpp -o main $(PARAMS) $(INCLUDES)

run:
	# Now running...
	./main -c config/config.js

clean:
	rm main
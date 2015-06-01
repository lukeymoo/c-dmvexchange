CC 					= g++
SRC_DIR 			= app/controller
SRC_CONTROLLER 		= $(SRC_DIR)/generic.cpp $(SRC_DIR)/view.cpp $(SRC_DIR)/database.cpp
SRC_MODELS 			= $(SRC_DIR)/models/user/user.cpp
PRE_SRC_TMPL		= app/tmpl_src/layout/layout.tmpl
TMPL_DEST 			= tmpl_build/layout.cpp
BUILD_TMPL_DIR 		= tmpl_build
PARAMS 				= -Wall -lcppcms -lbooster -lpqxx -lpq
INCLUDES 			= -Iapp/controller/headers -Iapp/models/headers
OBJ					= objects/main.o objects/generic.o objects/view.o objects/static.o objects/layout.o  


all:
	# TIP FROM QUORA USER
	# ---------------------
	# 1. Get a list of potential customers
	# 2. Develop a message
	# 3. Create a catalogue
	# 4. Start calling, emailing
	# 5. Get personal visits
	# 6. Sell, sell, sell
	# 7. Talk to as many customers as you can . Make your presentation and listen to them to see what they need
	# 8. Tweak your speech with the following customers until you get it right.
	# 
	@echo
	# Generating template source
	@echo
	cppcms_tmpl_cc $(PRE_SRC_TMPL) -o $(TMPL_DEST)
	@echo
	# Compiling...
	@echo
	$(CC) -std=c++11 main.cpp $(SRC_CONTROLLER) $(TMPL_DEST) \
		$(PARAMS) $(INCLUDES) -o main
	@echo
	 
	 

run:
	# Now running...
	./main -c config/config.js

clean:
	rm main objects/*
CXX           	= g++-7
#CXXFLAGS        = -pg -std=c++11 -Wall -Wfatal-errors -Wextra -I/home/feng/include -Iinclude -O0 -DDEBUG
#LINK          	= $(CXX) -pthread -lmongoclient -lboost_thread -lboost_filesystem -lboost_program_options -lboost_system -pg
CXXFLAGS        = -mtune=corei7 -march=corei7 -Ofast -mfpmath=sse -mmmx -msse -msse2 -msse3 -mssse3 -msse4.1 -msse4.2 -msse4 -pipe -std=c++11 -Wall -Wfatal-errors -Wextra -I/home/feng/include -Iinclude -I/usr/local/include
LINK          	= $(CXX) -pthread -lmongoclient -lboost_thread -lboost_filesystem -lboost_program_options -lboost_system

####### Output directory
OBJECTS_DIR   = ./obj
BIN_DIR       = ./bin

prediction_test: test/prediction_test.cc
	$(CXX) $(CXXFLAGS) -o $(OBJECTS_DIR)/prediction_test.o -c test/prediction_test.cc
	$(LINK) -o $(BIN_DIR)/prediction_test $(OBJECTS_DIR)/prediction_test.o

rmg_test: test/rmg_test.cc
	$(CXX) $(CXXFLAGS) -o $(OBJECTS_DIR)/rmg_test.o -c test/rmg_test.cc
	$(LINK) -o $(BIN_DIR)/rmg_test $(OBJECTS_DIR)/rmg_test.o

mrm_test: test/mrm_test.cc
	$(CXX) $(CXXFLAGS) -o $(OBJECTS_DIR)/mrm_test.o -c test/mrm_test.cc
	$(LINK) -o $(BIN_DIR)/mrm_test $(OBJECTS_DIR)/mrm_test.o

ubf_test: test/ubf_test.cc
	$(CXX) $(CXXFLAGS) -o $(OBJECTS_DIR)/ubf_test.o -c test/ubf_test.cc
	$(LINK) -o $(BIN_DIR)/ubf_test $(OBJECTS_DIR)/ubf_test.o

lc_test: test/lc_test.cc
	$(CXX) $(CXXFLAGS) -o $(OBJECTS_DIR)/lc_test.o -c test/lc_test.cc
	$(LINK) -o $(BIN_DIR)/lc_test $(OBJECTS_DIR)/lc_test.o

item_test: test/item_test.cc
	$(CXX) $(CXXFLAGS) -o $(OBJECTS_DIR)/item_test.o -c test/item_test.cc
	$(LINK) -o $(BIN_DIR)/item_test $(OBJECTS_DIR)/item_test.o

user_test: test/user_test.cc
	$(CXX) $(CXXFLAGS) -o $(OBJECTS_DIR)/user_test.o -c test/user_test.cc
	$(LINK) -o $(BIN_DIR)/user_test $(OBJECTS_DIR)/user_test.o

rating_test: test/rating_test.cc
	$(CXX) $(CXXFLAGS) -o $(OBJECTS_DIR)/rating_test.o -c test/rating_test.cc
	$(LINK) -o $(BIN_DIR)/rating_test $(OBJECTS_DIR)/rating_test.o

left_matrix_update_test: test/left_matrix_update_test.cc
	$(CXX) $(CXXFLAGS) -o $(OBJECTS_DIR)/left_matrix_update_test.o -c test/left_matrix_update_test.cc
	$(LINK) -o $(BIN_DIR)/left_matrix_update_test $(OBJECTS_DIR)/left_matrix_update_test.o

insert_movelens_100k: test/insert_movelens_100k.cc
	$(CXX) $(CXXFLAGS) -o $(OBJECTS_DIR)/insert_movelens_100k.o -c test/insert_movelens_100k.cc
	$(LINK) -o $(BIN_DIR)/insert_movelens_100k $(OBJECTS_DIR)/insert_movelens_100k.o

ubf_movelens_100k: test/ubf_movelens_100k.cc
	$(CXX) $(CXXFLAGS) -o $(OBJECTS_DIR)/ubf_movelens_100k.o -c test/ubf_movelens_100k.cc
	$(LINK) -o $(BIN_DIR)/ubf_movelens_100k $(OBJECTS_DIR)/ubf_movelens_100k.o

web_main: web/main.cc
	$(CXX) $(CXXFLAGS) -o $(OBJECTS_DIR)/web_main.o -c web/main.cc
	$(CXX) -o $(BIN_DIR)/web_main $(OBJECTS_DIR)/web_main.o -lwthttp -lwt
	./$(BIN_DIR)/web_main --docroot . --http-address 0.0.0.0 --http-port 9090


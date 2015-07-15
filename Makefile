TARGET_FOLDER := bin
TARGET := $(TARGET_FOLDER)/trace
SOURCES_FOLDER := src
HEADERS_FOLDER := $(SOURCES_FOLDER)
OBJECTS_FOLDER := obj
SOURCES := $(wildcard $(SOURCES_FOLDER)/*.cpp)
HEADERS := $(wildcard $(SOURCES_FOLDER)/*.hpp)
OBJECTS := $(addprefix $(OBJECTS_FOLDER)/, $(notdir $(SOURCES:%.cpp=%.o)))
CXX_FLAGS := -g -ggdb -std=c++11 -I./$(HEADERS_FOLDER)/
all: $(TARGET)
$(TARGET): Makefile $(HEADERS) $(OBJECTS)
	@mkdir -p $(TARGET_FOLDER)
	g++ -o $(TARGET) $(OBJECTS) -lrt
#	@sudo setcap cap_net_raw=+ep $(TARGET)
$(OBJECTS_FOLDER)/%.o: $(SOURCES_FOLDER)/%.cpp $(HEADERS_FOLDER)/%.hpp
	@mkdir -p $(OBJECTS_FOLDER)
	g++ $(CXX_FLAGS) -o $@ -c $<
$(OBJECTS_FOLDER)/%.o: $(SOURCES_FOLDER)/%.cpp
	@mkdir -p $(OBJECTS_FOLDER)
	g++ $(CXX_FLAGS) -o $@ -c $<
clean:
	rm -rf $(TARGET_FOLDER) $(OBJECTS_FOLDER)

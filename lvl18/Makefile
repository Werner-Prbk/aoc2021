TARGET := main

CXX := g++-9
CXXFLAGS := -std=c++2a -Wall -Wextra

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS += -g
else
    CXXFLAGS += -o1
endif

INCLUDES = -I../common

srcfiles := $(shell find . -name "*.cpp")
srcfiles += $(shell find ../common -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(TARGET)

$(TARGET): $(objects)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $(TARGET) $(objects) $(LDLIBS)

clean:
	rm -f $(objects)
	rm -r $(TARGET)

run:
	./$(TARGET)

TARGET := main

CXX := g++-9
CXXFLAGS := -std=c++2a -Wall -Wextra -Werror

srcfiles := $(shell find . -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(TARGET)

$(TARGET): $(objects)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(objects) $(LDLIBS)

clean:
	rm -f $(objects)
	rm -r $(TARGET)

run:
	./$(TARGET)

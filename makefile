Exec = SortedListTest
OPTS = -O3 -march=native -std=c++11 -g

objects := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
objects += $(patsubst ../Board/%.cpp,../Board/%.o,$(wildcard ../Board/*.cpp))


.PHONY: all
all: $(Exec)

.PHONY: check
check:
	echo Objects are $(objects)

$(objects): %.o: %.cpp *.h
	$(CXX) -c $(OPTS) $< -o $@
	

$(Exec): $(objects)
	$(CXX) $(OPTS) $(objects) -o $(Exec)

.PHONY: clean
clean:
	-rm *.o $(Exec)
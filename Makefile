CC         = g++
CFLAGS     = -std=c++14 -c -O2 -Wall -Wextra
LFLAGS     = -std=c++14 -O2 -Wall -Wextra
SRC_DIR    = src
OBJ_DIR    = obj
SOURCES    = src/main.cpp \
             src/laby.cpp
OBJECTS    = $(SOURCES:src/%.cpp=obj/%.o)
EXECUTABLE = laby

all: $(EXECUTABLE)

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(@:obj/%.o=src/%.cpp) -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

.PHONY: clean
clean:
	rm -rfv $(OBJECTS) $(EXECUTABLE)

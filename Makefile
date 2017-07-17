CC         = g++
CFLAGS     = -std=c++14 -c -O2 -Wall -Wextra
LFLAGS     = -std=c++14 -O2 -Wall -Wextra
SRCDIR     = src
OBJDIR     = obj
SOURCES    = main.cpp \
             laby.cpp
OBJECTS    = $(SOURCES:%.cpp=$(OBJDIR)/%.o)
EXECUTABLE = laby

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(@:$(OBJDIR)/%.o=$(SRCDIR)/%.cpp) -o $@

$(OBJECTS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

# $(OBJECTS): $(SOURCES)
# 	$(CC) $(CFLAGS) $(@:obj/%.o=src/%.cpp) -o $@

.PHONY: clean
clean:
	rm -rfv $(EXECUTABLE)

CC         = g++
CFLAGS     = -std=c++14 -c -O2 -Wall -Wextra -MMD
LFLAGS     = -std=c++14 -O2 -Wall -Wextra
SRCDIR     = src
OBJDIR     = obj
SOURCES    = main.cpp \
             laby.cpp \
             cell.cpp
OBJECTS    = $(SOURCES:%.cpp=$(OBJDIR)/%.o)
EXECUTABLE = laby

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(@:$(OBJDIR)/%.o=$(SRCDIR)/%.cpp) -o $@

-include $(OBJDIR)/*.d

$(OBJECTS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rfv $(OBJDIR)/*.o $(OBJDIR)/*.d $(EXECUTABLE)

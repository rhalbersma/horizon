CXX ?= g++
CXXFLAGS ?= -O2
CXXFLAGS += -pthread -MMD -MP
LDFLAGS += -pthread

SRCDIR := c-files
OBJDIR := obj
TARGET := horizon

SOURCES := \
	Drivers.v2.4.cpp \
	FreePath.v2.4.cpp \
	Patterns.v2.4.cpp \
	alphabeta.v2.6.cpp \
	compact.v2.6.cpp \
	damend.v2.4.cpp \
	eval.v2.4.cpp \
	generate_legal_moves.v2.4.cpp \
	hash.v2.4.cpp \
	search_tree.v2.4.cpp \
	socket_handler.v2.4.cpp \
	voorpost.cpp \
	voorpost_procs.cpp \
	paths.cpp

OBJECTS := $(SOURCES:%.cpp=$(OBJDIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

-include $(OBJECTS:.o=.d)

.PHONY: all clean

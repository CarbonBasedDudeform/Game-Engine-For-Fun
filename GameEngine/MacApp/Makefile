# Makefile

# Compiler settings
CXX = clang++
CXXFLAGS = -std=c++20 $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)

# Directories
SRCDIR = src
OUTDIR = output

# Target executable name
TARGET = $(OUTDIR)/MacAppDemo

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OUTDIR)/%.o)

# Default target
all: $(OUTDIR) $(TARGET)

# Create output directory
$(OUTDIR):
	mkdir -p $(OUTDIR)

# Linking the executable from the object files
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compiling the source files into object files
$(OUTDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OUTDIR)

.PHONY: all clean

# Compiler settings
CXX = clang++
CXXFLAGS = -std=c++20 $(shell sdl2-config --cflags) -I../include
LDFLAGS = $(shell sdl2-config --libs)

# Directories
SRCDIR = src
OUTDIR = output
LIBDIR = ../lib

# Target executable name
TARGET = $(OUTDIR)/MacDemoApp

# Source files - Include both DemoApp and GraphicsEngine sources
SOURCES = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(LIBDIR)/*.cpp)

# Object files
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OUTDIR)/%.o,$(wildcard $(SRCDIR)/*.cpp)) \
          $(patsubst $(LIBDIR)/%.cpp,$(OUTDIR)/%.o,$(wildcard $(LIBDIR)/*.cpp))

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

$(OUTDIR)/%.o: $(LIBDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OUTDIR)

.PHONY: all clean

# Copyright (c) 2020 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
# 
# Modifications made by @return215 to fit customized workflow
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

# Define custom functions
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
platformpth = $(subst /,$(PATHSEP),$1)

# Set global macros
buildDir := bin
objDir := obj
srcDir := src
executable := app
libDir = lib/$(platform)
target = $(buildDir)/$(platform)/$(executable)
sources := $(call rwildcard,$(srcDir)/,*.cpp)
objects := $(patsubst src/%, $(objDir)/%, $(patsubst %.cpp, %.o, $(sources)))
depends := $(patsubst %.o, %.d, $(objects))
compileFlags := -std=c++17 -I include
linkFlags = -L $(libDir) -l raylib

# Check for Windows
ifeq ($(OS), Windows_NT)
	# Set Windows macros
	platform := windows
	CXX ?= g++
	linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
	libGenDir := src
	THEN := &&
	PATHSEP := \$(BLANK)
	MKDIR := -mkdir -p
	RM := -del /q
	COPY = -robocopy "$(call platformpth,$1)" "$(call platformpth,$2)" $3
else
	# Check for MacOS/Linux
	UNAMEOS := $(shell uname)
	ifeq ($(UNAMEOS), Linux)
		# Set Linux macros
		platform := linux
		CXX ?= g++
		linkFlags += -l GL -l m -l pthread -l dl -l rt -l X11
		libGenDir := src
	endif
	ifeq ($(UNAMEOS), Darwin)
		# Set macOS macros
		platform := mac
		CXX ?= clang++
		linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
		libGenDir := src
	endif

	# Set UNIX macros
	THEN := ;
	PATHSEP := /
	MKDIR := mkdir -p
	RM := rm -rf
	COPY = cp $1$(PATHSEP)$3 $2
endif

# Lists phony targets for Makefile
.PHONY: all setup submodules build run clean clean-lib

# Default target, compiles
all: setup build

build: $(target)

# Sets up the project for compiling, generates includes and libs
setup: include lib

# Pull and update the the build submodules
submodules:
	git submodule update --init --recursive --depth 1

# Copy the relevant header files into includes
include: submodules
	$(MKDIR) $(call platformpth, ./include)
	$(call COPY,vendor/raylib/src,./include,raylib.h)
	$(call COPY,vendor/raylib/src,./include,raymath.h)
	$(call COPY,vendor/raylib-cpp/include,./include,*.hpp)

# Build the raylib static library file and copy it into lib
lib: submodules
	cd vendor/raylib/src $(THEN) "$(MAKE)" PLATFORM=PLATFORM_DESKTOP
	$(MKDIR) $(call platformpth, $(libDir))
	$(call COPY,vendor/raylib/$(libGenDir),lib/$(platform),libraylib.a)

# Link the program and create the executable
$(target): $(objects)
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) $(objects) -o $(target) $(linkFlags)

# Add all rules from dependency files
-include $(depends)

# Compile objects to the build directory
$(objDir)/%.o: $(srcDir)/%.cpp Makefile
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS)

# Run the executable
run:
	$(target) $(ARGS)

# Clean up all relevant files
clean:
	$(RM) $(call platformpth, $(buildDir)/*)
	$(RM) $(call platformpth, $(objDir)/*)

# Clean up raylib ilbs and includes
clean-lib:
	$(RM) $(call platformpth, $(libDir)/*)
	$(RM) $(call platformpth, include/*)
	cd vendor/raylib-cpp/vendor/raylib/src $(THEN) "$(MAKE)" clean

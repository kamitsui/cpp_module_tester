# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kamitsui <kamitsui@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/15 19:38:29 by kamitsui          #+#    #+#              #
#    Updated: 2025/07/20 20:47:40 by kamitsui         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# @file cpp02/Makefile
# @brief This File is a Makefile for building of Tester Fixed class in ../../cpp02 directory.

# Directories
PRJ_ROOT = ../../cpp02
# Default for the build rule (re, fclean, clean)
OBJ_DIR = objs
DEP_DIR = .deps

# Compiler
CXX = c++
CF_DEP = -MMD -MP -MF $(@:$(OBJ_DIR)/%.o=$(DEP_DIR)/%.d)

# Google Test Path without .env file.
# 42 tokyo campas PC : no define this variable in .env file.
CXXFLAGS ?= -Wall -Wextra -Werror -std=c++17 -I$(HOME)/goinfre/.brew/include -pthread
LIBS ?= -L$(HOME)/goinfre/.brew/lib -lgtest -lgtest_main

# Google Test Path with .env. ( My environment is Ubuntu22.04 )
#CXXFLAGS = -Wall -Wextra -Werror -std=c++14 -I/usr/include -pthread
#LIBS = -L/usr/lib -lgtest -lgtest_main

# if existing .env file, read variable.
# '-' initial hyphen is no error if '.env' is no existing.
-include .env

# ALL Targets
TARGET_EX01 = ex01_app
TARGET_EX02 = ex02_app
ALL = $(TARGET_EX01) $(TARGET_EX02)

# ex01
ifeq ($(MAKECMDGOALS),ex01)
PRJ_DIR = $(PRJ_ROOT)/ex01
OBJ_DIR = objs/ex01
DEP_DIR = .deps/ex01
SRCS = Fixed.cpp test_ex01.cpp
NAME = $(TARGET_EX01)
endif

# ex02 : using google tester
ifeq ($(MAKECMDGOALS),ex02)
PRJ_DIR = $(PRJ_ROOT)/ex02
OBJ_DIR = objs/ex02
DEP_DIR = .deps/ex02
SRCS = Fixed.cpp test_ex02.cpp
NAME = $(TARGET_EX02)
endif

#ifeq ($(PRJ_DIR),)
#$(error "Usage: make ex01 or make ex02")
#endif

# Selected Target Project Directory
SRCS_DIR = $(PRJ_DIR)
INC_DIR += $(PRJ_DIR)

# vpath for serching source files in multiple directories
vpath %.cpp $(SRCS_DIR)

# Object files and dependency files
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
DEPS = $(addprefix $(DEP_DIR)/, $(SRCS:.cpp=.d))

# Rules for building object files
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(DEP_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) $(CF_DEP) -c $< -o $@

# Rules for building dependency files
$(DEP_DIR)/%.d: %.c
	@mkdir -p $(DEP_DIR)

# Default target
all:
	make ex01
	make ex02
.PHONY: all

# Build Target
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(NAME)

# Rule for removing object & dependency files
clean:
	rm -rf $(OBJ_DIR) $(DEP_DIR)
.PHONY: clean

# Rule for removing Target & others
fclean: clean
	rm -f $(ALL)
.PHONY: fclean

# Rule for Clean & Build Target
re: fclean all
.PHONY: re

# Rule for ex01 target
ex01: $(NAME)
	@echo "Building ex01 application"
	$(call ASCII_ART,$(NAME))
.PHONY: ex01

# Rule for ex02 target
ex02: $(NAME)
	@echo "Building ex02 application"
	$(call ASCII_ART,$(NAME))
.PHONY: ex02

# ASCII Art : Display Tips the way to use.
define ASCII_ART
	@echo " _____________________________________________"
	@echo "< Usage :                                     >"
	@echo "<  $$ ./$(1)                               >"
	@echo "----------------------------------------------"
	@echo "       \   ^__^"
	@echo "        \  (oo)\_______"
	@echo "           (__)\       )\\/\\"
	@echo "               ||----w |"
	@echo "               ||     ||\n"
endef

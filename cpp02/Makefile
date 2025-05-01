# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kamitsui <kamitsui@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/15 19:38:29 by kamitsui          #+#    #+#              #
#    Updated: 2025/05/02 00:15:28 by kamitsui         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# @file cpp02/Makefile
# @brief This File is a Makefile for building of Tester Fixed class in ../../cpp02 directory.

# Directories
OBJ_DIR = objs
DEP_DIR = .deps
PRJ_ROOT = ../../cpp02

# Source files
ifeq ($(MAKECMDGOALS),ex01)
PRJ_DIR = $(PRJ_ROOT)/ex01
SRCS = Fixed.cpp test_ex01.cpp
NAME = ex01_app
endif

ifeq ($(MAKECMDGOALS),ex02)
PRJ_DIR = $(PRJ_ROOT)/ex02
SRCS = AnotherClass.cpp text_ex02.cpp
NAME = ex02_app
endif

#ifeq ($(PRJ_DIR),)
#$(error "Usage: make ex01 or make ex02")
#endif

SRCS_DIR = $(PRJ_DIR)
INC_DIR = $(PRJ_DIR)

# vpath for serching source files in multiple directories
vpath %.cpp $(SRCS_DIR)

# Object files and dependency files
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
DEPS = $(addprefix $(DEP_DIR)/, $(SRCS:.cpp=.d))

# Compiler
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11
CF_DEP = -MMD -MP -MF $(@:$(OBJ_DIR)/%.o=$(DEP_DIR)/%.d)

# Rules for building object files
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(DEP_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) $(CF_DEP) -c $< -o $@

# Rules for building dependency files
$(DEP_DIR)/%.d: %.c
	@mkdir -p $(DEP_DIR)

ALL = ex01_app
# Default target
all:
	make ex01
.PHONY: all

# Build Target
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

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

ex01: ex01_app
	@echo "Building ex01 application"
	$(call ASCII_ART,${NAME})
.PHONY: ex01

ex02: ex02_app
	@echo "Building ex02 application"
	$(call ASCII_ART,${NAME})
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

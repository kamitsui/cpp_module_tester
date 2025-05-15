# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kamitsui <kamitsui@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/15 19:38:29 by kamitsui          #+#    #+#              #
#    Updated: 2025/05/15 14:40:47 by kamitsui         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# @file cpp03/Makefile
# @brief This File is a Makefile for building of Tester Fixed class in ../../cpp03 directory.

# Directories
PRJ_ROOT = ../../cpp03
# Default for the build rule (re, fclean, clean)
OBJ_DIR = objs
DEP_DIR = .deps

# Compiler
CXX = c++
CF_DEP = -MMD -MP -MF $(@:$(OBJ_DIR)/%.o=$(DEP_DIR)/%.d)

# ALL Targets
TARGET_EX00 = ex00_app
TARGET_EX01 = ex01_app
TARGET_EX02 = ex02_app
TARGET_EX03 = ex03_app
ALL = $(TARGET_EX00) $(TARGET_EX01) $(TARGET_EX02) $(TARGET_EX03)

# Definitions for building ex00 test program.
ifeq ($(MAKECMDGOALS),ex00)
PRJ_DIR = $(PRJ_ROOT)/ex00
OBJ_DIR = objs/ex00
DEP_DIR = .deps/ex00
SRCS = ClapTrap.cpp test_ex00.cpp
# My environ (Ubuntu22.04)
#CXXFLAGS = -Wall -Wextra -Werror -std=c++14 -I/usr/include -pthread
#LIBS = -L/usr/lib -lgtest -lgtest_main
# 42 tokyo campas PC
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -I$(HOME)/goinfre/.brew/include -pthread
LIBS = -L$(HOME)/goinfre/.brew/lib -lgtest -lgtest_main
NAME = $(TARGET_EX00)
endif

# Definitions for building ex01 test program.
ifeq ($(MAKECMDGOALS),ex01)
PRJ_DIR = $(PRJ_ROOT)/ex01
OBJ_DIR = objs/ex01
DEP_DIR = .deps/ex01
SRCS = ClapTrap.cpp ScavTrap.cpp test_ex01.cpp
# My environ (Ubuntu22.04)
#CXXFLAGS = -Wall -Wextra -Werror -std=c++14 -I/usr/include -pthread
#LIBS = -L/usr/lib -lgtest -lgtest_main
# 42 tokyo campas PC
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -I$(HOME)/goinfre/.brew/include -pthread
LIBS = -L$(HOME)/goinfre/.brew/lib -lgtest -lgtest_main
NAME = $(TARGET_EX01)
endif

# Definitions for building ex02 test program.
ifeq ($(MAKECMDGOALS),ex02)
PRJ_DIR = $(PRJ_ROOT)/ex02
OBJ_DIR = objs/ex02
DEP_DIR = .deps/ex02
SRCS = ClapTrap.cpp ScavTrap.cpp FragTrap.cpp test_ex02.cpp
# My environ (Ubuntu22.04)
# CXXFLAGS = -Wall -Wextra -Werror -std=c++14 -I/usr/include -pthread
# LIBS = -L/usr/lib -lgtest -lgtest_main
# 42 tokyo campas PC
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -I$(HOME)/goinfre/.brew/include -pthread
LIBS = -L$(HOME)/goinfre/.brew/lib -lgtest -lgtest_main
NAME = $(TARGET_EX02)
endif

# Definitions for building ex03 test program.
ifeq ($(MAKECMDGOALS),ex03)
PRJ_DIR = $(PRJ_ROOT)/ex03
OBJ_DIR = objs/ex03
DEP_DIR = .deps/ex03
SRCS = ClapTrap.cpp ScavTrap.cpp FragTrap.cpp DiamondTrap.cpp test_ex03.cpp
# My environ (Ubuntu22.04)
#CXXFLAGS = -Wall -Wextra -Werror -std=c++14 -I/usr/include -pthread
#LIBS = -L/usr/lib -lgtest -lgtest_main
# 42 tokyo campas PC
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -I$(HOME)/goinfre/.brew/include -pthread
LIBS = -L$(HOME)/goinfre/.brew/lib -lgtest -lgtest_main
NAME = $(TARGET_EX03)
endif

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
	make ex00
	make ex01
	make ex02
	make ex03
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
ex00: $(NAME)
	@echo "Building ex00 application"
	$(call ASCII_ART,$(NAME))
.PHONY: ex00

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

# Rule for ex03 target
ex03: $(NAME)
	@echo "Building ex03 application"
	$(call ASCII_ART,$(NAME))
.PHONY: ex03

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

# Makefile Option : Disable '--print-directory'
MAKEFLAGS += --no-print-directory

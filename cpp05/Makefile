# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kamitsui <kamitsui@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/15 19:38:29 by kamitsui          #+#    #+#              #
#    Updated: 2025/06/29 04:56:27 by kamitsui         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# @file cpp05/Makefile
# @brief This File is a Makefile for building of Tester Fixed class in ../../cpp03 directory.

# Directories
PRJ_ROOT = ../../cpp05
# Default for the build rule (re, fclean, clean)
OBJ_DIR = objs
DEP_DIR = .deps

# Compiler
CXX = c++
CF_DEP = -MMD -MP -MF $(@:$(OBJ_DIR)/%.o=$(DEP_DIR)/%.d)
# Google Test Path
# My environ (Ubuntu22.04)
CXXFLAGS = -Wall -Wextra -Werror -std=c++14 -I/usr/include -pthread
LIBS = -L/usr/lib -lgtest -lgtest_main
# 42 tokyo campas PC
#CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -I$(HOME)/goinfre/.brew/include -pthread
#LIBS = -L$(HOME)/goinfre/.brew/lib -lgtest -lgtest_main

# ALL Targets
TARGET_EX00 = ex00_app
TARGET_EX01 = ex01_app
TARGET_EX02 = ex02_app
TARGET_EX03 = ex03_app
ALL = $(TARGET_EX00) $(TARGET_EX01) $(TARGET_EX02) $(TARGET_EX03)

# Definitions for building ex00 test program.
ifeq ($(MAKECMDGOALS),ex00)
EX_NUM = ex00
SRCS = Bureaucrat.cpp BureaucratTest.cpp main.cpp
NAME = $(TARGET_EX00)
endif

# Definitions for building ex01 test program.
ifeq ($(MAKECMDGOALS),ex01)
EX_NUM = ex01
SRCS = Bureaucrat.cpp Form.cpp main.cpp \
	   FormBureaucratTest.cpp 
NAME = $(TARGET_EX01)
endif

# Definitions for building ex02 test program.
ifeq ($(MAKECMDGOALS),ex02)
EX_NUM = ex02
SRCS = \
	   Bureaucrat.cpp AForm.cpp \
	   ShrubberyCreationForm.cpp RobotomyRequestForm.cpp PresidentialPardonForm.cpp \
	   AFormBureaucratTests.cpp \
	   main.cpp

NAME = $(TARGET_EX02)
endif

# Definitions for building ex03 test program.
ifeq ($(MAKECMDGOALS),ex03)
EX_NUM = ex03
SRCS = \
	   Bureaucrat.cpp AForm.cpp Intern.cpp \
	   ShrubberyCreationForm.cpp RobotomyRequestForm.cpp PresidentialPardonForm.cpp \
	   InternTests.cpp \
	   main.cpp
NAME = $(TARGET_EX03)
endif

# Selected Target Project Directory
PRJ_DIR = $(PRJ_ROOT)/$(EX_NUM)
SRCS_DIR = ./ $(EX_NUM) $(PRJ_DIR)
INC_DIR += $(PRJ_DIR)
OBJ_DIR = objs/$(EX_NUM)
DEP_DIR = .deps/$(EX_NUM)

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
$(DEP_DIR)/%.d: %.cpp
	@mkdir -p $(DEP_DIR)

# Enable dependency file
-include $(DEPS)

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

# Rule for ex00 target
ex00: $(NAME)
	@echo "Build" "'$(TARGET_EX00)'" "Complete!"
	$(call ASCII_ART,$(NAME))
	$(call ASK_AND_EXECUTE_ON_YES, ./$(NAME))
.PHONY: ex00

# Rule for ex01 target
ex01: $(NAME)
	@echo "Build" "'$(TARGET_EX01)'" "Complete!"
	$(call ASCII_ART,$(NAME))
	$(call ASK_AND_EXECUTE_ON_YES, ./$(NAME))
.PHONY: ex01

# Rule for ex02 target
ex02: $(NAME)
	@echo "Build" "'$(TARGET_EX02)'" "Complete!"
	$(call ASCII_ART,$(NAME))
	$(call ASK_AND_EXECUTE_ON_YES, ./$(NAME))
.PHONY: ex02

# Rule for ex03 target
ex03: $(NAME)
	@echo "Build" "'$(TARGET_EX03)'" "Complete!"
	$(call ASCII_ART,$(NAME))
	$(call ASK_AND_EXECUTE_ON_YES, ./$(NAME))
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

# Ask and Exit on No
define ASK_AND_EXECUTE_ON_YES
	@echo "Execute? $(1) (y/n)"
	@read -r answer; \
	case "$$answer" in \
		[yY]) \
			echo $(1); \
			$(1); \
			;; \
		*) \
			echo "Skip ..."; \
			;; \
	esac
endef

# Makefile Option : Disable '--print-directory'
MAKEFLAGS += --no-print-directory

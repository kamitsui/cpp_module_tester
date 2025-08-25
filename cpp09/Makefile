# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kamitsui <kamitsui@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/15 19:38:29 by kamitsui          #+#    #+#              #
#    Updated: 2025/08/25 18:33:31 by kamitsui         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# @file cpp09/Makefile

# Directories
PRJ_ROOT = ../../cpp09
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
TARGET_EX00 = ex00_app
TARGET_EX01 = ex01_app
TARGET_EX02 = ex02_app
ALL = $(TARGET_EX00) $(TARGET_EX01) $(TARGET_EX02)

# Definitions for building ex00 test program.
ifeq ($(MAKECMDGOALS),ex00)
EX_NUM = ex00
SRCS = test_bitcoin_exchange.cpp main.cpp BitcoinExchange.cpp
NAME = $(TARGET_EX00)
endif

# Definitions for building ex01 test program.
ifeq ($(MAKECMDGOALS),ex01)
EX_NUM = ex01
SRCS = test_span.cpp main.cpp Span.cpp
NAME = $(TARGET_EX01)
endif

# Definitions for building ex02 test program.
ifeq ($(MAKECMDGOALS),ex02)
EX_NUM = ex02
SRCS = test_mutantstack.cpp test_mutantstack_exceptions.cpp main.cpp
NAME = $(TARGET_EX02)
endif

# Selected Target Project Directory
PRJ_DIR = $(PRJ_ROOT)/$(EX_NUM)
SRCS_DIR = ./ $(EX_NUM) $(PRJ_DIR)
CF_INC = -I$(PRJ_DIR) -I$(EX_NUM)
OBJ_DIR = objs/$(EX_NUM)
DEP_DIR = .deps/$(EX_NUM)

# vpath for serching source files in multiple directories
vpath %.cpp $(SRCS_DIR)

# Object files and dependency files
APP_OBJS = $(addprefix $(OBJ_DIR)/, $(APP_SRCS:.cpp=.o))
TEST_OBJS = $(addprefix $(OBJ_DIR)/, $(TEST_SRCS:.cpp=.o))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
DEPS = $(addprefix $(DEP_DIR)/, $(SRCS:.cpp=.d))

# Rules for building object files
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(DEP_DIR)
	$(CXX) $(CXXFLAGS) $(CF_INC) $(CF_DEP) -c $< -o $@

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

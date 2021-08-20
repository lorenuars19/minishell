# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/04/10 13:37:24 by lorenuar          #+#    #+#              #
#    Updated: 2021/08/03 21:18:52 by lorenuar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ================================ VARIABLES ================================= #

# The name of your executable
NAME	= minishell

# Compiler and compiling flags
CC	= gcc

ifneq ($(SOFT), 1)
CFLAGS	= -Wall -Werror -Wextra
endif

# Debug, use with`make DEBUG=1`
ifeq ($(DEBUG),1)
CFLAGS	+= -g3 -fsanitize=address
endif

# Folder name
SRCDIR	= src/
INCDIR	= includes/
OBJDIR	= bin/

# Add include folder
CFLAGS	+= -I $(INCDIR)

CFLAGS += -I ./lib/lib_utils/includes/

ifeq ($(DBG),1)
	CFLAGS += -g3 -fsanitize=address -fsanitize=leak
endif

# Linking stage flags
LIB_UTILS = ./lib/lib_utils/libutils.a

LDFLAGS = -lreadline -ltermcap $(LIB_UTILS)

###▼▼▼<src-updater-do-not-edit-or-remove>▼▼▼
# **************************************************************************** #
# **   Generated with https://github.com/lorenuars19/makefile-src-updater   ** #
# **************************************************************************** #

SRCS = \
	./src/exec.c \
	./src/ft_split.c \
	./src/merge_tokens.c \
	./src/scanner.c \
	./src/handle_heredoc.c \
	./src/main.c \
	./src/syntax_checker.c \
	./src/parser.c \
	./src/builtins.c \
	./src/scanner_utils.c \
	./src/variable_expansion.c \
	./src/ft_itoa.c \

HEADERS = \
	./includes/parsing.h\
	./includes/minishell.h\
	./includes/exec.h\

###▲▲▲<src-updater-do-not-edit-or-remove>▲▲▲

# String manipulation magic
SRC		:= $(notdir $(SRCS))
OBJ		:= $(SRC:.c=.o)
OBJS	:= $(addprefix $(OBJDIR), $(OBJ))

# Colors
GR	= \033[32;1m
RE	= \033[31;1m
YE	= \033[33;1m
CY	= \033[36;1m
RC	= \033[0m

# Implicit rules
VPATH := $(SRCDIR) $(OBJDIR) $(shell find $(SRCDIR) -type d)

# ================================== RULES =================================== #

ifneq ($(NOLIB), 1)
define MAKE_LIB_UTILS =
	@printf "$(GR)=== Compile $(dir $(LIB_UTILS)) $(MAKECMDGOALS) $(RC)\n"
	@$(MAKE) -C $(dir $(LIB_UTILS)) $(MAKECMDGOALS)
endef
else
define MAKE_LIB_UTILS =
	@echo Compiled without library
endef
endif

all : $(LIB_UTILS) $(NAME)

# Compiling
$(OBJDIR)%.o : %.c $(HEADERS)
	@mkdir -p $(OBJDIR)
	@printf "$(GR)+$(RC)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIB_UTILS) :
	$(MAKE_LIB_UTILS)

# Linking
$(NAME)	:  $(SRCS)  $(OBJS)
	@printf "\n$(GR)=== Compiled [$(CC) $(CFLAGS)] ===\n--- $(SRC)$(RC)\n"
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@printf "$(YE)&&& Linked [$(CC) $(LDFLAGS)] &&&\n--- $(NAME)$(RC)\n"

# Cleaning
clean :
	$(MAKE_LIB_UTILS)
	@printf "$(RE)--- Removing $(OBJDIR)$(RC)\n"
	@rm -rf $(OBJDIR)

fclean : clean
	@printf "$(RE)--- Removing $(NAME)$(RC)\n"
	@rm -f $(NAME)

# Special rule to force to remake everything
re : fclean all

# This runs the program
run : $(NAME)
	@printf "$(CY)>>> Running $(NAME)$(RC)"
	./$(NAME)

# This specifies the rules that does not correspond to any filename
.PHONY	= all run clean fclean re

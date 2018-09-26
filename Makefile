# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/11 14:19:41 by jfortin           #+#    #+#              #
#    Updated: 2018/09/19 16:35:03 by jfortin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS = -Wall -Wextra -Werror -MMD

ifeq ($(DEBUG),yes)
	CFLAGS += -g -DDEBUG #-fsanitize=address
else
	CFLAGS += -O2 -flto -march=native
endif

ifeq ($(PADDING),)
	CFLAGS += -D PADDING=16
else
	CFLAGS += -D PADDING=$(PADDING)
endif

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

SRC_NAME = malloc.c\
		free.c\
		realloc.c\
		show_alloc_mem.c\
		print_debug.c\
		ft_memcpy.c\
		ft_memset.c\
		calloc.c

SRC_DIR = src
BUILD_DIR = obj
INC_DIR = includes

OBJ = $(addprefix $(BUILD_DIR)/,$(SRC_NAME:.c=.o))
DPD = $(addprefix $(BUILD_DIR)/,$(SRC_NAME:.c=.d))
SRC = $(addprefix $(SRC_DIR)/,$(SRC_NAME))

opti:
		@$(MAKE) all -j

debug:
		@$(MAKE) opti DEBUG=yes

all: $(NAME)

$(NAME): $(OBJ)
		@echo "\033[2K \033[A"
		@gcc $(CFLAGS) -shared -o $(NAME) $(OBJ)
		@ln -sf $(NAME) $(LINK)
		@echo "$(NAME) created"
		@echo "$(NAME)" > .gitignore

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
		@mkdir -p $(BUILD_DIR)
		@echo "\033[2K Compilation of $< \033[A"
		@gcc $(CFLAGS) -I $(INC_DIR) -c $< -o $@

clean:
		@rm -rf $(BUILD_DIR)
		@echo "OBJ deleted"

fclean: clean
		@rm -f $(NAME)
		@echo "$(NAME) deleted"
		@rm -rf $(LINK)
		@echo "[$(NAME)]: link deleted"

re : fclean
		@$(MAKE) opti

.PHONY: clean

-include $(DPD)
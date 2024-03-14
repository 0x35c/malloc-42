# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ulayus <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/30 11:40:22 by ulayus            #+#    #+#              #
#    Updated: 2024/03/14 13:51:10 by ulayus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so
SHORT_NAME := libft_malloc.so

SRCS := malloc.c\
		free.c\
		utils.c\
		init.c\
		info.c

LIBFT_DIR := libs/libft/

CC := gcc

CFLAGS := -Wall -Wextra -Werror -g -std=c99 -O3
LIBS := -Llibs/libft -lft -Llibs/libft/libs/ft_printf -lft_printf

OBJS := $(addprefix objs/, $(SRCS:.c=.o))

all: $(NAME) 

$(NAME): $(OBJS)
	make -s -C $(LIBFT_DIR) --no-print-directory
	$(CC) $(OBJS) -shared -o $(NAME) $(LIBS)
	ln -sf $(NAME) $(SHORT_NAME)
	echo "$(NAME) compiled"

objs/%.o: srcs/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -fPIC -o $@

clean:
	rm -rf objs
	echo "$(NAME) objects removed"

fclean: clean
	rm -f $(NAME) $(SHORT_NAME)
	echo "$(NAME) binary removed"
	make fclean -C $(LIBFT_DIR) --no-print-directory

re: fclean all

.PHONY: all clean fclean re bonus
.SILENT:

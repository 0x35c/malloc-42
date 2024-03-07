# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ulayus <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/30 11:40:22 by ulayus            #+#    #+#              #
#    Updated: 2024/03/07 13:27:24 by ulayus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so
SHORT_NAME := libft_malloc.so

SRCS := sum.c

HDR_DIR := includes/
HDR := sum.h
HDR := $(addprefix $(HDR_DIR), $(HDR))

CC := gcc

CFLAGS := -Wall -Wextra -Werror

OBJS := $(addprefix objs/, $(SRCS:.c=.o))

all: $(NAME) 

$(NAME): $(OBJS)
	@make -s -C ./libs/libft --no-print-directory
	gcc $(OBJS) -shared -o $(NAME)
	ln -sf $(NAME) $(SHORT_NAME)

objs/%.o: srcs/%.c $(HDR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -fPIC -o $@

clean:
	rm -rf objs

fclean: clean
	rm -f $(NAME) $(SHORT_NAME)

re: fclean all

.PHONY: all clean fclean re bonus

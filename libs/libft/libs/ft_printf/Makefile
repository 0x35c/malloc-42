# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ulayus <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/30 11:40:22 by ulayus            #+#    #+#              #
#    Updated: 2024/03/12 11:47:20 by ulayus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft_printf.a

SRCS =	ft_printf.c\
		ft_dprintf.c\
		ft_printf_utils.c\
		ft_print_addr.c

HDR_DIR := includes/

CC = gcc

CFLAGS = -Wall -Wextra -Werror

OBJS := $(addprefix objs/, $(SRCS:.c=.o))

all: $(NAME) 

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)
	echo "$(NAME) compiled"

objs/%.o: srcs/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(HDR_DIR)

clean:
	rm -rf objs/
	echo "$(NAME) objects removed"

fclean: clean
	rm -f $(NAME)
	echo "$(NAME) binary removed"

re: fclean all

.SILENT:
.PHONY: all clean fclean re

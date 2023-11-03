# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/22 16:21:54 by ccarnot           #+#    #+#              #
#    Updated: 2023/10/18 15:10:48 by ccarnot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

CFLAGS = -g3 -Wall -Wextra -Werror -pthread
#CFLAGS = -fsanitize=thread -g -Wall -Wextra -Werror -pthread

INCL = -I 

SRCS = main_bonus.c clear_bonus.c parsing_bonus.c init_bonus.c \
	time_bonus.c routines_bonus.c monitor_bonus.c

OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS)
	cc -o $(NAME) $(OBJS)

all: $(NAME)

%.o: %.c
	cc $(INCL) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean
	make all

.PHONY: all clean fclean re

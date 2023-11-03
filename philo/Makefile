# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/22 16:21:54 by ccarnot           #+#    #+#              #
#    Updated: 2023/10/11 11:32:18 by ccarnot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CFLAGS = -g3 -Wall -Wextra -Werror -pthread
#CFLAGS = -fsanitize=thread -g -Wall -Wextra -Werror -pthread

INCL = -I 

SRCS = main.c clear.c parsing.c init.c time.c routines.c monitor.c

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

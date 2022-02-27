# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/15 15:06:38 by agardet           #+#    #+#              #
#    Updated: 2022/02/26 13:06:35 by agardet          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

#~~~~ Output ~~~~#

NAME		=	philo

#~~~~ Paths ~~~~#

VPATH		=	src/
PATH_OBJ	=	objs/
PATH_INC	=	inc/

#~~~~ Sources ~~~~#
 
SRCS		=	main.c \
				utils.c \
				philo_thread.c \
				actions.c

#~~~~ Objects ~~~~#

OBJS		=	$(addprefix $(PATH_OBJ), $(SRCS:.c=.o))

#~~~~ Includes ~~~~#

INCS		=	$(addprefix $(PATH_INC), philo.h)

#~~~~ Macros ~~~~#

CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror  
#-fsanitize=thread
RM			=	rm -rf

#========================================#
#=============== TARGETS ================#
#========================================#

#~~~~ Main Rules ~~~~#

all :			$(NAME)

$(NAME) :		$(OBJS)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -I $(PATH_INC)

re :			fclean all

#~~~~ Compilation Rules ~~~~#

$(PATH_OBJ)%.o :	%.c $(INCS)
					@mkdir -p $(dir $@);
					$(CC) $(CFLAGS) -c $< -o $@ -I $(PATH_INC)

#~~~~ Cleaning Rules ~~~~#

clean :
				$(RM) $(PATH_OBJ)

fclean :		
				$(RM) $(PATH_OBJ) $(NAME)

.PHONY:		all clean fclean re
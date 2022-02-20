#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jiglesia <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/09 12:55:34 by jiglesia          #+#    #+#              #
#    Updated: 2022/02/20 11:52:54 by jiglesia         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME		=	philo

#****************** INC *******************#
# General
INC			=	./include/

# Libft
SUB_MAKE	=	#./libft/
INCFT		=	#./libft/

INCLUDE		=	-I $(INC)

INC_LIB		=	#$(INCFT)/libft.a


#***************** SRC* *******************#

DIRSRC		=	.
DIRCHEC		=
DIRMV		=
DIRINPUT	=
DIRLIST		=
DIRQ_SORT	=
DIRLASER	=

SRC			=	main.c
CHECKER		=
MOVES		=
INPUT		=
LIST		=
Q_SORT		=
LOADING		=

PSRC		=	$(SRC)

#***************** DEPS ******************#

DIROBJ		=	./depo/

OAUX		=	$(PSRC:%=$(DIROBJ)%)
CAUX		=	$(CSRC:%=$(DIROBJ)%)
DEPS		=	$(OAUX:.c=.d)
OBJS		=	$(OAUX:.c=.o)
CDEPS		=	$(CAUX:.c=.d)
COBJS		=	$(CAUX:.c=.o)

ifdef FLAGS
	ifeq ($(FLAGS), no)
CFLAGS		=
	endif
	ifeq ($(FLAGS), debug)
CFLAGS		=	-Wall -Wextra -Werror -ansi -pedantic -g
	endif
else
CFLAGS		=	-Wall -Wextra -Werror
endif

CC			=	/usr/bin/clang
RM			=	/bin/rm -f
ECHO		=	/bin/echo -e

#************************ DEPS COMPILATION *************************

%.o		:		../$(DIRSRC)/%.c
				$(CC) $(CFLAGS) $(INCLUDE) -MMD -o $@ -c $<

%.o		:		../$(DIRCHEC)/%.c
				$(CC) $(CFLAGS) $(INCLUDE) -MMD -o $@ -c $<

%.o		:		../$(DIRMV)/%.c
				$(CC) $(CFLAGS) $(INCLUDE) -MMD -o $@ -c $<

%.o		:		../$(DIRINPUT)/%.c
				$(CC) $(CFLAGS) $(INCLUDE) -MMD -o $@ -c $<

%.o		:		../$(DIRLIST)/%.c
				$(CC) $(CFLAGS) $(INCLUDE) -MMD -o $@ -c $<

%.o		:		../$(DIRQ_SORT)/%.c
				$(CC) $(CFLAGS) $(INCLUDE) -MMD -o $@ -c $<

%.o		:		../$(DIRLASER)/%.c
				$(CC) $(CFLAGS) $(INCLUDE) -MMD -o $@ -c $<

#************************ MAIN COMPILATION *************************

$(NAME)	:		$(OBJS)
				@$(CC)  $(INCLUDE) $(CFLAGS) -o $(NAME) $(OBJS) $(INC_LIB)
				@$(ECHO) '> Compiled'

#bonus	:		$(COBJS)
#				$(CC)  $(INCLUDE) $(CFLAGS) -o $(CHNAME) $(COBJS) $(INC_LIB)
#				@$(ECHO) '> Compiled'

clean	:
				@($(RM) $(OBJS))
				@($(RM) $(DEPS))
				@$(ECHO) '> Directory cleaned'

all		:		$(NAME)

fclean	:		clean
				@$(RM) $(NAME)
				@$(ECHO) '> Remove executable'

re		:		fclean all


apt		:
				@(sudo apt-get install gcc make xorg libxext-dev libbsd-dev)

.PHONY	:		all clean re

-include $(DEPS)

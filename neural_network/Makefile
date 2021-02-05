SRCS =	neural_network_init.c \
		neural_network_train.c \
		neural_network_print.c \
		neural_network_test.c \
		neural_network_save.c \
		neural_network_free.c \
		array.c \
		formulas.c \
		activation_functions.c \

OBJS =		${SRCS:.c=.o}

CFLAGS =	-Wall -Wextra -Werror

NAME =		nn.a

all :		${NAME}

${NAME} :	${OBJS}
			ar rc ${NAME} ${OBJS}
			ranlib ${NAME}

clean :
			rm -f ${OBJS}

fclean :	clean
			rm -f ${NAME} ./error/error.csv
			rm -rf ./backup/*.csv

re :		fclean all

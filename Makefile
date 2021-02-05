SRCS =		main.c

CFLAGS =	-Wall -Wextra -Werror

LIBS =		dataframe/df.a neural_network/nn.a

all :				df nn
					gcc ${CFLAGS} ${SRCS} ${LIBS}

plot :				
					python3 ./neural_network/error/plot.py

df :				
					make -C dataframe/

df_clean :				
					make -C dataframe/ clean

df_fclean :				
					make -C dataframe/ fclean

nn :				
					make -C neural_network/

nn_clean :				
					make -C neural_network/ clean

nn_fclean :				
					make -C neural_network/ fclean

check_leaks :		all
					leaks -atExit -- ./a.out

clean :				df_clean nn_clean

fclean :			df_fclean nn_fclean clean
					rm -f a.out

re :				fclean all
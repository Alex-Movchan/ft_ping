NAME = ft_ping

G = gcc

FLAGS = -Wall -Wextra -Werror

LIBFT = libft/libft.a

SRC =   main.c \
        ft_error.c \
        ft_loop_recvfrom.c \
        ft_sendto.c \
        ft_signal_hendling.c \
        ft_socket.c \
        ft_validation_argument.c

OBG =   main.o \
        ft_error.o \
        ft_loop_recvfrom.o \
        ft_sendto.o \
        ft_signal_hendling.o \
        ft_socket.o \
        ft_validation_argument.o

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft/
	$(G) $(FLAGS) -o $@ $(OBJ) $(LIBFT)

%.o: %.c
	$(G) -c $< -o $@

clean:
	@make -C libft/ clean
	@rm -f $(OBJ)


fclean: clean
	@make -C libft/ fclean
	@rm -f $(NAME)
re: fclean all
	@make -C libft/ re

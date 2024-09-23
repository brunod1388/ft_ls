CC			= gcc
FLAGS		= -Wall -Wextra -Werror -o3 -g3
INCLUDES	= -I libft/includes/ -I includes/
LIBFT		= -L./libft -lft

ifeq ($(debug), 1)
	CFLAGS	= -DDEBUG=1
endif

NAME 		= ft_ls

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS 	=	main.c ft_args.c ft_ls.c ft_print.c ft_format.c ft_utils.c ft_comparators.c ft_directory.c

OBJS	=	$(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

_BLUE	=	\e[34m
_PURPLE	=	\e[35m
_CYAN	=	\e[36m
_END	=	\e[0m

#%.o: %.c
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@printf "$(NAME) object : $(_BLUE)%-33.33s\r$(_END)" $@
	@${CC} ${CFLAGS} $(INCLUDES) -c $< -o $@

$(NAME):	$(OBJS)
	@printf "\n$(NAME) objects	[$(_BLUE)✓$(_END)]\n"
	@$(MAKE) -C ./libft
	@gcc $(FLAGS) $(OBJS) $(INCLUDES) $(LIBFT) -o $(NAME)
	@printf "$(NAME)		[$(_BLUE)✓$(_END)]\n"
ifeq ($(debug), 1)
	@printf "debug mode	[$(_PURPLE)ON$(_END)]\n"
endif

all:	$(NAME)

re:	clean
	$(MAKE) $(NAME)

clean:
	@rm -rf $(OBJ_DIR) $(NAME)
	@printf "$(_PURPLE)$(NAME) object	deleted$(_END)\n"

fclean:	clean
	@$(MAKE) fclean -C ./libft
	@rm -rf $(NAME)
	@printf "$(_PURPLE)$(NAME)		deleted$(_END)\n"

dre: clean debug

test:
	./ft_ls testFolder libft -la
	ls -la testFolder libft
	exa -la testFolder libft

retest: re test

leaks:
	leaks -atExit -- ./ft_ls

PHONY: re all clean fclean
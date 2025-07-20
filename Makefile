NAME		=	minishell

# commands
CC			=	cc
CFLAGS		=	-g# -Wall -Wextra -Werror
RM			=	rm -rf
MAKEFLAGS	+=	--no-print-directory

# libft
LIBFT		=	libft
LIBFT_DIR	=	./$(LIBFT)
LIBFT_A		=	$(LIBFT_DIR)/$(LIBFT).a

# Directories
SRCS_DIR	=	./srcs
COM_DIR		=	$(SRCS_DIR)/command
EXEC_DIR	=	$(SRCS_DIR)/execute
PARS_DIR	=	$(SRCS_DIR)/parser


OBJS_DIR	=	./objs

# Sources
SRCS		=	$(wildcard $(SRCS_DIR)/*.c)
COM_SRCS	=	$(wildcard $(COM_DIR)/*.c)
EXEC_SRCS	=	$(wildcard $(EXEC_DIR)/*.c)
PARS_SRCS	=	$(wildcard $(PARS_DIR)/*.c)
SRCS		+=	$(COM_SRCS) $(EXEC_SRCS) $(PARS_SRCS)

# Objects
OBJS		=	$(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

# includes
INCLUDES	=	-I ./includes -I $(LIBFT_DIR)/includes
LDFLAGS		=	-lreadline

# OS differences
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	OS			=	mac
	INCLUDES	+=	-I/opt/homebrew/opt/readline/include
	LDFLAGS		+=	-L/opt/homebrew/opt/readline/lib
else
	OS			=	linux
	INCLUDES	+=	-I/usr/include/readline
	LDFLAGS		+=	-L/usr/lib
endif

INCLUDES		+=	-I $(LIBFT_DIR)/$(OS)/includes
SRCS			+=	$(OS_SRCS)

# Colors
RESET		=	\033[0m
BOLD		=	\033[1m
LIGHT_BLUE	=	\033[94m
YELLOW		=	\033[93m

# Progress Bar
TOTAL_FILES	:=	$(words $(OBJS))
COMPILED	=	0

define progress
	@$(eval COMPILED=$(shell echo $$(expr $(COMPILED) + 1)))
	@CURRENT_PERCENT=$$(expr $(COMPILED) \* 100 / $(TOTAL_FILES)); \
	printf "\033[K$(YELLOW)[%3d%%] Compiling: $<$(RESET)\r" $$CURRENT_PERCENT
endef

# Targets
all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	@printf "\033[K$(BOLD)$(LIGHT_BLUE)Compiling $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(LIBFT_A) -o $(NAME) $(LDFLAGS)
	@printf "\r\033[K$(BOLD)$(LIGHT_BLUE)$(NAME) created successfully!$(RESET)\n"

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(progress)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@printf "\033[K$(BOLD)$(LIGHT_BLUE)Cleaning object files...$(RESET)"
	@$(MAKE) clean -C $(LIBFT_DIR)
	@$(RM) $(OBJS_DIR)
	@printf "\r\033[K$(BOLD)$(LIGHT_BLUE)Objects cleaned!$(RESET)\n"

fclean:
	@printf "\033[K$(BOLD)$(LIGHT_BLUE)Removing $(NAME)...$(RESET)"
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@$(RM) $(NAME) $(OBJS_DIR)
	@printf "\r\033[K$(BOLD)$(LIGHT_BLUE)Full clean complete!$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re run valgrind

run: $(NAME)
	./$(NAME)

val: $(NAME)
	valgrind --leak-check=full ./$(NAME)

test: $(NAME)
	./$(NAME) 2>/dev/null

.PHONY: run val test
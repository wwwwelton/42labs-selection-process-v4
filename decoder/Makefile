SOURCES_FILES	=	main.c utils.c tree.c print.c string.c segment.c bits.c

SOURCES_DIR		=	sources

OBJ_DIR			=	objects

HEADER			=	$(SOURCES_DIR)/decoder.h

SOURCES			=	$(addprefix $(SOURCES_DIR)/, $(SOURCES_FILES))

OBJECTS			=	$(SOURCES:$(SOURCES_DIR)/%.c=$(OBJ_DIR)/%.o)

NAME			=	decoder

CC				=	gcc
RM				=	rm -rf

CFLAGS			=	-Wall -Wextra -Werror -g
LDFLAGS			=	-pthread

$(OBJ_DIR)/%.o:		$(SOURCES_DIR)/%.c $(HEADER)
					$(CC) $(CFLAGS) -c $< -o $@

all:				$(NAME)

bonus:				all

$(NAME):			$(OBJ_DIR) $(OBJECTS) $(HEADER)
					$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR):
					mkdir -p $(OBJ_DIR)

clean:
					$(RM) $(OBJ_DIR)

fclean:				clean
					$(RM) $(NAME)

re:					fclean all

.PHONY:				all clean fclean re bonus

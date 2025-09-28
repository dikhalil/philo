# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = philo

# Include
INCLUDE = -I./include

# Source files
SRCS =  

# Object files
OBJ_DIR = obj
OBJS = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Default target
all: $(NAME)

# Build pipex binary (regular)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# Compile .c to .o
$(OBJ_DIR)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Clean object files
clean:
	rm -f $(OBJS)

# Full clean: remove objects and binary
fclean: clean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re

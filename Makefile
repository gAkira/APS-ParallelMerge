NAME = aps
CC = gcc
CC_FLAG = -Wall -Wextra -Werror
OMP_FLAG = -fopenmp

SRC_DIR = ./src
SRC = $(wildcard $(SRC_DIR)/*.c)

OBJ_DIR = ./obj
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

INC_DIR = ./inc

all: help

run: compile
	@./$(NAME)

compile: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CC_FLAG) $(OMP_FLAG) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
		@$(CC) $(CC_FLAG) $(OMP_FLAG) -I$(INC_DIR) -c $< -o $@

clean:
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean compile

help:
	@echo "Attention:"
	@echo "    In order to have you program running correctly, you must a GCC version higher than 4.2.0:"
	@echo "        $$> gcc --version"
	@echo "    Install the OpenMP:"
	@echo "        $$> sudo apt install libomp-dev"
	@echo "    We can also set the number of threads in the terminal:"
	@echo "        $$> export OMP_NUM_THREADS=8"
	@echo "---------------------------------------------------------------------------------------------"
	@echo "Usage:"
	@echo "    make <command>"
	@echo "---------------------------------------------------------------------------------------------"
	@echo "COMMANDS"
	@echo "Specifics:"
	@echo "    [help]    -    list of commands"
	@echo "    run       -    compile and execute the code"
	@echo "    compile   -    compile the code"
	@echo "    clean     -    delete all objects (.o)"
	@echo "    fclean    -    delete all objects (.o) and the executable file"
	@echo "    re        -    recompile files and regenerate executable file"
	@echo "    nyan      -    Nyan? Nyan!"
	@echo
	@echo "    Example: $$> make run"
	@echo "        [This command will compile the program and run the executable file]"
	@echo ""
	@echo "    Example: $$> make re"
	@echo "        [This program will recompile all the code and regenerate the executable file]"

nyan:
	@echo "    ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"
	@echo "    ░░░░░░░░░░▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄░░░░░░░░░"
	@echo "    ░░░░░░░░▄▀░░░░░░░░░░░░▄░░░░░░░▀▄░░░░░░░"
	@echo "    ░░░░░░░░█░░▄░░░░▄░░░░░░░░░░░░░░█░░░░░░░"
	@echo "    ░░░░░░░░█░░░░░░░░░░░░▄█▄▄░░▄░░░█░▄▄▄░░░"
	@echo "    ░▄▄▄▄▄░░█░░░░░░▀░░░░▀█░░▀▄░░░░░█▀▀░██░░"
	@echo "    ░██▄▀██▄█░░░▄░░░░░░░██░░░░▀▀▀▀▀░░░░██░░"
	@echo "    ░░▀██▄▀██░░░░░░░░▀░██▀░░░░░░░░░░░░░▀██░"
	@echo "    ░░░░▀████░▀░░░░▄░░░██░░░▄█░░░░▄░▄█░░██░"
	@echo "    ░░░░░░░▀█░░░░▄░░░░░██░░░░▄░░░▄░░▄░░░██░"
	@echo "    ░░░░░░░▄█▄░░░░░░░░░░░▀▄░░▀▀▀▀▀▀▀▀░░▄▀░░"
	@echo "    ░░░░░░█▀▀█████████▀▀▀▀████████████▀░░░░"
	@echo "    ░░░░░░████▀░░███▀░░░░░░▀███░░▀██▀░░░░░░"
	@echo "    ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"

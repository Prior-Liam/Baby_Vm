
# modify this makefile based on your requirement
SRCS = main.c

FLAGS = -Wall -Wextra -Werror

NAME = cs219

all: $(NAME)

$(NAME):
	@echo "\033[32mCompiling source files\033[0m"
	@gcc main.c -o pa1

clean:
	@echo "\033[32mCleaning up\033[0m"
	@rm -rf *.o pa1

re: clean all

.PHONY: clean all re
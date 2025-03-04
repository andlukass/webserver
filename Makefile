CFLAGS = -std=c++98 #-Wall -Werror -Wextra
CC = g++

SRCS = main.cpp                    \
		./src/Utils.cpp            \
		./src/Parser.cpp           \
		./src/ServerConfig.cpp     \

NAME = webserver

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@echo "no objects to clean..."

fclean:
	rm -f $(NAME)

re: fclean all
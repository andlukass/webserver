CFLAGS = -std=c++98 #-Wall -Werror -Wextra
CC = g++

SRCS =  ./src/main.cpp             \
		./src/Utils.cpp            \
		./src/Parser.cpp           \
		./src/Location.cpp         \
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
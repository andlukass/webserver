CFLAGS = -std=c++98 -Wall -Werror -Wextra
CC = c++
		
SRCS =  ./src/main.cpp                         \
		./src/Utils.cpp                        \
		./src/ServerConfig.cpp                 \
		./src/directives/Directive.cpp         \
		./src/directives/Autoindex.cpp         \
		./src/directives/AllowMethods.cpp      \
		./src/directives/ServerDirective.cpp   \
		./src/directives/ClientMaxBodySize.cpp \
		./src/directives/ErrorPage.cpp         \
		./src/directives/Listen.cpp            \
		./src/directives/Root.cpp              \
		./src/directives/Path.cpp              \
		./src/directives/Locations.cpp         \
		./src/directives/LocationDirective.cpp \
		./src/directives/MultiDirective.cpp    \
		./src/directives/FilePathDirective.cpp \
		./src/Server.cpp                       \
		./src/ServerManager.cpp                \
		./src/cgi/CgiHandler.cpp               \
		./src/HttpRequest.cpp                  \
		./src/Client.cpp                       \

NAME = webserver

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@echo "no objects to clean..."

fclean:
	rm -f $(NAME)

re: fclean all
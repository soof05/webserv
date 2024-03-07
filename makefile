src = ./src/main.cpp ./src/config/*.cpp ./src/supervisor/*.cpp \
	./src/multiplexing/*.cpp ./src/client/*.cpp \
	./src/client/request/*.cpp ./src/client/respond/*.cpp

cc = c++ -Wall -Wextra -Werror -std=c++98

webserv = webserv

all: webserv

$(webserv): $(src)
	@$(cc) $(src) -o webserv
	@tput setaf 2; echo "webserv IS READY"

clean:
	@rm -rf *.o
	@tput setaf 2; echo "CLEAN COMPLETE"

fclean: clean
	@rm -rf webserv
	@tput setaf 2; echo "FCLEAN CLEAN"

re: clean fclean all

.PHONY: all clean fclean re
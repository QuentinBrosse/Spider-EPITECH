CC		=	g++

SRC		=	src/main.cpp \
			../Common/TCPClient.cpp \
			../Common/SSLNetwork.cpp \
			src/TCPServer.cpp \
			src/parser.cpp

OBJ		=	$(SRC:.cpp=.o)

NAME		=	server

CXXFLAGS	=	-I ./includes -I ../Common -L ../lib/linux -lssl -lcrypto -std=c++11

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(CXXFLAGS) -o $(NAME)

re: fclean $(NAME)

fclean: clean
	$(RM) $(NAME)

clean:
	$(RM) $(OBJ)

.PHONY: clean fclean re

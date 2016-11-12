CXX		=	g++

CXXFLAGS	=	-I ./includes -I ../Common
CXXFLAGS	+=	-std=c++11

LDFLAGS		=	-lssl -lcrypto

SRC		=	src/main.cpp \
			../Common/TCPClient.cpp \
			../Common/SSLCipher.cpp \
			../Common/SSLTCPClient.cpp \
			src/TCPServer.cpp \
			src/parser.cpp

OBJ		=	$(SRC:.cpp=.o)

NAME		=	server



all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

re: fclean all

fclean: clean
	$(RM) $(NAME)

clean:
	$(RM) $(OBJ)


.PHONY: all clean fclean re

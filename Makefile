NAME		= webserv

CC			= c++ 
CFLAGS		= -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address
DEP_FLAG	= -MMD -MP
INCLUDE		= -I$(INC_DIR)

RMF			= rm -f
RMD			= rm -rf
MKDIR		= mkdir -p

INC_DIR		= inc/
SRC_DIR		= src/
OBJ_DIR		= obj/
TEST_DIR	= tests/
REQ_DIR		= request/
CONF_DIR	= Parsing_configuration/

SRC_FILES	= main  Server Utils \

TEST_FILES	= HttpParserTester
REQ_FILES	= HttpRequest HttpParser RequestLine Uri

CONF_FILES = ABlockBase LocationConf ParsingConf ServerConf

ALL_FILES	+= $(SRC_FILES)
ALL_FILES	+= $(addprefix $(TEST_DIR), $(TEST_FILES))
ALL_FILES	+= $(addprefix $(REQ_DIR), $(REQ_FILES))

ALL_FILES	+= $(addprefix $(CONF_DIR), $(CONF_FILES))

SRCS		= $(addprefix $(SRC_DIR), $(addsuffix .cpp, $(ALL_FILES)))
OBJS		= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(ALL_FILES)))
DEPS		= $(addprefix $(OBJ_DIR), $(addsuffix .d, $(ALL_FILES)))

all:	$(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp Makefile
		$(MKDIR) $(dir $@)
		$(CC) $(CFLAGS) $(INCLUDE) $(DEP_FLAG) -c $< -o $@

clean:
	$(RMD) $(OBJ_DIR)

fclean: clean
	$(RMF) $(NAME)

re: fclean all

-include $(DEPS)
.PHONY:	all clean fclean re

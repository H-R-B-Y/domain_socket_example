NAME			:= libsf_shared.a
CC 				:= gcc
CFLAGS			:= -Wextra -Wall -Werror -g3 -O0

MAKEFLAGS		+= --no-print-directory

INCLUDES		:= -I$(abspath ./include) -I$(abspath ../libft_extended/include)

SRC_DIR			:= ./src

SRCS			:= \
				./src/socket_exists.c \
				./src/nonblocking_fd.c \
				./src/partial_write/partial_write_process.c \
				./src/partial_write/partial_write_management.c \
				./src/partial_write/partial_write_init.c \
				./src/partial_write/partial_write_free.c \
				./src/partial_read/partial_read_process.c \
				./src/partial_read/partial_read.c \
				\

BIN				:= \
				\

OBJS			:= $(BIN:.bin=.o) ${SRCS:.c=.o}

all: $(NAME)

$(NAME): $(OBJS)
		ar rcs $(NAME) $(OBJS)

%.o: %.c
		@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

%.o: %.bin
	@ld -r -b binary -o $@ $<

clean:
		@rm -rf $(OBJS)

rm:
		@rm -rf $(NAME)

fclean: clean rm

re: fclean all

.PHONY: re fclean rm clean all

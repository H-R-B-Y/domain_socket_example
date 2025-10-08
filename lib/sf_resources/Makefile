NAME			:= libsf_server.a
CC 				:= gcc
CFLAGS			:= -Wextra -Wall -Werror -g3 -O0

MAKEFLAGS		+= --no-print-directory

INCLUDES		:= -I$(abspath ./include) -I$(abspath ../libft_extended/include) -I$(abspath ../shared_utils/include)

SRC_DIR			:= ./src

SRCS			:= \
			./src/utility/send_to_connection.c \
			./src/utility/send_global.c \
			./src/server/server_shutdown.c \
			./src/server/server_run.c \
			./src/server/server_init.c \
			./src/server/handle_stdin/handle_terminal.c \
			./src/server/handle_management/handle_join.c \
			./src/server/handle_client/handle_write.c \
			./src/server/handle_client/handle_msg.c \
			./src/connection_lookup/disconnect_connection.c \
			./src/connection_lookup/connnection_lookup_destroy.c \
			./src/connection_lookup/connection_lookup_remove.c \
			./src/connection_lookup/connection_lookup_init.c \
			./src/connection_lookup/connection_lookup_get.c \
			./src/connection_lookup/connection_lookup_add.c \
			./src/connection_lookup/connection/new_connection.c \
			./src/connection_lookup/connection/delete_connection.c \
			./src/server/terminate_srv.c \
			./src/server/handle_management/handle_disconnect.c \
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

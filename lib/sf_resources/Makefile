SERVER_NAME			:= libsf_server.a
CLIENT_NAME			:= libsf_client.a
CC 				:= gcc
CFLAGS			:= -Wextra -Wall -Werror -g3 -O0

MAKEFLAGS		+= --no-print-directory

INCLUDES		:= -I$(abspath ./include) -I$(abspath ../libft_extended/include) -I$(abspath ../shared_utils/include)

SRC_DIR			:= ./src

SHARED_SRCS		:= \
			./src/shared/socket_exists.c \
			./src/shared/nonblocking_fd.c \
			./src/shared/partial_write/partial_write_process.c \
			./src/shared/partial_write/partial_write_management.c \
			./src/shared/partial_write/partial_write_init.c \
			./src/shared/partial_write/partial_write_free.c \
			./src/shared/partial_read/partial_read.c \
			./src/shared/partial_read/partial_read_process.c \
					\

CLIENT_SRCS			:= \
			./src/client/internal/handle_msg.c \
			./src/client/internal/handle_stdin.c \
			./src/client/internal/handle_write.c \
			./src/client/internal/on_connect.c \
			./src/client/internal/terminate_client.c \
			./src/client/client_connect.c \
			./src/client/client_init.c \
			./src/client/client_run.c \
			./src/client/client_send_message.c \
			./src/client/client_shutdown.c \
			./src/client/client_stop.c \
					\


SERVER_SRCS			:= \
			./src/server/utility/send_to_connection.c \
			./src/server/utility/send_global.c \
			./src/server/server_shutdown.c \
			./src/server/server_run.c \
			./src/server/server_init.c \
			./src/server/handle_stdin/handle_terminal.c \
			./src/server/handle_management/handle_join.c \
			./src/server/handle_client/handle_write.c \
			./src/server/handle_client/handle_msg.c \
			./src/server/connection_lookup/disconnect_connection.c \
			./src/server/connection_lookup/connnection_lookup_destroy.c \
			./src/server/connection_lookup/connection_lookup_remove.c \
			./src/server/connection_lookup/connection_lookup_init.c \
			./src/server/connection_lookup/connection_lookup_get.c \
			./src/server/connection_lookup/connection_lookup_add.c \
			./src/server/connection_lookup/connection/new_connection.c \
			./src/server/connection_lookup/connection/delete_connection.c \
			./src/server/terminate_srv.c \
			./src/server/handle_management/handle_disconnect.c \
					\

BIN				:= \
				\

SHARED_OBJS			:= $(SHARED_SRCS:.c=.o)
SERVER_OBJS			:= ${SERVER_SRCS:.c=.o}
CLIENT_OBJS			:= ${CLIENT_SRCS:.c=.o}

all: $(SERVER_NAME) $(CLIENT_NAME)

$(SERVER_NAME): $(SERVER_OBJS) $(SHARED_OBJS)
		ar rcs $(SERVER_NAME) $(SERVER_OBJS) $(SHARED_OBJS)

$(CLIENT_NAME): $(CLIENT_OBJS) $(SHARED_OBJS)
		ar rcs $(CLIENT_NAME) $(CLIENT_OBJS) $(SHARED_OBJS)

%.o: %.c
		@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

%.o: %.bin
		@ld -r -b binary -o $@ $<

clean:
		rm -rf $(SERVER_OBJS)
		rm -rf $(CLIENT_OBJS)
		rm -rf $(SHARED_OBJS)

rm:
		rm -rf $(SERVER_NAME) $(CLIENT_NAME)

fclean: clean rm

re: fclean all

.PHONY: re fclean rm clean all

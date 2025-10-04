NAME			:= server
CC 				:= gcc
CFLAGS			:= -Wextra -Wall -Werror -g3 -O0

# Flags for mlx because it needs to be compiled with specific flags for distro compatibility

MAKEFLAGS		+= --no-print-directory

LIBFLAGS		:= -L./lib/ -lft

INCLUDES		:= -I$(abspath ./include) -I$(abspath ./lib/libft/include) -I$(abspath ../include)

LIB_DIR			:= ./lib

LIBFT_DIR		:= $(abspath $(LIB_DIR)/libft)
LIBFT			:= $(LIBFT_DIR)/libft.a

SRC_DIR			:= ./src

SRCS			:= \
				./src/main_loop.c \
				./src/server/terminations.c \
				./src/server/init.c \
				./src/handlers/new_connection.c \
				./src/handlers/handle_msg.c \
				./src/handlers/handle_disconnect.c \
				\


BIN				:= \
				\

TEST_SCRIPT		:=

OBJS			:= $(BIN:.bin=.o) ${SRCS:.c=.o}

MAIN			:= $(SRC_DIR)/main.c

all: $(NAME)

$(NAME): $(MAIN) $(LIBFT) $(OBJS) 
		$(CC) $(CFLAGS) $(INCLUDES) $(MAIN) $(OBJS) $(LIBFLAGS) -o $(NAME)

$(LIBFT):
		@$(MAKE) --directory $(LIBFT_DIR) CFLAGS="$(CFLAGS)" CC=$(CC) INCLUDES=$(INCLUDES)
		cp $(LIBFT) $(LIB_DIR)

%.o: %.c
		@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

%.o: %.bin
	@ld -r -b binary -o $@ $<

clean:
		@rm -rf $(OBJS)
		@find . -name '*.gcda' -delete
		@find . -name '*.gcno' -delete

rm:
		@$(MAKE) --directory $(LIBFT_DIR) fclean
		@rm -rf $(NAME)
		@rm -rf $(LIB_DIR)/libft.a $(LIB_DIR)/libmlx.a $(LIB_DIR)/libmlxwrap.a $(LIB_DIR)/*.a

fclean: clean rm pre post

re: fclean all

# Check submodule status
submodule-status:
	@echo "=== Libft Status ==="
	@cd $(LIBFT_DIR) && git status --porcelain
	@echo "=== Parent Repo Submodule Status ==="
	@git status --porcelain lib/

install-submodules:
	@echo "Initializing and updating submodules..."
	@git submodule update --init --recursive
	@echo "Submodules initialized and updated successfully"
	@echo "Recording submodule versions in parent repository..."
	@git add lib/
	@echo "Submodule versions recorded. Run 'git commit' to save the state."

pull-submodules:
	@echo "Pulling latest changes from all submodules..."
	@git submodule update --remote --merge
	@echo "Submodules updated successfully"
	@echo "Recording updated submodule versions in parent repository..."
	@git add lib/
	@echo "Updated submodule versions recorded. Run 'git commit' to save the new state."

coverage:
ifeq ($(TEST_SCRIPT),)
	@printf "There has been an issue running coverage: no script to run program\n"
else
	@rm -rf coverage_reports
	@find . -name '*.gcda' -delete
	@make --directory . re CFLAGS="-fprofile-arcs -ftest-coverage" CC=gcc-10
	@bash -c $(TEST_SCRIPT)
	@sleep 2
	@mkdir -p coverage_reports
	@find . -name '*.gcda' -exec mv -n {} . \;
	@find . -name '*.gcno' -exec mv -n {} . \;
	@find . -name '*.c' -exec gcov-10 -o . {} \;  >> coverage_reports.txt
	@find . -name '*.gcov' -exec mv {} coverage_reports/ \;
	@find . -name '*.gcda' -delete
	@find . -name '*.gcno' -delete
endif

cachegrind:
ifeq ($(TEST_SCRIPT),)
	@printf "There has been an issue running cachegrind: no script to run program\n"
else
	@rm -rf cachegrind_reports
	@mkdir -p cachegrind_reports
	@valgrind --tool=cachegrind --cachegrind-out-file=cachegrind_reports/cachegrind.out \
		bash -c $(TEST_SCRIPT)
	@cg_annotate cachegrind_reports/cachegrind.out > cachegrind_reports/cachegrind.txt
	@printf "Cachegrind report written to cachegrind_reports/cachegrind.txt\n"
endif

perf:
ifeq ($(TEST_SCRIPT),)
	@printf "There has been an issue running perf: no script to run program\n"
else
	@rm -rf perf_reports
	@mkdir -p perf_reports
	@perf stat -e cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses \
		bash -c $(TEST_SCRIPT) 2> perf_reports/perf.txt
	@printf "Perf stats written to perf_reports/perf.txt\n"
endif


.PHONY: all clean fclean re  submodule-status install-submodules pull-submodules pre post coverage cachegrind perf
NAME		:= philo_bonus

CC			:= clang
LINK_CMD	:= clang

CFLAGS		:= -std=c89 -Wall -Wextra -pedantic
LFLAGS		:=

SRC_DIR		:= .
OBJ_DIR		:= ./obj
VPATH		:= $(SRC_DIR)

SRC_FILES	:= main.c util.c routine.c setup.c syscall.c syscall_extra.c atoui.c time.c
OBJ_FILES	:= $(addprefix $(OBJ_DIR)/,$(SRC_FILES:%.c=%.o))

SILENT		:= @
ifdef verbose
	SILENT	:=
endif

ifndef config
	config	:= debug
endif

ifeq ($(config), debug)
	CFLAGS	+= -Og -fno-inline -g3
	LFLAGS += -Og -fno-inline
	ifdef san
		CFLAGS += -fsanitize=$(san)
		LFLAGS += -fsanitize=$(san)
	endif
else ifeq ($(config), release)
	CFLAGS += -O2 -g1
	LFLAGS += -O2
else ifeq ($(config), distr)
	CFLAGS += -Ofast -g0
	LFLAGS += -Ofast -g0
else
$(error "unknown config '$(config)'")
endif

all: $(NAME)
$(NAME): $(OBJ_FILES)
	@echo Linking $(NAME)...
	$(SILENT)$(LINK_CMD) -o $(NAME) $(OBJ_FILES) $(LFLAGS)

$(OBJ_DIR)/%.o: %.c
	@echo $(notdir $<)
	$(SILENT)mkdir -p $(OBJ_DIR)
	$(SILENT)$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@echo Cleaning object files...
	$(SILENT)rm -f $(OBJ_FILES)

fclean: clean
	@echo Cleaning $(NAME)
	$(SILENT)rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

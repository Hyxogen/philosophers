NAME		:= philo

CC		:= cc
LINK_CMD	:= cc

CFLAGS		:= -Wall -Wextra
LFLAGS		:=

SRC_DIR		:= .
OBJ_DIR		:= ./obj
VPATH		:= $(SRC_DIR)

SRC_FILES	:= main.c util.c routine.c setup.c action.c atoui.c
OBJ_FILES	:= $(addprefix $(OBJ_DIR)/,$(SRC_FILES:%.c=%.o))

SILENT		:= @
ifdef verbose
	SILENT	:=
endif

ifndef config
	config	:= distr
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
	@echo LINK_CMD $(NAME)...
	$(SILENT)$(LINK_CMD) -o $(NAME) $(OBJ_FILES) $(LFLAGS)

$(OBJ_DIR)/%.o: %.c
	@echo CC $(notdir $<)
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

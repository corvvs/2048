NAME	:= 2048
B_NAME	:= 

CXX		:= clang++

CC		:= cc
CFLAGS	:= -Wall -Werror -Wextra -MMD -MP

LIBS	= -lncurses

SRCDIR	:= .
OBJDIR	:= ./obj
SRCS	:= $(shell find . -maxdepth 1 -type f -name '*.c')
OBJS	:= $(SRCS:$(SRCDIR)%.c=$(OBJDIR)%.o)
DEPENDS	:= $(SRCS:$(SRCDIR)%.c=$(OBJDIR)%.d)

INCLUDES	:= -I ./includes

.PHONY: all
all		: $(NAME) FORCE

bonus : CFLAGS += -D BONUS
# bonus : _libft $(B_NAME)

.PHONY: $(DEPENDS)
-include $(DEPENDS)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)/$(*D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME)	: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBS) -o $@

clean: FORCE
	rm -rf $(OBJDIR)

fclean: clean test-fclean
	rm -f $(NAME) $(B_NAME)

.PHONY: re
re: fclean all

.PHONY: FORCE
FORCE:

cpp:
	make CC=clang++ OBJDIR=./obj-cpp NAME=testfile "CFLAGS=$(CFLAGS) -Wno-deprecated"

-include test.mk

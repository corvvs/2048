NAME	:= 2048
B_NAME	:= 

CC		:= cc
CFLAGS	:= -Wall -Werror -Wextra -MMD -MP
IOPTS	= $(addprefix -I , $(INCLUDES))
# LOPTS	= $(addprefix -L , $(INCLUDES))
LIBS	= -lncurses

SRCDIR	:= .
OBJDIR	:= ./obj
SRCS	:= $(shell find . -maxdepth 1 -type f -name '*.c')
OBJS	:= $(SRCS:$(SRCDIR)%.c=$(OBJDIR)%.o)
DEPENDS	:= $(SRCS:$(SRCDIR)%.c=$(OBJDIR)%.d)

# LIBFTDIR	:= ./libft
# LIBFT		:= $(LIBFTDIR)/libft.a

INCLUDES	:= $(LIBFTDIR)

all		: _libft $(NAME) FORCE

-include $(DEPENDS)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)/$(*D)
	$(CC) $(CFLAGS) $(IOPTS) -c $< -o $@

$(NAME)	: $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(IOPTS) $(OBJS) $(LIBS) -o $@

$(LIBFT): _libft

_libft	: FORCE
# $(MAKE) -C $(LIBFTDIR)

bonus : CFLAGS += -D BONUS
# bonus : _libft $(B_NAME)

clean: FORCE
	$(MAKE) $@ -C $(LIBFTDIR)
	rm -rf $(OBJDIR)

fclean: clean
	$(MAKE) $@ -C $(LIBFTDIR)
	rm -f $(NAME) $(B_NAME)

.PHONY: FORCE
FORCE:
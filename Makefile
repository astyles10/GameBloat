CC		:=	gcc
SDIR	:=	src
IDIR	:=	-Iinclude
CFLAGS	:=	$(IDIR) -g -Wall -DLIN
LFLAGS	:=	-lm
ODIR	:=	debug
CFILES	:=	$(wildcard $(SDIR)/*.c)
OBJS	:=	$(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES))

emulator: $(ODIR) $(OBJS)
	$(CC) $(ODIR)/*.o -o $@ $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	@mkdir $@
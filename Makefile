CC		:=	gcc
SDIR	:=	src
IDIR	:=	-Iinclude
CFLAGS	:=	$(IDIR) -g -Wall -DLIN -DLITTLE_ENDIAN `pkg-config --cflags gtk4`
LFLAGS	:=	-lm `pkg-config --libs gtk4`
ODIR	:=	debug
MODULES	:=	MBC
CFILES	:=	$(shell find $(SDIR) -name "*.c")
OBJS	:=	$(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES))

emulator: $(ODIR) $(OBJS)
	$(CC) $(ODIR)/*.o $(ODIR)/$(MODULES)/*.o -o $@ $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p "$(@D)"
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	@mkdir $@

clean:
	@rm $(OBJS)
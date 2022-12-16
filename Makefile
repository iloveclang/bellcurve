CC=gcc
CFLAGS=-g -Wall
BINDIR=bin
OBJDIR=obj
SRCDIR=src
BIN=$(BINDIR)/bellcurve

# Debug build
$(BIN): $(OBJDIR)/main.o
	$(CC) $(CFLAGS) $^ -o $@

# Object files
$(OBJDIR)/%.o: $(SRCDIR)/$.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleaning
clean:
	$(RM) $(BINDIR)/* $(OBJDIR)/*.o




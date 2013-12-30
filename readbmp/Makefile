GCC = gcc
G++ = g++
RM = rm -f

BINS = readbmp
OBJS = readbmp.o bmp.o

all: $(BINS)

$(BINS): $(OBJS)
	$(GCC) -o $@ $(OBJS)

%:%.c
	$(GCC) -o $@ -g $<

%.o:%.c
	$(GCC) -c $<

clean:
	$(RM) $(OBJS) $(BINS)

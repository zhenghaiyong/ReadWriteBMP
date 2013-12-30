GCC = gcc
G++ = g++
RM = rm
CFLAGS = $(shell pkg-config --cflags opencv)
LDFLAGS = $(shell pkg-config --libs opencv)

BINS = video2image

all: $(BINS) 

%:%.cpp
	$(G++) $(CFLAGS) $(LDFLAGS) -o $@ -g $< 

clean:
	$(RM) $(BINS)

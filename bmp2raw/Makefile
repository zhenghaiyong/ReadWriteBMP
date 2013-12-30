GCC = gcc
G++ = g++
RM = rm

BINS = bmp2raw

all: $(BINS) 

%:%.c
	$(GCC) -o $@ -g $<

convert:
	for i in $(shell ls *.bmp);do (./bmp2raw $$i $${i%.*}); done

clean:
	$(RM) $(BINS)

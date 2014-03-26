CC=gcc
CFLAGS=--std=gnu99 -Wextra -Wno-sign-compare -pedantic-errors -g

vpath %.c src/
vpath %.h src/

worker: main.c ppmio.o graphics.o
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h, $^) -lpthread -lm

fonttest: fonttest.c font.o graphics.o graphics.o ppmio.o
	$(CC) $(CFLAGS) -o $@  $(filter-out %.h, $^)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

.PHONY: purge
purge: clean
	-rm worker
.PHONY: clean
clean:
	-rm *.o

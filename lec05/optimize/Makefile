CC = gcc
CFLAGS = -Wall -g
#CFLAGS = -O2 -DNDEBUG
LDLIBS = -lm
OBJS = main1.o func3.o optimize.o

a.out:	$(OBJS)
	$(CC) $(OBJS) $(LDLIBS)

clean:
	rm -f *~ *.o a.out

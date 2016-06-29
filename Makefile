#########define
#CC = gcc
CFLAGS = -Wall -g -O2
#CFLAGS = -Wall
INCLUDES = -I/usr/include/libxml2
LIBS = -lxml2 -lhiredis

all: objs/luwak
.PHONY: all

#########
objs/luwak: objs/luwak.o \
		objs/lwk_redis.o \
		objs/lwk_xml.o

	$(CC) -o objs/luwak \
		objs/luwak.o \
		objs/lwk_xml.o \
		objs/lwk_redis.o \
		$(LIBS)

#########
#  core  #
#########
objs/luwak.o: src/luwak.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-I src/luwak \
		-o objs/luwak.o \
		src/luwak.c

objs/lwk_xml.o: src/lwk_xml.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/lwk_xml.o \
		src/lwk_xml.c

objs/lwk_redis.o: src/lwk_redis.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/lwk_redis.o \
		src/lwk_redis.c

#########
#  clean #
#########
clean:
	rm -f objs/luwak
	rm -f objs/*.o

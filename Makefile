#########define
CC = gcc
#CFLAGS = -Wall -g -O2
CFLAGS = -Wall
INCLUDES = -I/usr/include/libxml2
LIBS = -lxml2 -lhiredis -lpthread

all: objs/luwak
.PHONY: all

#########
objs/luwak: objs/luwak.o \
		objs/lwk_xml.o \
		objs/lwk_log.o \
		objs/lwk_slock.o \
		objs/lwk_str.o \
		objs/lwk_redis.o \

	$(CC) -o objs/luwak \
		objs/luwak.o \
		objs/lwk_xml.o \
		objs/lwk_log.o \
		objs/lwk_slock.o \
		objs/lwk_str.o \
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

objs/lwk_log.o: src/lwk_log.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/lwk_log.o \
		src/lwk_log.c

objs/lwk_slock.o: src/lwk_slock.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/lwk_slock.o \
		src/lwk_slock.c

objs/lwk_str.o: src/lwk_str.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/lwk_str.o \
		src/lwk_str.c

objs/lwk_redis.o: src/lwk_redis.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/lwk_redis.o \
		src/lwk_redis.c

#########
#  clean #
#########
clean:
	rm -f luwak
	rm -f luwak.xml
	rm -f objs/luwak
	rm -f objs/*.o

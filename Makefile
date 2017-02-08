CC = gcc

CFLAGS = -Wall -Wextra -O3

md5: md5.c
	@$(CC) $(CFLAGS) -o md5 md5.c

clean:
	@$(RM) md5

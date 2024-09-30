CC = gcc

install: calendar
	ln -s $(CURDIR)/calendar ~/.local/share/bin/calendar

calendar: calendar.h argparse.h calendar.c argparse.c main.c
	$(CC) -o $@ calendar.c argparse.c main.c

clean:
	rm calendar
	rm ~/.local/share/bin/calendar

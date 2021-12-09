NFLAGS= `pkg-config --libs-only-l MLV`

CC= gcc

CFLAGS= -W -Wall -lm -std=c89 -pedantic `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV`

sources= ./module/api.c ./module/const.c ./module/string.c ./module/update.c ./module/save.c ./module/graphics.c ./module/game.c main.c

objets= $(sources:.c=.o)

clean : projet
	rm -r ./*.o ./module/*.o

projet: $(objets)
	$(CC) $(CFLAGS) -o $@  $^ $(NFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $(NFLAGS) -c $< 
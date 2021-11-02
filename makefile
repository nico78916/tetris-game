projet : game.o graphics.o save.o update.o string.o api.o const.o
	gcc -W -Wall -std=c89 -pedantic `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV` graphics.o game.o main.c update.o save.o api.o string.o const.o `pkg-config --libs-only-l MLV` -o projet -lm
	rm *.o
update.o : api.o graphics.o const.o
	gcc -W -Wall -std=c89 -pedantic `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV` -c ./module/update.c `pkg-config --libs-only-l MLV` -lm
game.o : api.o graphics.o save.o const.o
	gcc -W -Wall -std=c89 -pedantic `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV` -c ./module/game.c `pkg-config --libs-only-l MLV` -lm
graphics.o : api.o const.o
	gcc -W -Wall -std=c89 -pedantic `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV` -c ./module/graphics.c `pkg-config --libs-only-l MLV` -lm
save.o : api.o const.o
	gcc -W -Wall -std=c89 -pedantic `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV` -c ./module/save.c `pkg-config --libs-only-l MLV` -lm
api.o :
	gcc -W -Wall -std=c89 -pedantic `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV` -c ./module/api.c `pkg-config --libs-only-l MLV` -lm
string.o :
	gcc -W -Wall -std=c89 -pedantic `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV` -c ./module/string.c `pkg-config --libs-only-l MLV` -lm
const.o :
	gcc -W -Wall -std=c89 -pedantic `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV` -c ./module/const.c `pkg-config --libs-only-l MLV` -lm

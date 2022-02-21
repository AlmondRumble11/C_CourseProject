HT: paaohjelma.o ali1.o ali2.o
	gcc paaohjelma.o ali1.o ali2.o -o HT -std=c99 -Wall -pedantic
paaohjelma.o: paaohjelma.c ali1.h ali2.h
	gcc paaohjelma.c -c -std=c99 -Wall -pedantic
ali1.o: ali1.c ali1.h ali2.h
	gcc ali1.c -c -std=c99 -Wall -pedantic
ali2.o: ali2.c ali2.h
	gcc ali2.c -c -std=c99 -Wall -pedantic



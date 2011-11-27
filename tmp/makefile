all:test
test:test.o http.o maplite.o
	gcc -o test $^
test.o:test.c
	gcc -g -c $<
http.o:http.c http.h
	gcc -g -c $<
maplite.o:maplite.c maplite.h
	gcc -g -c $<

clean:
	rm *.o test
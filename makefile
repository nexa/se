LIBEVENTPATH=/home/nexa/usr/libevent/
test:test.o queue.o vector.o
	gcc -o $@ $^
se:se.o vector.o page.o spiders.o
	gcc -o $@ -levent -L${LIBEVENTPATH}/lib/ $^
se.o:se.c
	gcc -g -c -I${LIBEVENTPATH}/include/ $<
queue.o:queue.c queue.h
	gcc -g -c $<
vector.o:vector.c vector.h
	gcc -g -c $<
page.o:page.c page.h
	gcc -g -c $<
spiders.o:spiders.c
	gcc -g -c -I${LIBEVENTPATH}/include/ $<
test.o:test.c
	gcc -g -c $<



clean:
	rm *.o
	rm test
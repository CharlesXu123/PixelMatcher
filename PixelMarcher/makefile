all: test_marcher test_minheap driver

driver: marcher.c imgutils.c minheap.c driver.c
	gcc -g $^ -o $@ -lm

test_marcher: marcher.c imgutils.c minheap.c test_marcher.c
	gcc -g $^ -o $@ -lm

test_minheap: minheap.c test_minheap.c
	gcc -g $^ -o $@ -lm

clean:
	rm -f test_marcher test_minheap driver *.ppm

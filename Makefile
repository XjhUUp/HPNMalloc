function_test:./src/hpnmalloc.c ./test/test-bench.c
	gcc -g -o function.out ./src/hpnmalloc.c ./test/test-bench.c  -I ./include -lpthread -lm

clean:
	rm -f *.o *.out *.so
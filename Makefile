function_test:./src/hpnmalloc.c ./src/decpas.c ./test/test-bench.c
	gcc -g -o function.out ./src/hpnmalloc.c ./src/decpas.c ./test/test-bench.c  -I ./include -lpthread -lm


hpnmalloc:./src/hpnmalloc.c ./src/decpas.c
	gcc -g -o hpnmalloc.out  ./src/decpas.c ./src/hpnmalloc.c  -I ./include -lpthread -lm

clean:
	rm -f *.o *.out *.so
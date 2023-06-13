gcc -g -o function.out ../src/hpnmalloc.c ./test-bench.c  -I ../include -lpthread -lm

./function.out >> hpn

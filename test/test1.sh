gcc -g -o function.out ../src/hpnmalloc.c ../src/decpas.c ../test/test-bench.c  -I ../include -lpthread -lm
./function.out
rm /mnt/pmemdir2/back
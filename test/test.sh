gcc -g -o function.out ../src/hpnmalloc.c ./test-bench.c  -I ../include -lpthread -lm
g++ -o addr_handle addr_handle.cpp

../tools/pin-3.27-98718-gbeaa5d51e-gcc-linux/pin -t ../tools/pin-3.27-98718-gbeaa5d51e-gcc-linux/source/tools/ManualExamples/obj-intel64/pinatrace.so --  ./function.out | ./addr_handle >> hpn.txt

python3 static_analy.py

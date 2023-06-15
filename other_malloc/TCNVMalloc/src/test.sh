gcc -o tcnvmalloc_test pq.c tcnvmalloc.c test-bench.c -lpthread
g++ -o addr_handle addr_handle.cpp

../../../tools/pin-3.27-98718-gbeaa5d51e-gcc-linux/pin -t ../../../tools/pin-3.27-98718-gbeaa5d51e-gcc-linux/source/tools/ManualExamples/obj-intel64/pinatrace.so --  ./tcnvmalloc_test | ./addr_handle >> tcnvmalloc.txt

python3 static_analy.py

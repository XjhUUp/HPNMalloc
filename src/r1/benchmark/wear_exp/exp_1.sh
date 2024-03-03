#测试DRAM和NVM写入次数分布
gcc -g -o hpnmalloc1.out ../../HPN/hpnmalloc.c ../../HPN/wtcpa.c ./exp_code/exp_1.c -I ../../HPN/    -DHPNMALLOC -lpthread -lm
gcc -g -o hpnmalloc2.out ../../HPN/hpnmalloc.c ../../HPN/wtcpa.c ./exp_code/exp_2.c -I ../../HPN/    -DHPNMALLOC -lpthread -lm
# gcc -g -o wamalloc.out  ../../TCNVMalloc/src/tcnvmalloc.c ../../TCNVMalloc/src/wtcpa.c ../../TCNVMalloc/src/pq.c   ./exp_code/exp_2.c -I ../../TCNVMalloc/src/    -DWAMALLOC -lpthread -lm
g++ -o addr_handle ../addr_handle.cpp -I ../../HPN/ 

# i="1"
BASE="10000"

# BLKSIZE="8"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 1 >> ./exp_data/hpn_res1.txt
#     i=$(($i+1))
# done



# i="1"
# BLKSIZE="16"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 1 >> ./exp_data/hpn_res2.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="32"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 1 >> ./exp_data/hpn_res3.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="64"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 1 >> ./exp_data/hpn_res4.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="128"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 1 >> ./exp_data/hpn_res5.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="256"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 1 >> ./exp_data/hpn_res6.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="512"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 1 >> ./exp_data/hpn_res7.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="512"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE   | ./addr_handle 1 >> ./exp_data/hpn_res8.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="128"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE  | ./addr_handle 1 >> ./exp_data/hpn_res9.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="256"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE  | ./addr_handle 1 >> ./exp_data/hpn_res10.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="512"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE  | ./addr_handle 1 >> ./exp_data/hpn_res11.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="1024"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE  | ./addr_handle 1 >> ./exp_data/hpn_res12.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="2048"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE  | ./addr_handle 1 >> ./exp_data/hpn_res13.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="4096"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE  | ./addr_handle 1 >> ./exp_data/hpn_res14.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="10240"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE  | ./addr_handle 1 >> ./exp_data/hpn_res15.txt
#     i=$(($i+1))
# done

i="9"
while [ $i -lt 14 ]
do 
    python3 ../static_analy.py 1 ./exp_data/hpn_res$(($i)).txt  $i
    i=$(($i+1))
done



rm addr_handle
rm hpnmalloc1.out
rm hpnmalloc2.out
rm pinatrace.out

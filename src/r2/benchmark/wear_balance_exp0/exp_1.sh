gcc -g -o hpnmalloc.out ../../HPN/hpnmalloc.c ../../HPN/wtcpa.c ./exp_code/exp_1.c -I ../../HPN/    -DHPNMALLOC -lpthread -lm
gcc -g -o wamalloc.out  ../../TCNVMalloc/src/tcnvmalloc.c  ../../TCNVMalloc/src/pq.c   ./exp_code/exp_1.c -I ../../TCNVMalloc/src/    -DWAMALLOC -lpthread -lm
gcc -g -o nvmalloc.out ../../nvmalloc/nvmalloc.c ./exp_code/exp_1.c   -I  ../../nvmalloc -DNVMALLOC -lpthread -lm
gcc -g -o hpnmalloc1.out ../../HPN1/hpnmalloc1.c ../../HPN1/wtcpa.c ./exp_code/exp_1.c -I ../../HPN1/    -DHPNMALLOC1 -lpthread -lm

gcc -g -o hpnmalloc2.out ../../HPN/hpnmalloc.c ../../HPN/wtcpa.c ./exp_code/exp_2.c -I ../../HPN/    -DHPNMALLOC -lpthread -lm
gcc -g -o wamalloc1.out  ../../TCNVMalloc/src/tcnvmalloc.c  ../../TCNVMalloc/src/pq.c   ./exp_code/exp_2.c -I ../../TCNVMalloc/src/    -DWAMALLOC -lpthread -lm
gcc -g -o nvmalloc1.out ../../nvmalloc/nvmalloc.c ./exp_code/exp_2.c   -I  ../../nvmalloc -DNVMALLOC -lpthread -lm
gcc -g -o hpnmalloc3.out ../../HPN1/hpnmalloc1.c ../../HPN1/wtcpa.c ./exp_code/exp_2.c -I ../../HPN1/    -DHPNMALLOC1 -lpthread -lm

g++ -o addr_handle ../addr_handle.cpp   -I  ../../HPN


i="1"
BASE="10000"

# BLKSIZE="8"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res0.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res0.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res0.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res0.txt
#     i=$(($i+1))
# done






# i="1"
# BLKSIZE="16"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res1.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res1.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res1.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res1.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="32"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res2.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res2.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res2.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res2.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="64"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res3.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res3.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res3.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res3.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="128"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res4.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res4.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res4.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res4.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="256"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res5.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res5.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res5.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res5.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="512"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res6.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res6.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res6.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res6.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="512"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res7.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc3.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res7.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc1.out $TIMES $BLKSIZE   | ./addr_handle 3 >> ./exp_data/wa_res7.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc1.out $TIMES $BLKSIZE  | ./addr_handle 2 >> ./exp_data/nvm_res7.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="4096"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     # ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res8.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc3.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res8.txt
#     # ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc1.out $TIMES $BLKSIZE   | ./addr_handle 3 >> ./exp_data/wa_res8.txt
#     # ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc1.out $TIMES $BLKSIZE  | ./addr_handle 2 >> ./exp_data/nvm_res8.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="10240"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     # ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res9.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc3.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res9.txt
#     # ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc1.out $TIMES $BLKSIZE   | ./addr_handle 3 >> ./exp_data/wa_res9.txt
#     # ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc1.out $TIMES $BLKSIZE  | ./addr_handle 2 >> ./exp_data/nvm_res9.txt
#     i=$(($i+1))
# done


# i="0"
# while [ $i -lt 10 ]
# do 
#     python3 ../static_analy.py 4 ./exp_data/hpn_res$(($i)).txt ./exp_data/hpn1_res$(($i)).txt ./exp_data/wa_res$(($i)).txt ./exp_data/nvm_res$(($i)).txt $i
#     i=$(($i+1))
# done

# i="0"
# while [ $i -lt 10 ]
# do 
#     python3 ../static_analy.py 6  ./exp_data/hpn1_res$(($i)).txt ./exp_data/wa_res$(($i)).txt ./exp_data/nvm_res$(($i)).txt $i
#     i=$(($i+1))
# done

python3 ../test.py ./exp_data/hpn1_res9.txt ./exp_data/wa_res9.txt ./exp_data/nvm_res9.txt


rm hpnmalloc.out
rm hpnmalloc1.out
rm hpnmalloc2.out
rm hpnmalloc3.out
rm wamalloc.out
rm wamalloc1.out
rm nvmalloc.out
rm nvmalloc1.out
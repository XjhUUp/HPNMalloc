#优化后的内存块磨损均衡的方差
gcc -g -o hpnmalloc.out ../../HPN/hpnmalloc.c ../../HPN/wtcpa.c ./exp_code/exp_1.c -I ../../HPN/    -DHPNMALLOC -lpthread -lm
gcc -g -o wamalloc.out  ../../TCNVMalloc/src/tcnvmalloc.c  ../../TCNVMalloc/src/pq.c   ./exp_code/exp_1.c -I ../../TCNVMalloc/src/    -DWAMALLOC -lpthread -lm
gcc -g -o nvmalloc.out ../../nvmalloc/nvmalloc.c ./exp_code/exp_1.c   -I  ../../nvmalloc -DNVMALLOC -lpthread -lm
gcc -g -o hpnmalloc1.out ../../HPN1/hpnmalloc1.c ../../HPN1/wtcpa.c ./exp_code/exp_1.c -I ../../HPN1/    -DHPNMALLOC1 -lpthread -lm

gcc -g -o hpnmalloc2.out ../../HPN/hpnmalloc.c ../../HPN/wtcpa.c ./exp_code/exp_2.c -I ../../HPN/    -DHPNMALLOC -lpthread -lm
gcc -g -o wamalloc1.out  ../../TCNVMalloc/src/tcnvmalloc.c  ../../TCNVMalloc/src/pq.c   ./exp_code/exp_2.c -I ../../TCNVMalloc/src/    -DWAMALLOC -lpthread -lm
gcc -g -o nvmalloc1.out ../../nvmalloc/nvmalloc.c ./exp_code/exp_2.c   -I  ../../nvmalloc -DNVMALLOC -lpthread -lm
gcc -g -o hpnmalloc3.out ../../HPN1/hpnmalloc1.c ../../HPN1/wtcpa.c ./exp_code/exp_2.c -I ../../HPN1/    -DHPNMALLOC1 -lpthread -lm

g++ -o addr_handle ../addr_handle.cpp   -I  ../../HPN


BASE="10000"

# i="1"
# BLKSIZE="8"
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
# BLKSIZE="16"
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
# BLKSIZE="32"
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
# BLKSIZE="64"
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
# BLKSIZE="128"
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
# BLKSIZE="256"
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
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res7.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res7.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res7.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res7.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="1024"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res8.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res8.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res8.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res8.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="1536"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res9.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res9.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res9.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res9.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="2048"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res10.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res10.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res10.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res10.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="2560"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res11.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res11.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res11.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res11.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="3072"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res12.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res12.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res12.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res12.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="3584"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res13.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res13.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res13.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res13.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="4096"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res14.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res14.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res14.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/nvm_res14.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="128"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res15.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc3.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res15.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc1.out $TIMES $BLKSIZE   | ./addr_handle 3 >> ./exp_data/wa_res15.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc1.out $TIMES $BLKSIZE  | ./addr_handle 2 >> ./exp_data/nvm_res15.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="256"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res16.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc3.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res16.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc1.out $TIMES $BLKSIZE   | ./addr_handle 3 >> ./exp_data/wa_res16.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc1.out $TIMES $BLKSIZE  | ./addr_handle 2 >> ./exp_data/nvm_res16.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="512"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res17.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc3.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res17.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc1.out $TIMES $BLKSIZE   | ./addr_handle 3 >> ./exp_data/wa_res17.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc1.out $TIMES $BLKSIZE  | ./addr_handle 2 >> ./exp_data/nvm_res17.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="1024"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res18.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc3.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res18.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc1.out $TIMES $BLKSIZE   | ./addr_handle 3 >> ./exp_data/wa_res18.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc1.out $TIMES $BLKSIZE  | ./addr_handle 2 >> ./exp_data/nvm_res18.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="2048"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res19.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc3.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res19.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc1.out $TIMES $BLKSIZE   | ./addr_handle 3 >> ./exp_data/wa_res19.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc1.out $TIMES $BLKSIZE  | ./addr_handle 2 >> ./exp_data/nvm_res19.txt
#     i=$(($i+1))
# done



# i="1"
# BLKSIZE="4096"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res20.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc3.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res20.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc1.out $TIMES $BLKSIZE   | ./addr_handle 3 >> ./exp_data/wa_res20.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc1.out $TIMES $BLKSIZE  | ./addr_handle 2 >> ./exp_data/nvm_res20.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="10240"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc2.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn_res21.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc3.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/hpn1_res21.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc1.out $TIMES $BLKSIZE   | ./addr_handle 3 >> ./exp_data/wa_res21.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./nvmalloc1.out $TIMES $BLKSIZE  | ./addr_handle 2 >> ./exp_data/nvm_res21.txt
#     i=$(($i+1))
# done


i="15"
while [ $i -lt 22 ]
do 
    python3 ../static_analy.py 4 ./exp_data/hpn_res$(($i)).txt ./exp_data/hpn1_res$(($i)).txt ./exp_data/wa_res$(($i)).txt ./exp_data/nvm_res$(($i)).txt $(($i+20))
    i=$(($i+1))
done



rm hpnmalloc.out
rm hpnmalloc1.out
rm hpnmalloc2.out
rm hpnmalloc3.out
rm wamalloc.out
rm wamalloc1.out
rm nvmalloc.out
rm nvmalloc1.out
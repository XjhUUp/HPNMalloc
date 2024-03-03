#测试HPNMalloc、cpma、wamalloc在4个NVM硬件上的磨损效果
gcc -g -o hpnmalloc.out ../../HPN/hpnmalloc.c ../../HPN/wtcpa.c ./exp_code/exp_2.c -I ../../HPN/    -DHPNMALLOC -lpthread -lm
gcc -g -o cpma.out ../../CPMA/wtcpa.c ../../CPMA/cpma.c  ./exp_code/exp_2.c -I ../../CPMA/      -DCPMA -lpthread -lm
gcc -g -o wamalloc.out  ../../TCNVMalloc/src/tcnvmalloc.c ../../TCNVMalloc/src/wtcpa.c ../../TCNVMalloc/src/pq.c  ./exp_code/exp_2.c -I ../../TCNVMalloc/src/    -DWAMALLOC -lpthread -lm
g++ -o addr_handle ../addr_handle.cpp -I ../../HPN/ 


# i="1"
# BASE="10000"
# BLKSIZE="4096"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE  | ./addr_handle 3 >> ./exp_data/hpn_res1.txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./cpma.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/cpma_res1.txt
#     # ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/wa_res1.txt
#     i=$(($i+1))
# done

i="5"
BASE="10000"
BLKSIZE="10240"
while [ $i -lt 11 ]
do
    TIMES=$(($i*$BASE))
    ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc.out $TIMES $BLKSIZE | ./addr_handle 3 >> ./exp_data/hpn_res2.txt
    ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./cpma.out $TIMES $BLKSIZE  | ./addr_handle 3 >> ./exp_data/cpma_res2.txt
    # ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES  | ./addr_handle 3 >> ./exp_data/wa_res1.txt
    i=$(($i+1))
done

python3 ../static_analy.py 3 ./exp_data/hpn_res2.txt ./exp_data/cpma_res2.txt  
python3 ../static_analy.py 4 ./exp_data/hpn_res2.txt ./exp_data/cpma_res2.txt  11




rm addr_handle
rm hpnmalloc.out
rm cpma.out
rm wamalloc.out




#测试HPNMalloc和wamalloc在NVM上的元数据磨损效果
gcc -g -o hpnmalloc1.out ../../HPN/hpnmalloc.c ../../HPN/wtcpa.c ./exp_code/exp_2.c -I ../../HPN/    -DHPNMALLOC -lpthread -lm
gcc -g -o wamalloc.out  ../../TCNVMalloc/src/tcnvmalloc.c ../../TCNVMalloc/src/wtcpa.c ../../TCNVMalloc/src/pq.c   ./exp_code/exp_2.c -I ../../TCNVMalloc/src/    -DWAMALLOC -lpthread -lm
g++ -o addr_handle ../addr_handle.cpp -I ../../HPN/ 

# i="1"
# BASE="10000"
# BLKSIZE="4096"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE  | ./addr_handle 2 >> ./exp_data/hpn_res$(($i+8)).txt
#     ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/wa_res$(($i+8)).txt
#     python3 ../static_analy.py 2 ./exp_data/hpn_res$(($i+8)).txt ./exp_data/wa_res$(($i+8)).txt  ./exp_pic/hpn_res$(($i+8)).png
#     i=$(($i+1))
# done

i="40"
BASE="10000"
BLKSIZE="10240"
while [ $i -lt 41 ]
do
    TIMES=$(($i*$BASE))
    # ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./hpnmalloc1.out $TIMES $BLKSIZE  | ./addr_handle 2 >> ./exp_data/hpn_res$(($i)).txt
    ../../../../tools/pin/./pin -t ../../../../tools/pin/source/tools/ManualExamples/obj-intel64/pinatrace.so -- ./wamalloc.out $TIMES $BLKSIZE | ./addr_handle 2 >> ./exp_data/wa_res$(($i)).txt
    python3 ../static_analy.py 2 ./exp_data/hpn_res$(($i)).txt ./exp_data/wa_res$(($i)).txt  ./exp_pic/hpn_res$(($i)).png
    i=$(($i+1))
done

rm addr_handle
rm hpnmalloc1.out
rm wamalloc.out

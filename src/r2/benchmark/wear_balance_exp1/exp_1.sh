#优化后的内存块磨损均衡的平均写入次数
# gcc -g -o hpnmalloc.out ../../HPN/hpnmalloc.c ../../HPN/wtcpa.c ./exp_code/exp_1.c -I ../../HPN/    -DHPNMALLOC -lpthread -lm
# gcc -g -o wamalloc.out  ../../TCNVMalloc/src/tcnvmalloc.c  ../../TCNVMalloc/src/pq.c   ./exp_code/exp_1.c -I ../../TCNVMalloc/src/    -DWAMALLOC -lpthread -lm
# gcc -g -o nvmalloc.out ../../nvmalloc/nvmalloc.c ./exp_code/exp_1.c   -I  ../../nvmalloc -DNVMALLOC -lpthread -lm
# gcc -g -o hpnmalloc1.out ../../HPN1/hpnmalloc1.c ../../HPN1/wtcpa.c ./exp_code/exp_1.c -I ../../HPN1/    -DHPNMALLOC1 -lpthread -lm

# gcc -g -o hpnmalloc2.out ../../HPN/hpnmalloc.c ../../HPN/wtcpa.c ./exp_code/exp_2.c -I ../../HPN/    -DHPNMALLOC -lpthread -lm
# gcc -g -o wamalloc1.out  ../../TCNVMalloc/src/tcnvmalloc.c  ../../TCNVMalloc/src/pq.c   ./exp_code/exp_2.c -I ../../TCNVMalloc/src/    -DWAMALLOC -lpthread -lm
# gcc -g -o nvmalloc1.out ../../nvmalloc/nvmalloc.c ./exp_code/exp_2.c   -I  ../../nvmalloc -DNVMALLOC -lpthread -lm
# gcc -g -o hpnmalloc3.out ../../HPN1/hpnmalloc1.c ../../HPN1/wtcpa.c ./exp_code/exp_2.c -I ../../HPN1/    -DHPNMALLOC1 -lpthread -lm

# g++ -o addr_handle ../addr_handle.cpp   -I  ../../HPN




i="15"
while [ $i -lt 22 ]
do 
    python3 ../static_analy.py 5 ./exp_data/hpn_res$(($i)).txt ./exp_data/hpn1_res$(($i)).txt ./exp_data/wa_res$(($i)).txt ./exp_data/nvm_res$(($i)).txt $(($i))
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
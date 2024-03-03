# gcc -g -o hpnmalloc.out ../../HPN/hpnmalloc.c ../../HPN/wtcpa.c ./exp_code/exp_1.c -I ../../HPN/    -DHPNMALLOC -lpthread -lm
# gcc -g -o wamalloc.out  ../../TCNVMalloc/src/tcnvmalloc.c  ../../TCNVMalloc/src/pq.c  ../../TCNVMalloc/src/wtcpa.c   ./exp_code/exp_1.c -I ../../TCNVMalloc/src/     -DWAMALLOC -lpthread -lm
# gcc -g -o nvmalloc.out ../../nvmalloc/nvmalloc.c ./exp_code/exp_1.c   -I  ../../nvmalloc -DNVMALLOC -lpthread -lm
# gcc -g -o hpnmalloc1.out ../../HPN1/hpnmalloc1.c ../../HPN1/wtcpa.c ./exp_code/exp_1.c -I ../../HPN1/    -DHPNMALLOC1 -lpthread -lm

gcc -g -o hpnmalloc2.out ../../HPN/hpnmalloc.c ../../HPN/wtcpa.c ./exp_code/exp_2.c -I ../../HPN/    -DHPNMALLOC -lpthread -lm
gcc -g -o wamalloc1.out  ../../TCNVMalloc/src/tcnvmalloc.c  ../../TCNVMalloc/src/pq.c ../../TCNVMalloc/src/wtcpa.c    ./exp_code/exp_2.c -I ../../TCNVMalloc/src/    -DWAMALLOC -lpthread -lm
gcc -g -o nvmalloc1.out ../../nvmalloc/nvmalloc.c ./exp_code/exp_2.c   -I  ../../nvmalloc -DNVMALLOC -lpthread -lm
gcc -g -o hpnmalloc3.out ../../HPN1/hpnmalloc1.c ../../HPN1/wtcpa.c ./exp_code/exp_2.c -I ../../HPN1/    -DHPNMALLOC1 -lpthread -lm

# g++ -o addr_handle ../addr_handle.cpp   -I  ../../HPN


BASE="10000"

# i="1"
# BLKSIZE="8"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res21.txt
#     ./hpnmalloc1.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res21.txt
#     ./wamalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/wa_res21.txt
#     ./nvmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/nvm_res21.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="16"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res22.txt
#     ./hpnmalloc1.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res22.txt
#     ./wamalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/wa_res22.txt
#     ./nvmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/nvm_res22.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="32"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res23.txt
#     ./hpnmalloc1.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res23.txt
#     ./wamalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/wa_res23.txt
#     ./nvmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/nvm_res23.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="64"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res24.txt
#     ./hpnmalloc1.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res24.txt
#     ./wamalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/wa_res24.txt
#     ./nvmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/nvm_res24.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="128"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res25.txt
#     ./hpnmalloc1.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res25.txt
#     ./wamalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/wa_res25.txt
#     ./nvmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/nvm_res25.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="256"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res26.txt
#     ./hpnmalloc1.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res26.txt
#     ./wamalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/wa_res26.txt
#     ./nvmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/nvm_res26.txt
#     i=$(($i+1))
# done



# i="1"
# BLKSIZE="512"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res27.txt
#     ./hpnmalloc1.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res27.txt
#     ./wamalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/wa_res27.txt
#     ./nvmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/nvm_res27.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="1024"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res28.txt
#     ./hpnmalloc1.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res28.txt
#     ./wamalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/wa_res28.txt
#     ./nvmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/nvm_res28.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="1536"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res29.txt
#     ./hpnmalloc1.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res29.txt
#     ./wamalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/wa_res29.txt
#     ./nvmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/nvm_res29.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="2048"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res30.txt
#     ./hpnmalloc1.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res30.txt
#     ./wamalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/wa_res30.txt
#     ./nvmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/nvm_res30.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="2560"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res31.txt
#     ./hpnmalloc1.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res31.txt
#     ./wamalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/wa_res31.txt
#     ./nvmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/nvm_res31.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="3072"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res32.txt
#     ./hpnmalloc1.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res32.txt
#     ./wamalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/wa_res32.txt
#     ./nvmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/nvm_res32.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="3584"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res33.txt
#     ./hpnmalloc1.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res33.txt
#     ./wamalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/wa_res33.txt
#     ./nvmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/nvm_res33.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="4096"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res34.txt
#     ./hpnmalloc1.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res34.txt
#     ./wamalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/wa_res34.txt
#     ./nvmalloc.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/nvm_res34.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="128"
# THNUM="1"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc2.out $TIMES $BLKSIZE  $THNUM>> ./exp_data/hpn_res35.txt
#     ./hpnmalloc3.out $TIMES $BLKSIZE  $THNUM>> ./exp_data/hpn1_res35.txt
#     ./wamalloc1.out $TIMES $BLKSIZE    $THNUM>> ./exp_data/wa_res35.txt
#     ./nvmalloc1.out $TIMES $BLKSIZE   $THNUM>> ./exp_data/nvm_res35.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="256"
# THNUM="1"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc2.out $TIMES $BLKSIZE  $THNUM>> ./exp_data/hpn_res36.txt
#     ./hpnmalloc3.out $TIMES $BLKSIZE  $THNUM>> ./exp_data/hpn1_res36.txt
#     ./wamalloc1.out $TIMES $BLKSIZE    $THNUM>> ./exp_data/wa_res36.txt
#     ./nvmalloc1.out $TIMES $BLKSIZE   $THNUM>> ./exp_data/nvm_res36.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="512"
# THNUM="1"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc2.out $TIMES $BLKSIZE  $THNUM>> ./exp_data/hpn_res37.txt
#     ./hpnmalloc3.out $TIMES $BLKSIZE  $THNUM>> ./exp_data/hpn1_res37.txt
#     ./wamalloc1.out $TIMES $BLKSIZE    $THNUM>> ./exp_data/wa_res37.txt
#     ./nvmalloc1.out $TIMES $BLKSIZE   $THNUM>> ./exp_data/nvm_res37.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="1024"
# THNUM="1"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc2.out $TIMES $BLKSIZE $THNUM >> ./exp_data/hpn_res38.txt
#     ./hpnmalloc3.out $TIMES $BLKSIZE $THNUM >> ./exp_data/hpn1_res38.txt
#     ./wamalloc1.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/wa_res38.txt
#     ./nvmalloc1.out $TIMES $BLKSIZE  $THNUM >> ./exp_data/nvm_res38.txt
#     i=$(($i+1))
# done


# i="1"
# BLKSIZE="4096"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc2.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res39.txt
#     ./hpnmalloc3.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res39.txt
#     ./wamalloc1.out $TIMES $BLKSIZE $THNUM    >> ./exp_data/wa_res39.txt
#     ./nvmalloc1.out $TIMES $BLKSIZE $THNUM   >> ./exp_data/nvm_res39.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="10240"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#     ./hpnmalloc2.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn_res40.txt
#     ./hpnmalloc3.out $TIMES $BLKSIZE $THNUM  >> ./exp_data/hpn1_res40.txt
#     ./wamalloc1.out $TIMES $BLKSIZE $THNUM    >> ./exp_data/wa_res40.txt
#     ./nvmalloc1.out $TIMES $BLKSIZE $THNUM   >> ./exp_data/nvm_res40.txt
#     i=$(($i+1))
# done




i="39"
while [ $i -lt 41 ]
do 
    python3 ../static_analy.py 9 ./exp_data/hpn_res$(($i)).txt  ./exp_data/wa_res$(($i)).txt ./exp_data/nvm_res$(($i)).txt $(($i+50))
    i=$(($i+1))
done



# rm hpnmalloc.out
# rm hpnmalloc1.out
# rm hpnmalloc2.out
# rm hpnmalloc3.out
# rm wamalloc.out
# rm wamalloc1.out
# rm nvmalloc.out
# rm nvmalloc1.out
# rm addr_handle.out
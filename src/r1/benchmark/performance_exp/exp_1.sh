gcc -g -o hpnmalloc.out ../../HPN/hpnmalloc.c ../../HPN/wtcpa.c ./exp_code/exp_1.c -I ../../HPN/    -DHPNMALLOC -lpthread -lm
gcc -g -o wamalloc.out  ../../TCNVMalloc/src/tcnvmalloc.c ../../TCNVMalloc/src/wtcpa.c ../../TCNVMalloc/src/pq.c   ./exp_code/exp_1.c -I ../../TCNVMalloc/src/    -DWAMALLOC -lpthread -lm
gcc -g -o nvmalloc.out ../../nvmalloc/nvmalloc.c ./exp_code/exp_1.c   -I  ../../nvmalloc -DNVMALLOC -lpthread -lm
gcc -g -o cpma.out ../../CPMA/wtcpa.c ../../CPMA/cpma.c  ./exp_code/exp_1.c -I ../../CPMA/      -DCPMA -lpthread -lm

gcc -g -o hpnmalloc1.out ../../HPN/hpnmalloc.c ../../HPN/wtcpa.c ./exp_code/exp_2.c -I ../../HPN/    -DHPNMALLOC -lpthread -lm
gcc -g -o wamalloc1.out  ../../TCNVMalloc/src/tcnvmalloc.c ../../TCNVMalloc/src/wtcpa.c ../../TCNVMalloc/src/pq.c   ./exp_code/exp_2.c -I ../../TCNVMalloc/src/    -DWAMALLOC -lpthread -lm
gcc -g -o nvmalloc1.out ../../nvmalloc/nvmalloc.c ./exp_code/exp_2.c   -I  ../../nvmalloc -DNVMALLOC -lpthread -lm
gcc -g -o cpma1.out ../../CPMA/wtcpa.c ../../CPMA/cpma.c  ./exp_code/exp_2.c -I ../../CPMA/      -DCPMA -lpthread -lm

BASE="10000"



# i="1"
# BLKSIZE="8"
# THNUM="1"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res1.txt
#       ./wamalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res1.txt
#       ./nvmalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res1.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res1.txt
#     i=$(($i+1))
# done
# #     TIMES=$(($i*$BASE))
# #  ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res1.txt

# i="1"
# BLKSIZE="16"
# THNUM="1"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res2.txt
#       ./wamalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res2.txt
#       ./nvmalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res2.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res2.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="32"
# THNUM="1"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res3.txt
#       ./wamalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res3.txt
#       ./nvmalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res3.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res3.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="64"
# THNUM="1"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res4.txt
#       ./wamalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res4.txt
#       ./nvmalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res4.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res4.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="128"
# THNUM="1"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res5.txt
#       ./wamalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res5.txt
#       ./nvmalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res5.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res5.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="256"
# THNUM="1"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res6.txt
#       ./wamalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res6.txt
#       ./nvmalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res6.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res6.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="512"
# THNUM="1"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res7.txt
#       ./wamalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res7.txt
#       ./nvmalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res7.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res7.txt
#     i=$(($i+1))
# done




# i="1"
# BLKSIZE="512"
# THNUM="1"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc1.out $TIMES $BLKSIZE   $THNUM  >> ./exp_data/hpn_res8.txt
#       ./wamalloc1.out $TIMES  $BLKSIZE $THNUM >> ./exp_data/wa_res8.txt
#       ./nvmalloc1.out $TIMES  $BLKSIZE $THNUM >> ./exp_data/nvm_res8.txt
#       # ./cpma1.out  $TIMES  $THNUM >> ./exp_data/cpma_res8.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="4096"
# THNUM="1"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc1.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res9.txt
#       ./wamalloc1.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res9.txt
#       ./nvmalloc1.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res9.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res7.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="10240"
# THNUM="1"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc1.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res10.txt
#       ./wamalloc1.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res10.txt
#       ./nvmalloc1.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res10.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res7.txt
#     i=$(($i+1))
# done


i="10"
while [ $i -lt 11 ]
do 
    python3 ../static_analy.py 6 ./exp_data/hpn_res$(($i)).txt ./exp_data/wa_res$(($i)).txt ./exp_data/nvm_res$(($i)).txt  $i
    i=$(($i+1))
done



rm hpnmalloc.out
rm wamalloc.out
rm nvmalloc.out
rm cpma.out
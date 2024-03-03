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
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res9.txt
#       ./wamalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res9.txt
#       ./nvmalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res9.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res1.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="16"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res10.txt
#       ./wamalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res10.txt
#       ./nvmalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res10.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res2.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="32"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res11.txt
#       ./wamalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res11.txt
#       ./nvmalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res11.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res3.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="64"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res12.txt
#       ./wamalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res12.txt
#       ./nvmalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res12.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res4.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="128"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res13.txt
#       ./wamalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res13.txt
#       ./nvmalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res13.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res5.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="256"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res14.txt
#       ./wamalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res14.txt
#       ./nvmalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res14.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res6.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="512"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res15.txt
#       ./wamalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res15.txt
#       ./nvmalloc.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res15.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res7.txt
#     i=$(($i+1))
# done

# i="1"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc1.out $TIMES $BLKSIZE   $THNUM  >> ./exp_data/hpn_res16.txt
#       ./wamalloc1.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res16.txt
#       ./nvmalloc1.out $TIMES  $BLKSIZE $THNUM >> ./exp_data/nvm_res16.txt
#       # ./cpma1.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res8.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="4096"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc1.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res11.txt
#       ./wamalloc1.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res11.txt
#       ./nvmalloc1.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res11.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res7.txt
#     i=$(($i+1))
# done

# i="1"
# BLKSIZE="10240"
# THNUM="10"
# while [ $i -lt 11 ]
# do
#     TIMES=$(($i*$BASE))
#       ./hpnmalloc1.out $TIMES $BLKSIZE  $THNUM  >> ./exp_data/hpn_res12.txt
#       ./wamalloc1.out $TIMES $BLKSIZE $THNUM >> ./exp_data/wa_res12.txt
#       ./nvmalloc1.out $TIMES $BLKSIZE $THNUM >> ./exp_data/nvm_res12.txt
#       # ./cpma.out  $TIMES $BLKSIZE $THNUM >> ./exp_data/cpma_res7.txt
#     i=$(($i+1))
# done


i="11"
while [ $i -lt 13 ]
do 
    python3 ../static_analy.py 7 ./exp_data/hpn_res$(($i)).txt ./exp_data/wa_res$(($i)).txt ./exp_data/nvm_res$(($i)).txt  $(($i))
    i=$(($i+1))
done



rm hpnmalloc.out
rm wamalloc.out
rm nvmalloc.out
rm cpma.out
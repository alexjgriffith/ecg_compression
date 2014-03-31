g++ src/compare.c src/common_functions.c -o bin/compare.o
chmod +x ./bin/compare.o
echo -e "4096\t4096"> temp
for i in $(seq 1 100)
do
echo -e "4096\t4096" >> temp    
done

cat temp | ./bin/compare.o


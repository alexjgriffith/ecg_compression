
function test-fan
{
awk -F "\t" 'NR<2000{print $3}' data/aami.txt | bin/ecg_compression.o -m fan -e 3 > temp_c~ 
echo

awk -F "\t" 'NR<2000{print $2}' data/aami.txt | ./bin/ecg_compression.o -m fan -e 3 | ./bin/ecg_compression.o -m fan -d >temp_a~
awk -F "\t" 'NR<2000{print $2}' data/aami.txt > temp_b~
du -h temp_c~
du -h temp_b~
#echo
paste  temp_b~ temp_a~ > data/fan-results.txt

./python/graphing-compression.py data/fan-results.txt
#echo
}

function test-dpcm
{
awk -F "\t" 'NR<2001{print $2}' data/aami.txt | ./bin/ecg_compression.o -m dpcm >temp_a~
awk -F "\t" 'NR>1 && NR<2000{print $2}' data/aami.txt >temp_b~
paste temp_a~ temp_b~ > data/dpcm_2nd.txt
./python/graphing-compression.py data/dpcm_2nd.txt

}
function find-deletion
{
a=`awk '$1!=$2{print NR}' $1 | head -1`
 echo $a
 grep $a $2
}
function test-huffman
{
awk -F "\t" 'NR<20000{print $3}' data/aami.txt >temp_a~
awk -F "\t" 'NR<20000{print $3}' data/aami.txt |./bin/ecg_compression.o -m make-huffman>data/huffman_code.txt
awk -F "\t" 'NR<20000{print $3}' data/aami.txt |./bin/ecg_compression.o -m huffman -h data/huffman_code.txt >temp_b~
cat temp_b~ | ./bin/ecg_compression.o -m huffman -d -h data/huffman_code.txt >temp_c~
#du -h temp_a~
#du -h temp_b~
wc temp_a~
wc temp_c~
paste temp_a~ temp_c~ >temp_d~
}
#test-huffman
#find-deletion temp_d~ data/huffman_code.txt
#head data/huffman_code.txt

#awk -F "\t" 'NR<20000{print $3}' data/aami.txt |./bin/ecg_compression.o -m dpcm | awk 'NR>1 && NR<10000 {print $0}'>temp_a~
#awk -F "\t" 'NR>1 && NR<10000{print $3}' data/aami.txt >temp_b~
#paste temp_a~ temp_b~ >temp_c~
#python/graphing-compression.py temp_a~ 1

function test-difference
{
awk -F "\t" 'NR<20000{print $3}' data/aami.txt |./bin/ecg_compression.o -m dpcm | ./bin/ecg_compression.o -m dpcm -d > temp_a~
awk -F "\t" 'NR<20000{print $3}' data/aami.txt >temp_b~

paste temp_b~ temp_a~ >temp_c~
python/graphing-compression.py temp_c~ 
awk -F "\t" 'NR<20000{print $3}' data/aami.txt |./bin/ecg_compression.o -m dpcm |awk 'NR>1 {print $0}'>temp_b~
python/graphing-compression.py temp_b~ 1
}
./make.sh

cd ../
for a in `grep -w line ecg_compression/data/cbd_files.data|awk '{print $2}'| head -20 | tail -10 `
do
echo $a
rdsamp -r $a | awk -F "\t" '{print $2-$3}' |sed 's/\ //g' >ecg_compression/temp.data
cd ecg_compression/

awk -F "\t" 'NR<2000{print $1}' temp.data > temp_a~
awk -F "\t" 'NR<2000{print $1}' temp.data |./bin/ecg_compression.o -m spline > temp~
cat temp~ | ./bin/ecg_compression.o -m spline -d > temp_b~

paste temp_a~ temp_b~ >temp_c~
wc -l temp_a~
wc -l temp_b~
python/graphing-compression.py temp_c~
done
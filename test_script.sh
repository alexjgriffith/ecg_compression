
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
function test-huffman
{
awk -F "\t" 'NR>1 && NR<2000{print $2}' data/aami.txt |./bin/ecg_compression.o -m make-huffman>data/huffman_code.txt
}
test-huffman
head data/huffman_code.txt

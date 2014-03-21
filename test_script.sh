cat data/test.txt | bin/ecg_compression.o -m fan -e 0.2 
echo

cat data/test.txt | bin/ecg_compression.o -m fan -e 0.2 \
| bin/ecg_compression.o -m fan -d>temp~
#echo
paste data/test.txt temp~ > data/fan-results.txt

./python/graphing-compression.py data/fan-results.txt
#echo
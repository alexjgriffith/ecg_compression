function holding
{
g++ src/fourier.c src/common_functions.c -o bin/fourier.o
awk 'NR>50000 && NR<1400{print $1}' data/aami.txt > temp
cat temp| ./bin/fourier.o  >frequency
python python/graphing-compression.py frequency alt

python python/graphing-compression.py temp 1
}
#`grep -w line ecg_compression/data/cbd_files.data|awk '{print $2}' | head -1`
#a=www.physionet.org/physiobank/database/cdb/12621_01
#`ls www.physionet.org/physiobank/database/cdb/*.dat| sed 's/\(.*\)\..*/\1/'| head -200`
cd ../
grep -w line ecg_compression/data/cbd_files.data|wc -l
#cdb,aami-ec13 
for a in `grep -w line ecg_compression/data/cbd_files.data|awk '{print $2}' `
do
echo > ecg_compression/temp
echo>ecg_compression/start.data
echo > ecg_compression/fourier_components.data
echo $a
rdsamp -r $a | awk -F "\t" '{print $2-$3}' |sed 's/\ //g' >ecg_compression/temp.data
cd ecg_compression/

g++ src/rpeak.c src/fourier.c src/common_functions.c -o bin/rpeak.o
awk 'NR<9000 {print $1}' temp.data > temp
cat temp |./bin/rpeak.o >>start.data

./python/fourier_reconstruction.py fourier_components.data start.data temp.data >> descriptors.data
#python python/graphing-compression.py fourier.data alt
#python python/graphing-compression.py temp 1 #alt2 temp3 $a
#python python/graphing-compression.py temp 1
cd ../
done
cd ecg_compression/
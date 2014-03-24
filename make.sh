cd src
g++ -o ../bin/ecg_compression.o main.c fan.c  spline.c entropy.c common_functions.c
cd ../
chmod +x bin/ecg_compression.o
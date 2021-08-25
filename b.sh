#! /bin/sh
# echo "hello world"
make clean
make


#./KSP map/data_cal/sub/cal_sub_keys_10_120.node 5,92,64,12,85,45,78,23,89,7,32,99,102,114,107,109,25,88,11,19 testData/res.txt testData/out1.txt
./KSP map/data_cal/sub/cal_sub_keys_10_150.node 5,92,64,12,85,45,78,23,89,7,32,134,145,127,100,56,41,24,11,19 testData/res.txt testData/out1.txt
echo "=======================================================================">>testData/out1.txt

#./KSP map/data_cal/sub/cal_sub_keys_10_120.node 5,92,64,12,85,45,78,23,89,7,32,99,102,114,107 testData/res.txt testData/out1.txt
#./KSP map/data_cal/sub/cal_sub_keys_10_150.node 5,92,64,12,85,45,78,23,89,7,32,134,145,127,100 testData/res.txt testData/out1.txt
#echo "=======================================================================">>testData/out1.txt



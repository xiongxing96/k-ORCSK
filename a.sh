#! /bin/sh
# echo "hello world"
make clean
make

./KSP map/data_cal/sub/cal_sub_keys_10_100.node 5,92,64,12,85 testData/res.txt testData/out.txt
./KSP map/data_cal/sub/cal_sub_keys_10_120.node 5,92,64,12,85 testData/res.txt testData/out.txt
./KSP map/data_cal/sub/cal_sub_keys_10_150.node 5,92,64,12,85 testData/res.txt testData/out.txt
echo "=======================================================================">>testData/out.txt

./KSP map/data_cal/sub/cal_sub_keys_10_100.node 5,92,64,12,85,45,78 testData/res.txt testData/out.txt
./KSP map/data_cal/sub/cal_sub_keys_10_120.node 5,92,64,12,85,45,78 testData/res.txt testData/out.txt
./KSP map/data_cal/sub/cal_sub_keys_10_150.node 5,92,64,12,85,45,78 testData/res.txt testData/out.txt
echo "=======================================================================">>testData/out.txt

./KSP map/data_cal/sub/cal_sub_keys_10_100.node 5,92,64,12,85,45,78,23,89,7 testData/res.txt testData/out.txt
./KSP map/data_cal/sub/cal_sub_keys_10_120.node 5,92,64,12,85,45,78,23,89,7 testData/res.txt testData/out.txt
./KSP map/data_cal/sub/cal_sub_keys_10_150.node 5,92,64,12,85,45,78,23,89,7 testData/res.txt testData/out.txt
echo "=======================================================================">>testData/out.txt
echo "=======================================================================">>testData/out.txt

./KSP map/data_cal/sub/cal_sub_keys_5_100.node 5,92,64,12,85 testData/res.txt testData/out.txt
./KSP map/data_cal/sub/cal_sub_keys_10_100.node 5,92,64,12,85 testData/res.txt testData/out.txt
./KSP map/data_cal/sub/cal_sub_keys_20_100.node 5,92,64,12,85 testData/res.txt testData/out.txt
echo "=======================================================================">>testData/out.txt

./KSP map/data_cal/sub/cal_sub_keys_5_100.node 5,92,64,12,85,45,78 testData/res.txt testData/out.txt
./KSP map/data_cal/sub/cal_sub_keys_10_100.node 5,92,64,12,85,45,78 testData/res.txt testData/out.txt
./KSP map/data_cal/sub/cal_sub_keys_20_100.node 5,92,64,12,85,45,78 testData/res.txt testData/out.txt
echo "=======================================================================">>testData/out.txt

./KSP map/data_cal/sub/cal_sub_keys_5_100.node 5,92,64,12,85,45,78,23,89,7 testData/res.txt testData/out.txt
./KSP map/data_cal/sub/cal_sub_keys_10_100.node 5,92,64,12,85,45,78,23,89,7 testData/res.txt testData/out.txt
./KSP map/data_cal/sub/cal_sub_keys_20_100.node 5,92,64,12,85,45,78,23,89,7 testData/res.txt testData/out.txt

echo "=======================================================================">>testData/out.txt


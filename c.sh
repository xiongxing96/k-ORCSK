#! /bin/sh
# echo "hello world"
make clean
make


#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 5,12 testData/rest.txt testData/outcal.txt
#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 12,44,17,35 testData/rest.txt testData/outcal.txt
#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 5,92,64,12,85,78 testData/rest.txt testData/outcal.txt
#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 15,32,44,13,85,45,78,93 testData/rest.txt testData/outcal.txt
#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 15,32,44,13,85,45,78,93,55,21 testData/rest.txt testData/outcal.txt
#echo "=======================================================================">>testData/outcal.txt

#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 5,92,64,12,85,78 testData/calforlen.txt testData/outcal.txt
#echo "============================cal for len===========================================">>testData/outcal.txt

#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 5,92,64,12,85,78 testData/calforlen.txt testData/outcal.txt 10
#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 5,92,64,12,85,78 testData/calforlen.txt testData/outcal.txt 20
#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 5,92,64,12,85,78 testData/calforlen.txt testData/outcal.txt 30
#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 5,92,64,12,85,78 testData/calforlen.txt testData/outcal.txt 40
#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 5,92,64,12,85,78 testData/calforlen.txt testData/outcal.txt 50
#echo "============================cal for k ===========================================">>testData/outcal.txt

#echo "============================cal for route size DAPrune===========================================">>testData/outcal.txt
#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 5,92,64,12,85,78 testData/calforlen.txt testData/outcal.txt 30
#echo "============================cal for route size DA===========================================">>testData/outcal.txt

#echo "==================================compare with KOSR=====================================">>testData/outcal.txt
#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 5,12 testData/rest.txt testData/outcal.txt 30
#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 12,44,17,35 testData/rest.txt testData/outcal.txt 30
#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 5,92,64,12,85,78 testData/rest.txt testData/outcal.txt 30
#./KSP map/data_cal/sub/cal_sub_keys_5_100.node 15,32,44,13,85,45,78,93 testData/rest.txt testData/outcal.txt 30
./KSP map/data_cal/sub/cal_sub_keys_5_100.node 15,32,44,13,85,45,78,93,55,21 testData/rest.txt testData/outcal.txt 30

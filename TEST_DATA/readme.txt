Prior the test you need to generate data files

./test_file w s TEST_DATA/data.001.txt TEST_DATA.BIN/data.001.db
./test_file w s TEST_DATA/data.002.txt TEST_DATA.BIN/data.002.db
./test_file w s TEST_DATA/data.003.txt TEST_DATA.BIN/data.003.db
./test_file w s TEST_DATA/data.004.txt TEST_DATA.BIN/data.004.db

Then you can do tests such:

./db_lsm l TEST_DATA.BIN/data.'*'.bin -


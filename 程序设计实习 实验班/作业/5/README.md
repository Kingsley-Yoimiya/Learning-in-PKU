例如你的文件为rank.cpp，我们下发grader_test.cpp和rank.h，利用如下命令行编译：
g++ grader_test.cpp rank.cpp -o rank -O2 -lm

设输入文件为1.in用如下命令行运行：
./rank < 1.in

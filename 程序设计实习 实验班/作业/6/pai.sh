fname=6
bname=6_bf

g++-13 $fname.cpp -o $fname.out
g++-13 $bname.cpp -o $bname.out
g++-13 maker.cpp -o maker.out
g++-13 checker.cpp -o checker.out
for i in `seq 1 1000`;do
    ./maker.out 10000 10000
    ./$fname.out
    ./$bname.out
    if ./checker.out ;
    then 
        echo "AC";
    else
        echo "WA"
        break
    fi
done
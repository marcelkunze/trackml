 rm graph/* paths/*
 make clean; make; make makeGraph
./makeGraph 21100 15 3 $2 $3
./makeGraph 21002 $1 1 $2 $3
time ./tracker 21001 1 $2 $3 | tee out.$1-$2-$3
cp -pr graph graph.$1-$2-$3
cp -pr paths paths.$1-$2-$3

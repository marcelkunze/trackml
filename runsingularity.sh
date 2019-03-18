singularity exec docker://estradevictorantoine/trackml:1.0 /bin/bash -c "cd $PWD;make clean;make eval;exit"
cd run
rm graph/* paths/* xmlp/*
cp ../cloudkitchen.zip .
unzip -o cloudkitchen.zip
./runs.sh
echo -ne '\007'

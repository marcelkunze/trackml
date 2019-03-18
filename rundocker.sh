docker run -it -v$(pwd):/home/my_working_directory  --rm estradevictorantoine/trackml:1.0 /bin/bash -c "cd /home/my_working_directory;make clean;make eval;exit"
cd run
rm graph/* paths/* xmlp/*
cp ../cloudkitchen.zip .
unzip -o cloudkitchen.zip
./run.sh
echo -ne '\007'

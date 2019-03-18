
INPUT_DATA="$HOME/workspace/training_000021450_000021499"

docker run -i --rm \
	-v $(pwd):/home/code \
	-v $INPUT_DATA:/home/data \
	estradevictorantoine/trackml:1.0 \
	/bin/sh -c "cd /home/code; python main.py $*"

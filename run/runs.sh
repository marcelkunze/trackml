
INPUT_DATA="$HOME/workspace/training_000021450_000021499"

singularity exec --bind $INPUT_DATA:/home/data docker://estradevictorantoine/trackml:1.0 python $PWD/main.py


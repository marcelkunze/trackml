Below you can find a outline of how to reproduce my solution for the CodaLab trackML throughput competition.
If you run into any trouble with the setup/code or have any questions please contact me at marcel@cloudkitchen.info.

# A multi threaded global tracking algorithm based on directed acyclic graphs and machine learning

The algorithm has been designed to make use of artificial neural networks for pattern recognition on the basis of spatial coordinates together with simple geometrical information such as directional cosines or a helix score calculation. Typical patterns to be investigated are hit pairs and triples that might seed candidate tracks. The training of the networks was accomplished by presentation of typically 5 million ground truth patterns over 500 epochs.
The hit data are sorted into voxels organized in directed acyclic graphs (DAG) in order to allow for fast track propagation. In addition to the spatial hit data the DAGs hold information about the network model to apply, and a z vertex estimate derived from ground truth. As they combine the data with the corresponding methods the DAGs form a nice foundation to define tasks that can be run in parallel very efficiently in a multi threaded environment. There are two sets of graphs: One set covers detector slices wrt. the z-axis, the other covers phi/theta tiles. Each set would work perfectly well by itself, but a clever combination of the two yields the best CodaLab score: The first set is used to seed the pair finder while the other drives the triple finder (COMBINEDMETHOD).

## ARCHIVE CONTENTS
    *.cxx, *.h   : source files
    Makefile     : build the model
    paths, graph : DAGs
    xmlp         : neural networks
    doc          : documentation

## HARDWARE
    Ubuntu 18.04 LTS (256 GB SSD)
    Intel NUC 7I7BNH (16 GB RAM)

## SOFTWARE
* C++11 with STL
* [Neural Network Objects](https://github.com/marcelkunze/rhonno) for training
* [Root framework](https://root.cern.ch/) for training

## DATA SETUP
    git clone https://github.com/marcelkunze/trackml.git
    cd trackml
    make

## DATA PROCESSING
Parameters and operational modes of the model are centrally defined in _Parameters.h_. The _DATAPATH_, the _WORKPATH_, and the _TRAINPATH_ need to be adapted to fit the local installation.

    ./tracker 21001

Processing is generally done in a container environment using a 50 events data set (training_000021450_000021499). The model can be tested by running it in a docker container:

    ./rundocker.sh

This also produces a zip-file of all files to be submitted to CodaLab.

The model works with [singularity](https://www.sylabs.io/) containers as well:

    ./runsingularity.sh

The scripts need to be modified to define the *INPUT_DATA* path to locate the data set.

## MODEL BUILD
The DAGs reside in the _graph_ (tiles) and _paths_ (slices) directories, respectively. The training of the DAGs is performed by

    ./makeGraph [ event number mode phires theres ]

* event - event to process (default: 21001)
* number - number of events in sequence (default: 1)
* mode - 1 for tile graphs, 3 for slice graphs
* phires - phi resolution (default: 12)
* theres - theta resolution (default: 14)

Graph generation takes about half a minute for 25 events. There is a script to prepare a suiting set of graphs:

    ./generate.sh 25 12 14

where the argument denotes the number of events to use for training and the phi and theta resolution, respectively.

There are three neural networks for pattern recognition:

* XMLP1 for hit pair recognition (8-15-5-1 multilayer perceptron), good esp. in forward/backward detector region
* XMLP2 for hit pair recognition (9-15-5-1 multilayer perceptron), good esp. in central detector region
* XMLP3 for hit triple recognition (10-15-5-1 multilayer perceptron), good everywhere

The neural networks are trained with ntuples stored in [root files](https://root.cern.ch/). These are produced if the flag *TRAINFILE* is set in the programs. The training has been performed with the NetworkTrainer of the [Neural Network Objects](https://github.com/marcelkunze/rhonno) by use of the corresponding training files, e.g.

    NetworkTrainer train1.nno
    NetworkTrainer train2.nno
    NetworkTrainer train3.nno

The supervised training takes about half an hour for 500 epochs of 4-5 million patterns each (extracted from ca. 10 events). For each epoch the corresponding network files are stored for in the _Networks_ directory. 

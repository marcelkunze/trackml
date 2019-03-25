#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_
// Neural Network based tracker
// M.Kunze, Heidelberg University, 2018

// Reconstruction

#define FOLDEDINPUT1
#define FOLDEDINPUT2
#define STARTLAYERS 30
#define COMBINEDMETHOD
#define PHIRESOLUTION 12
#define THETARESOLUTION 14
#define SWEIGHT    1.5
#define VERTEXCUTXY  5.
#define VERTEXCUTZ  165.
#define VERTEXSCORE 130.
#define VERTEXCUT 165.

// Tracker

//#define EVAL
#define MULTITHREAD true
#define SLICES
#define NSLICES 2
#define NTHREADS 2

#define TILESDIR "graph"
#define SLICEDIR "paths"
#define XMLP1DIR "xmlp"
#define XMLP2DIR "xmlp"
#define XMLP3DIR "xmlp"

#define NETFILE1 "XMLP1.net"
#define NETFILE2 "XMLP2.net"
#define NETFILE3 "XMLP3.net"

#define THRESHOLD1 0.15
#define THRESHOLD2 0.55
#define THRESHOLD3 0.75

#define LAYERS 48
#define MAXDIM 150000

#ifdef _WIN32
#define TRAINPATH "C:/Users/marce/workspace/train_xmlp"
#define DATAPATH "C:/Users/marce/workspace/train_sample"
#define WORKPATH "C:/Users/marce/workspace/trackml"
#else
#define TRAINPATH "/Users/marcel/workspace/train_xmlp"
#define DATAPATH "/Users/marcel/workspace/train_sample"
#define WORKPATH "/Users/marcel/workspace/trackml"
#define MEMORY
#endif

#endif

// Neural Network Tracker
// Generate graphs for tracking
// M.Kunze, Heidelberg University, 2018

#define FILENUM 21001
#define TRAINFILE false
#define PATHSTESTDIR "paths"
#define GRAPHTESTDIR "graph"
#define TRAINDIR "test"

#include <iostream>
#include "TNtuple.h"
#include "TString.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TView.h"
#include "TPolyMarker3D.h"
#include "TAxis3D.h"
#include "TPolyLine3D.h"
#include "Trainer.h"
#include "Reconstruction.h"

using namespace std;

int main(int argc, char**argv) {
    //Read which event to run from arguments, default is event # 21100
    bool eval = false;
    int filenum = FILENUM;
    int number = 1;
    int step = 0;
    int pr = PHIRESOLUTION;
    int tr = THETARESOLUTION;
    double threshold2 = THRESHOLD2;
    double threshold3 = THRESHOLD3;

    if (argc >= 2) {
        filenum = atoi(argv[1]);
    }
    
    if (argc >= 3) {
        number = atoi(argv[2]);
    }
    
    if (argc >= 4) {
        step = atoi(argv[3]);
    }
    
    if (argc >= 5) {
        pr = atoi(argv[4]);
    }
    
    if (argc >= 6) {
        tr = atoi(argv[5]);
    }
    
    if (argc >= 7) {
        threshold2 = atof(argv[6]);
    }
    
    if (argc >= 8) {
        threshold3 = atof(argv[7]);
    }

    ios::sync_with_stdio(false);
    cout << fixed;

    Reconstruction::setPhiResolution(pr);
    Reconstruction::setThetaResolution(tr);
    
    for (int n=0;n<number;n++,filenum++) {
        
        cout << "Running on event #" << filenum << endl;
        cout << "Step   " << step << endl;
        cout << "Phi    " << Reconstruction::phiResolution() << endl;
        cout << "Theta  " << Reconstruction::thetaResolution() << endl;

        Trainer *tracker = new Trainer(filenum,DATAPATH,WORKPATH);

        if (!eval) {
            tracker->readStarts();
            tracker->readTruth();
            tracker->sortTracks();
        }
        
        tracker->readHits();
        tracker->readCells();
        tracker->setEvaluation(eval);
        tracker->setThreshold2(threshold2);
        tracker->setThreshold3(threshold3);
        
        if (step!=2) {
            cout << "Reading paths" << endl;
            tracker->readTilesGraph();
        }
        
        if (!eval&&step>1) {
            cout << "Generating paths" << endl;
            tracker->generateTilesGraph();
        }
        
        if (!eval&&step>2) {
            cout << "Writing paths" << endl;
            tracker->writeTilesGraph();
        }

        string filename = string(WORKPATH) + "/" + GRAPHTESTDIR;
        if (step>0&&step<=2) tracker->generateTrackingGraph(filename.c_str());
        
        long nhits(0), ncells(0);
        nhits  = tracker->initGraphData();
        ncells = tracker->initHitDir();
        cout << "initGraphData: " << nhits << "  initHitDir: " << ncells << endl;

        tracker->initPolarModule();

        // Generate a training sample for hit pairs and triples
        if (TRAINFILE) {
            stringstream ss;
            ss << WORKPATH << "/" << TRAINDIR;
            tracker->generateTrainingData(ss.str().c_str(),filenum);
        }

        delete tracker;
    }
    
}

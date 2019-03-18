// Neural Network Tracker
// Read the trackml data files
// Run various track reconstruction methods (Tree, DAG, Swimmer)
// M.Kunze, Heidelberg University, 2018

#include "Tracker.h"
#include <iostream>

#define VERBOSE false
#define MAXTRACK 10
#define FILENUM 21001

using namespace std;

int main(int argc, char**argv) {
    //Read which event to run from arguments, default is event # 21100
    int filenum = FILENUM;
    int number = 1;
    int step = 1;

    if (argc >= 2) {
        filenum = atoi(argv[1]);
    }
    
    if (argc >= 3) {
        number = atoi(argv[2]);
        cout << "number " << step << endl;
    }
    
    if (argc >= 4) {
        step = atoi(argv[3]);
        cout << "step " << step << endl;
    }
    
    ios::sync_with_stdio(false);
    cout << fixed;
    
    for (int n=0;n<number;n++,filenum++) {
        
        cout << "Running on event #" << filenum << endl;
        cout << "Step " << step << endl;

        Tracker *tracker = new Tracker(filenum,DATAPATH,WORKPATH);
        
        if (!tracker->evaluation()) {
            //tracker->readBlacklist();
            //tracker->readWhitelist();
            tracker->readStarts();
            tracker->readTruth();
            tracker->sortTracks();
        }
        
        tracker->readHits();
        tracker->readCells();
        
        tracker->setDebug(VERBOSE);
        int *assignment = tracker->findTracks(step,WORKPATH);

        // Assemble tracks
        cout << "Assembling tracks..." << endl;
        map<int,vector<int> > tracks;

        long nhits = tracker->numberHits();
        int is = VERBOSE ? 0 : 1; // track 0 holds the unassigned points
        for(int i=is;i<nhits;i++) {
            int track = assignment[i];
            tracks[track].push_back(i);
        }
        
        long nt = tracks.size();
        cout << endl << "Number of tracks: " <<nt << endl;
/*
        for (auto it : tracks) {
            if (it.first<=0) continue; // Holds unassigned points
            auto track = it.second;
            if (track.size() == 0) continue;
            if (it.first<MAXTRACK || it.first>nt-MAXTRACK) {
                cout << "Track " << it.first << ": ";
                //for (auto it : track) cout << it << "/" << tracker->shortid(it) << "(" << tracker->truthPart(it)%nhits << ") ";
                tracker->printshort(track);
            }
            if (it.first == MAXTRACK) cout << endl << "..." << endl;
        }
*/
        delete tracker;
    }
    
}


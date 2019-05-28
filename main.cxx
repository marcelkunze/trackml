// Neural Network Tracker
// Driver to run trackml program
// Requires the ROOT framework for training and drawing options
//
// tracker [ event number phires theres threshold1 threshold2 ]
// Options:
// event - event to process (default: 21001)
// number - number of events in sequence (default: 1)
// phires - phi resolution (default: 12)
// theres - theta resolution (default: 14)
// threshold1 - cut on XMLP1 (default: 0.15)
// threshold2 - cut on XMLP2 (default: 0.15)
//
// M.Kunze, Heidelberg University, 2018

#include "Trainer.h"
#include "Reconstruction.h"

//#define TRAINFILE
//#define DRAW
#define FILENUM 21001
#define MAXTRACK 50
#define MINHITS 0
#define MAXHITS 150000

#include <iostream>

using namespace std;

int main(int argc, char**argv) {
    //Read which event to run from arguments, default is event # 21100
    bool eval = false;
    int filenum = FILENUM;
    int number = 1;
    int pr = PHIRESOLUTION;
    int tr = THETARESOLUTION;
    double threshold2 = THRESHOLD2;
    double threshold3 = THRESHOLD3;
    
    if (argc >= 2) {
        filenum = atoi(argv[1]);
    }
    
    if (argc >= 3) {
        number = atoi(argv[2]);
        cout << "Events " << number << endl;
    }
    
    if (argc >= 4) {
        pr = atoi(argv[3]);
    }
    
    if (argc >= 5) {
        tr = atoi(argv[4]);
    }
    
    if (argc >= 6) {
        threshold2 = atof(argv[5]);
    }
    
    if (argc >= 7) {
        threshold3 = atof(argv[6]);
    }
    
    ios::sync_with_stdio(false);
    cout << fixed;
    
    double score = 0.0;
    int nprocessed = 0;
    
    Reconstruction::setPhiResolution(pr);
    Reconstruction::setThetaResolution(tr);
    
    for (int n=0;n<number;n++,filenum++) {
        
        cout << endl << "Running on event #" << filenum << endl;
        
        Trainer *tracker = new Trainer(filenum,DATAPATH,WORKPATH);
        
        if (!eval) {
            //tracker->readBlacklist();
            //tracker->readWhitelist();
            tracker->readStarts();
            tracker->readTruth();
            tracker->sortTracks();
        }
        
        tracker->readHits();
        tracker->readCells();
        tracker->setEvaluation(eval);
        tracker->setThreshold2(threshold2);
        tracker->setThreshold3(threshold3);
        
        long nhits = tracker->getNumberHits();
        if (nhits<MINHITS || nhits>MAXHITS) continue;
        
        int *assignment = tracker->findTracks(1);
        
        // Assemble tracks
        cout << "Assembling tracks..." << endl;
        map<int,vector<int> > tracks;
        
        for(int i=0;i<nhits;i++) {
            int track = assignment[i];
            tracks[track].push_back(i);
        }
        
        long nt = tracks.size();
        cout << "Number of tracks: " <<nt << endl;
        
        cout << "Score: " << tracker->getScore() << endl;
        score += tracker->getScore();
        nprocessed++;
        
#ifdef TRAINFILE
        // Generate a training sample for hit pairs and triples
        tracker->generateTrainingData(TRAINPATH, filenum);
#endif
        
#ifdef DRAW
        // Show the results in a canvas
        tracker->draw(MAXTRACK,tracker->getHits(),tracks);
#endif
        
        delete tracker;
    }
    
    cout << endl << "events: " << nprocessed << "  average score: " << score/nprocessed << endl << endl;

    return 0;
}

#ifdef DRAW

#include "TNtuple.h"
#include "TString.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TView.h"
#include "TPolyMarker3D.h"
#include "TAxis3D.h"
#include "TPolyLine3D.h"

void Tracker::draw(unsigned long nt,vector<point> &hits,map<int,vector<int> > &tracks)
{
    // Initialize a 3D canvas and draw the hits and tracks
    TString dir = DATAPATH;
    TString filePrefix;
    filePrefix.Form("%s/event%09d",dir.Data(),filenum);
    TString cname = filePrefix+"-canvas.root";
    TFile output(cname,"RECREATE");
    TCanvas *c1 = new TCanvas("c1","NNO Tracking: XMLP",200,10,700,500);
    // create a pad
    TPad *p1 = new TPad("p1","p1",0.05,0.02,0.95,0.82,46,3,1);
    p1->SetFillColor(kBlack);
    p1->Draw();
    p1->cd();
    // creating a view
    TView *view = TView::CreateView(1);
    view->SetRange(-2000,-2000,-2000,2000,2000,2000); // draw in a 2 meter cube
    view->FrontView();
    // Draw axis
    TAxis3D rulers;
    rulers.Draw();
    // draw hits as PolyMarker3D
    TPolyMarker3D *hitmarker = new TPolyMarker3D((unsigned int) hits.size());
    int n=0;
    for (unsigned int i=1;i<nt;i++) {
        vector<int> track = tracks[i];
        for (auto it : track) {
            hitmarker->SetPoint(n++,hits[it].x,hits[it].y,hits[it].z);
        }
    }
    // set marker size, color & style
    hitmarker->SetMarkerSize(1.0);
    hitmarker->SetMarkerColor(kCyan);
    hitmarker->SetMarkerStyle(kStar);
    hitmarker->Draw();
    
    // Draw the tracks
    if (nt > tracks.size()) nt = tracks.size();
    for (unsigned int i=1;i<nt;i++) {
        //cout << endl << "Drawing track " << i+1 << ": ";
        vector<int> track = tracks[i];
        vector<point> p;
        for (auto it : track) p.push_back(hits[it]);
        sort(p.begin(),p.end(),point::sortByRadius);
        int n = 0;
        TPolyLine3D *connector = new TPolyLine3D((int)track.size());
        for (auto &it : p)    {
            connector->SetPoint(n++, it.x, it.y, it.z);
        }
        connector->SetLineWidth(1);
        connector->SetLineColor(kRed);
        connector->Draw();
    }
    
    cout <<  "Saving canvas file " << cname << endl;
    c1->Write();
    output.Close();
    
}
#endif

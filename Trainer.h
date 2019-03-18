#ifndef _TRAINER_H_
#define _TRAINER_H_
// Neural Network based training
// M.Kunze, Heidelberg University, 2018

#include "Tracker.h"

class Trainer  : public Tracker  {
    
public:
    Trainer(int n=21001,const char *datapath=DATAPATH,const char *workpath=WORKPATH) : Tracker(n,datapath,workpath) {};
    ~Trainer() {};

    void generateTrainingData(const char *base_path,int filenum);
    void transform(Particle &particle, std::vector<point> &points);
    std::vector<triple>  makeTrainPairs(int ir, Graph<long long> &g,TNtuple &nt);
    void makeTrainPairsGraph(int ir,Graph<long long> &g,TNtuple &nt);
    void makeTrain3(int ir,std::vector<triple> triples,TNtuple &nt);
    void makeTrainTriples(Graph<long long> &g,TNtuple &nt);
    void makeTrainTriplesGraph(Graph<long long> &g,TNtuple &nt);
    long addHits(int ir,std::vector<triple> &triples,int ai,int bi,Graph<long long> &g);

private:
    
};

#endif

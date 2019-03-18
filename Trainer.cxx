// Neural Network based tracker
// M.Kunze, Heidelberg University, 2018

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "TFile.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "Tracker.h"
#include "Trainer.h"
#include "Reconstruction.h"
#include "Graph.h"
#include "load.h"
#include "parallel.h"

using namespace std;

TRandom r;

#define PARALLEL_FOR_BEGIN(nb_elements) parallel_for(nb_elements, [&](int start, int end){ for(int i = start; i < end; ++i)
#define PARALLEL_FOR_END()},MULTITHREAD)

void Trainer::transform(Particle &particle, std::vector<point> &points) {
    
    vector<point> tmpvec;
    long nhits = (long)particle.hit.size();
    static int trackid = 0;
    
    trackid++;
    
    for (int i=0;i<nhits;i++) {
        vector<int> &h = particle.hit;
        int id = h[i];
        point h1 = hits[id]; // in mm
        point p(h1.x,h1.y,h1.z);
        tmpvec.push_back(p);
    }
    
    sort(tmpvec.begin(),tmpvec.end(),point::sortByRz);
    points.insert(points.end(),tmpvec.begin(),tmpvec.end());
}


// Generate a training sample for hit pairs and triples
void Trainer::generateTrainingData(const char *base_path,int filenum) {
    const string tuple2("rz1:phi1:z1:rz2:phi2:z2:f0:f1:f2:f3:f4:f5:score:l1:l2:vx:vy:vz:r1:r2:truth");
    const string tuple3("rz1:phi1:z1:rz2:phi2:z2:rz3:phi3:z3:score:l1:l2:l3:vx:vy:vz:r1:r2:r3:truth");
    TNtuple *ntuple2 = new TNtuple("tracks2","training data",tuple2.c_str());
    TNtuple *ntuple3 = new TNtuple("tracks3","training data",tuple3.c_str());
    TString filePrefix;
    filePrefix.Form("%s/event%09d",base_path,filenum);
    TString fname = filePrefix+".root";
    auto f = TFile::Open(fname,"RECREATE");
    cout << endl << "Generating training data file " << fname << endl;
    // Instantiate reconstruction objects for parallel processing
    static const int n = NTHREADS;
    initRecoObjects(n);
    initTasks();
    
    static mutex mylock;
    PARALLEL_FOR_BEGIN(n)
    {
        while (!tasks.empty()) {
            // get a task from the stack (beware of race hazards)
            mylock.lock();
            if (tasks.empty()) break;
            auto *g = tasks.top().second;
            tasks.pop();
            auto triples3  = makeTrainPairs(i,*g,*ntuple2);
            makeTrain3(i,triples3,*ntuple3);
            mylock.unlock();
        }
    }
    PARALLEL_FOR_END();

    ntuple2->Write();
    ntuple3->Write();
    delete ntuple2;
    delete ntuple3;
    f->Close();
    for (int i=0;i<n;i++) delete reco[i];
}


// Look for seeding points by hit pair combinations in the innnermost layers
std::vector<triple>  Trainer::makeTrainPairs(int ir, Graph<long long> &g,TNtuple &nt)
{
    long right=1,wrong=1;
    static const int n=15; // Seeding layer combinations
    static const int start_layer[48] = {0,11,4,18,1,5,12,13,6,2,3,19,20,7,14,21,24,36,15,8,22,9,16,38,40,42,26,28,30,25,37,10,17,23,32,34,44,46,27,39,29,41,31,43,33,45,35,47};

    vector<pair<int, int> > pairs;
    vector<triple> triples;
    
    for (int i = 0; i < n; i++)
    {
        if (!eval&&!debug) load(n, "makeTrainPairs");
        int layer1 = start_layer[i];
        
        for (auto start : g.hash(layer1)) { // all modules in first layer
            const auto &edgelist = g.edges(start);
            if (edgelist.size() == 0) continue;
            
            for (auto &edge : edgelist) {
                long nextindex = edge.first;
                double z0 = edge.second;
                
                for (auto ai : g.data(start)) { // all hits in module
                    // Generate seeding points
                    vector<pair<int,float> > seed;
                    for (auto bi : g.data(nextindex)) { // all hits in following modules
                        //if (metai[ai] == metai[bi]) continue; // Same layer (Double hits)
/*
                        double xy0 = reco[ir]->xyVertex(ai,bi); // check the radial distance from origin
                        if (i<3 && xy0 > VERTEXCUTXY) continue;
                        double z0 = reco[ir]->zVertex(ai,bi); // check the z distance from origin
                        if (z0 > VERTEXCUTZ) continue;
                        double zdist = reco[ir]->zdist2(hits[ai],hits[bi]);
                        if (zdist > VERTEXCUTZ) continue;
                        hits[0] = point(0,0,0); // origin
                        double score = reco[ir]->scoreTriple(0,ai,bi); // helix score wrt. origin
                        if (score > vertexscore[i]) continue;
*/
/*
                        point &pa = hits[ai];
                        point &pb = hits[bi];
                        double dot = pa.x*pb.x+pa.y*pb.y;
                        double alen = dist2(pa.x, pa.y);
                        double blen = dist2(pb.x, pb.y);
                        if (dot < 0 || dot*dot < alen*blen*(.7*.7)) continue;
                        
                        dot += pa.z*pb.z;
                        alen += pa.z*pa.z;
                        blen += pb.z*pb.z;
                        if (dot < 0 || dot*dot < alen*blen*(.7*.7)) continue;
*/
                        int gp = good_pair(ai, bi);
                        
                        if (gp<0) continue;
                        if (gp==0 && r.Rndm()>0.25*right/wrong) continue;
                        right += gp!=0;
                        wrong  += gp==0;
                        
                        float l1 = metai[ai];
                        float l2 = metai[bi];
                        
                        point &a = polar[ai];
                        point &b = polar[bi];
                        point &v = start_pos[truth_part[ai]];
                        float vx = v.x;
                        float vy = v.y;
                        float vz = v.z;

                        // pair recall
                        float f[7];
                        point vertex(0,0,z0);
                        reco[ir]->getFeatures(ai,bi,f,vertex);
                        float r1 = reco[ir]->recall1(g,ai,bi,f[0],f[1]);
                        float r2 = reco[ir]->recall2(g,ai,bi,f[0],f[1],0.001*f[6]);

                        float x[21]={(float)a.x,(float)a.y,(float)a.z,(float)b.x,(float)b.y,(float)b.z,f[0],f[1],f[2],f[3],f[4],f[5],f[6],l1,l2,vx,vy,vz,r1,r2,(float)gp};
                        nt.Fill(x);
                                                
#ifdef COMBINEDMETHOD
                        pair<int,int> index = graphHash(bi);
                        addHits(ir,triples,ai,bi,tgraph[index]);
#else
                        addHits(ir,triples,ai,bi,g);
#endif
                    }
                    
                }
            }
        }
    }
    cout << "makeTrainPairs right: " << right << " wrong: " << wrong << " triples: " << triples.size() << endl;
    return triples;
}


// Generate tracklets of 3 points wrt. the first point in seed
long Trainer::addHits(int ir,vector<triple> &triples,int ai,int bi,Graph<long long> &g)
{
    long index = voxel(bi);
    
    const auto &edgelist = g.edges(index);
    if (edgelist.size() == 0) return 0;
    
    for (auto &edge : edgelist) {
        long nextindex = edge.first;
        double vz = edge.second;
        
        auto &p = g.data(nextindex); // all hits in following modules
        if (p.size() == 0) continue;
        
        vector<triple> v;
        for (auto ci:p)
        {
            if (ai==bi || ci==ai || ci==bi) continue; // Same hit

            // triple recall
            double score = reco[ir]->scoreTriple(ai,bi,ci);
            float r3 = reco[ir]->recall3(g,ai,bi,ci,score);

            triple t(ai,bi,ci,r3,vz);
            triples.push_back(t);
        }
    }
    
    return triples.size();
}


// Look for seeding points by hit pair combinations in the innnermost layers
void Trainer::makeTrainPairsGraph(int ir,Graph<long long> &g,TNtuple &nt)
{
    long right=1,wrong=1;
    static const int n=15; // Seeding layer combinations
    static const int start_layer[48] = {0,11,4,18,1,5,12,13,6,2,3,19,20,7,14,21,24,36,15,8,22,9,16,38,40,42,26,28,30,25,37,10,17,23,32,34,44,46,27,39,29,41,31,43,33,45,35,47};

    vector<pair<int, int> > pairs;
    
    for (int i = 0; i < n; i++)
    {
        if (!eval&&!debug) load(n, "makeTrainPairsGraph");
        int layer1 = start_layer[i];
        
        for (auto start : g.hash(layer1)) { // all modules in first layer
            const auto &edgelist = g.edges(start);
            if (edgelist.size() == 0) continue;
            
            for (auto &edge : edgelist) {
                long nextindex = edge.first;
                
                for (auto ai : g.data(start)) { // all hits in module
                    // Generate seeding points
                    for (auto bi : g.data(nextindex)) { // all hits in following modules
                        if (ai==bi) continue;
                        point &a = polar[ai];
                        point &b = polar[bi];
                        float l1 = metai[ai];
                        float l2 = metai[bi];
                        point &v = start_pos[truth_part[ai]];
                        float vx = v.x;
                        float vy = v.y;
                        float vz = v.z;
                        
                        // pair recall
                        float f[7];
                        reco[ir]->getFeatures(ai,bi,f,v);
                        float r1 = reco[ir]->recall1(g,ai,bi,f[0],f[1]);
                        float r2 = reco[ir]->recall2(g,ai,bi,f[0],f[1],0.001*f[6]);

                        float x[21]={(float)a.x,(float)a.y,(float)a.z,(float)b.x,(float)b.y, (float)b.z,f[0],f[1],f[2],f[3],f[4],f[5],f[6],l1,l2,vx,vy,vz,r1,r2,1.0};
                        if (samepart(ai, bi)) {
                           nt.Fill(x); // right combination
                            right++;
                        }
                        else {
                            if (r.Rndm()<right/wrong) {
                                float x[21]={(float)a.x,(float)a.y,(float)a.z,(float)b.x,(float)b.y,(float)b.z,f[0],f[1],f[2],f[3],f[4],f[5],f[6],l1,l2,vx,vy,vz,r1,r2,0.0};
                                nt.Fill(x); // wrong combination
                                wrong++;
                            }
                        }
                    }
                }
            }
        }
    }
    cout << "makeTrainPairsGraph right: " << right << " wrong: " << wrong << endl;
}


// Look for seeding points by hit pair combinations in the innnermost layers
void Trainer::makeTrain3(int ir,vector<triple> triples,TNtuple &nt)
{
    long right=1,wrong=1;
    for (auto t : triples) {
        if (!eval&&!debug) load(triples.size(), "makeTrain3");
        int ai = get<0>(t);
        int bi = get<1>(t);
        int ci = get<2>(t);
        float r2 = 0.0;
        float r3 = get<3>(t);
        
        float l1 = metai[ai];
        float l2 = metai[bi];
        float l3 = metai[ci];
        
        point &a = polar[ai];
        point &b = polar[bi];
        point &c = polar[ci];
        
        point &v = start_pos[truth_part[ai]];
        float vx = v.x;
        float vy = v.y;
        float vz = v.z;

        float score = reco[ir]->scoreTriple(ai,bi,ci);

        float r1 = 0.0;
        if (samepart(ai, bi) && samepart(ai, ci) && samepart(bi, ci)) {
            float x[20]={(float)a.x,(float)a.y,(float)a.z,(float)b.x,(float)b.y,(float)b.z,(float)c.x,(float)c.y,(float)c.z,score,l1,l2,l3,vx,vy,vz,r1,r2,r3,1.0};
            nt.Fill(x); // right combination
            right++;
        }
        else {
            if (r.Rndm()<right/wrong) {
                float x[20]={(float)a.x,(float)a.y,(float)a.z,(float)b.x,(float)b.y,(float)b.z,(float)c.x,(float)c.y,(float)c.z,score,l1,l2,l3,vx,vy,vz,r1,r2,r3,0.0};
                nt.Fill(x); // wrong combination
                wrong++;
            }
        }
        
        // Vertex
        hits[0] = v;
        if (samepart(ai, bi) && samepart(ai, ci) && samepart(bi, ci)) {
            score = reco[0]->scoreTriple(0,ai,bi);
            float x[20]={(float)a.x,(float)a.y,(float)a.z,(float)b.x,(float)b.y,(float)b.z,(float)c.x,(float)c.y,(float)c.z,score,l1,l2,l3,vx,vy,vz,r1,r2,r3,1.0};
            nt.Fill(x); // right combination
            right++;
        }

    }
    cout << "makeTrain3 right: " << right << " wrong: " << wrong << endl;
}


// Look for seeding points by hit pair combinations in the innnermost layers
void Trainer::makeTrainTriples(Graph<long long> &g,TNtuple &nt)
{
    long right=1,wrong=1;
    
    reco[0]->initDensity3();
    
    for (auto &track : particles) {
        if (!eval&&!debug) load(particles.size(), "makeTrainTriples");
        vector<int> t = track.hit;
        if (t.size()<=2) continue;
        
        for (int i=0;i<track.hits-2;i++) {
            
            int ai = t[i];
            int bi = t[i+1];
            int ci = t[i+2];
            int good = samepart(ai,bi) && samepart(bi,ci);
            if (good==0) continue;
            
            float l1 = metai[ai];
            float l2 = metai[bi];
            float l3 = metai[ci];
            point &a = polar[ai];
            point &b = polar[bi];
            point &c = polar[ci];
            point &v = start_pos[truth_part[ai]];
            float vx = v.x;
            float vy = v.y;
            float vz = v.z;

            // pair recall
            float f[7];
            reco[0]->getFeatures(ai,bi,f,v);
            float r1 = reco[0]->recall1(g,ai,bi,f[0],f[1]);
            float r2 = reco[0]->recall2(g,ai,bi,f[0],f[1],0.001*f[6]);

            // triple recall
            float score = reco[0]->scoreTriple(ai,bi,ci);
            float r3 = reco[0]->recall3(g,ai,bi,ci,score,r2);

            float x[20]={(float)a.x,(float)a.y,(float)a.z,(float)b.x,(float)b.y,(float)b.z,(float)c.x,(float)c.y,(float)c.z,score,l1,l2,l3,vx,vy,vz,r1,r2,r3,1.0};
            nt.Fill(x); // right combination
            right++;
            
            for (auto i : g.data(voxel(ci))) {
                if (r.Rndm()>2.0*right/wrong) continue;
                point &c = polar[i];
                int di = i;
                if (di==ai || di==bi || di==ci) continue; // Do not take the same hit
                point &v = start_pos[truth_part[di]];
                float vx = v.x;
                float vy = v.y;
                float vz = v.z;

                // pair recall
                float f[7];
                reco[0]->getFeatures(bi,di,f,v);
                float r1 = reco[0]->recall1(g,bi,di,f[0],f[1]);
                float r2 = reco[0]->recall2(g,bi,di,f[0],f[1],0.001*f[6]);

                // triple recall
                float score = reco[0]->scoreTriple(ai,bi,di);
                float r3 = reco[0]->recall3(g,ai,bi,di,score,r2);

                float x[20]={(float)a.x,(float)a.y,(float)a.z,(float)b.x,(float)b.y,(float)b.z,(float)c.x,(float)c.y,(float)c.z,score,l1,l2,l3,vx,vy,vz,r1,r2,r3,0.0};
                nt.Fill(x); //wrong combination
                wrong++;
            }
        }
    }
    cout << "makeTrainTriples: " <<  Tracker::particles.size() << " particles. right: " << right << " wrong: " << wrong << endl;
}


// Look for seeding points by hit pair combinations in the innnermost layers
void Trainer::makeTrainTriplesGraph(Graph<long long> &g,TNtuple &nt)
{
    long right=1,wrong=1;

    for (auto &track : particles) {
        if (!eval&&!debug) load(particles.size(), "makeTrainTriplesGraph");
        vector<int> t = track.hit;
        if (t.size()<=2) continue;
        
        for (int i=0;i<track.hits-2;i++) {
            
            int ai = t[i];
            int bi = t[i+1];
            int ci = t[i+2];

            auto hits = g.data(voxel(ai)); // vector of hits in graph
            if (hits.size()==0) continue; // track is not present in graph

            int good = samepart(ai,bi) && samepart(bi,ci);
            if (good==0) continue;
            
            float l1 = metai[ai];
            float l2 = metai[bi];
            float l3 = metai[ci];
            
            point &a = polar[ai];
            point &b = polar[bi];
            point &c = polar[ci];
            
            point &v = start_pos[truth_part[ai]];
            float vx = v.x;
            float vy = v.y;
            float vz = v.z;

            // pair recall
            float f[7];
            reco[0]->getFeatures(bi,ci,f,v);
            float r1 = reco[0]->recall1(g,bi,ci,f[0],f[1]);
            float r2 = reco[0]->recall2(g,bi,ci,f[0],f[1],0.001*f[6]);

            // triple recall
            float score = reco[0]->scoreTriple(ai,bi,ci);
            float r3 = reco[0]->recall3(g,ai,bi,ci,score,r2);

            float x[20]={(float)a.x,(float)a.y,(float)a.z,(float)b.x,(float)b.y,(float)b.z,(float)c.x,(float)c.y,(float)c.z,score,l1,l2,l3,vx,vy,vz,r1,r2,r3,1.0};
            nt.Fill(x); //right combination
            right++;
            
            for (auto i : g.data(voxel(ci))) {
                if (r.Rndm()>2.0*right/wrong) continue;
                point &c = polar[i];
                int di = i;
                if (di==ai || di==bi || di==ci) continue; // Do not take the same hit
                point &v = start_pos[truth_part[di]];
                float vx = v.x;
                float vy = v.y;
                float vz = v.z;
                
                // pair recall
                float f[7];
                reco[0]->getFeatures(bi,di,f,v);
                float r1 = reco[0]->recall1(g,bi,di,f[0],f[1]);
                float r2 = reco[0]->recall2(g,bi,di,f[0],f[1],0.001*f[6]);

                // triple recall
                float score = reco[0]->scoreTriple(ai,bi,di);
                float r3 = reco[0]->recall3(g,ai,bi,di,score,r2);

                float x[20]={(float)a.x,(float)a.y,(float)a.z,(float)b.x,(float)b.y,(float)b.z,(float)c.x,(float)c.y,(float)c.z,score,l1,l2,l3,vx,vy,vz,r1,r2,r3,0.0};
                nt.Fill(x); //wrong combination
                wrong++;
            }
        }
    }
    cout << "makeTrainTriplesGraph: " <<  Tracker::particles.size() << " particles. right: " << right << " wrong: " << wrong << endl;
}




#include <iostream>
#include "TString.h"
#include "TMVA/Factory.h"

using namespace std;

int FILENUMBER(21100);
int NFILES(10);
//int NFILES(2);

void mlp1(int nfiles=NFILES) {
    
    TMVA::Tools::Instance();
    
    TFile *outputFile(0);
    TFile *inputFile(0);
    
    TList *t2 = new TList;
    for (int event=FILENUMBER;event<FILENUMBER+nfiles;event++) {
        char filename[256];
        sprintf(filename, "../test/event%09d.root",event);
        inputFile = TFile::Open(filename);
        TTree *tracks2 = (TTree*) inputFile->Get("tracks2");
        t2->Add(tracks2);
    }
    
    outputFile = TFile::Open("TMVAOutputMLP1.root", "RECREATE");
    auto *factory = new TMVA::Factory("TMVAClassification", outputFile,"!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification" );
    
    TCut signal("truth!=0");
    TCut background("truth==0");

    TTree *tracks2 = TTree::MergeTrees(t2);
    TTree *tsignal = tracks2->CopyTree(signal);
    TTree *tbackground = tracks2->CopyTree(background);

    auto *loader = new TMVA::DataLoader("dataset_mlp");
    loader->AddVariable("rz1");
    loader->AddVariable("phi1");
    loader->AddVariable("z1");
    loader->AddVariable("rz2");
    loader->AddVariable("phi2");
    loader->AddVariable("z2");
    loader->AddVariable("f0");
    loader->AddVariable("f1");
    
    // Select first hit is on Disc detector (r1==0)
    loader->AddSignalTree(tsignal);
    loader->AddBackgroundTree(tbackground);
    loader->PrepareTrainingAndTestTree(TCut("r1==0"),"nTrain_Signal=300000:nTrain_Background=300000:SplitMode=Random:NormMode=NumEvents:!V");
    //loader->PrepareTrainingAndTestTree(TCut("r1==0"),"nTrain_Signal=50000:nTrain_Background=50000:SplitMode=Random:NormMode=NumEvents:!V");
 
    // General layout
    TString layoutString = TString("Layout=TANH|128,TANH|128,LINEAR");
    
    // Training strategies
    TString training0 = TString("LearningRate=1e-1,Momentum=0.9,Repetitions=1,"
                                "ConvergenceSteps=2,BatchSize=256,TestRepetitions=10,"
                                "WeightDecay=1e-4,Regularization=L2,"
                                "DropConfig=0.0+0.5+0.5+0.5, Multithreading=True");
    TString training1 = TString("LearningRate=1e-2,Momentum=0.9,Repetitions=1,"
                                "ConvergenceSteps=2,BatchSize=256,TestRepetitions=10,"
                                "WeightDecay=1e-4,Regularization=L2,"
                                "DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
    TString trainingStrategyString = TString("TrainingStrategy=");
    trainingStrategyString += training0 + TString("|") + training1;
    
    // General Options
    TString dnnOptions = TString("!H:!V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
                                 "WeightInitialization=XAVIERUNIFORM");
    dnnOptions.Append(":");
    dnnOptions.Append(layoutString);
    dnnOptions.Append(":");
    dnnOptions.Append(trainingStrategyString);
    
    // Standard implementation, no dependencies.
    TString stdOptions =  dnnOptions + ":Architecture=CPU";
    //factory->BookMethod(loader, TMVA::Types::kDNN, "DNN1", stdOptions);
    factory->BookMethod(loader, TMVA::Types::kMLP, "MLP1", "!H:!V:NeuronType=tanh:VarTransform=N:NCycles=100:HiddenLayers=15,5:TestRate=5:!UseRegulator" );
    
    factory->TrainAllMethods();
    
    factory->TestAllMethods();
    factory->EvaluateAllMethods();
    
    auto *c = factory->GetROCCurve(loader);
    c->Draw();
    
}


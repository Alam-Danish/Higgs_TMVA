// This code is based on TMVAClassification.C from TMVA package

// Include the necessary header files for C++
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

// Include the necessary header files for ROOT
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TObjString.h"

// Include the necessary header files for TMVA
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

void training_test()
{
    // Load the TMVA library
    TMVA::Tools::Instance();

    // Create a ROOT output file where TMVA will store its output
    // Used "RECREATE" option to recreate the file if it already exists, and overwrite it if program runs again
    TString outfileName( "OutputTMVA.root" );
    TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

    // Create the factory object
    TMVA::Factory factory("TMVAClassification", outputFile,
                          "!V:!ROC:!Correlations:!Silent:Color:!DrawProgressBar:AnalysisType=Classification");

    // Create a DataLoader object
    TMVA::DataLoader loader("dataset");

    // Add the variables to the dataloader
    // The varible details are on the website : https://opendata.cern.ch/record/328
    // The variable name must be identical to the variable name in the input file

    // The first argument is the variable name, the second is the variable type
    // The variable type can be either 'F' for float or 'I' for integer
    loader.AddVariable("DER_mass_MMC",'F');
    loader.AddVariable("DER_mass_transverse_met_lep",'F');
    loader.AddVariable("DER_mass_vis",'F');
    loader.AddVariable("DER_pt_h",'F');
    loader.AddVariable("DER_deltaeta_jet_jet",'F');
    loader.AddVariable("DER_mass_jet_jet",'F');
    loader.AddVariable("DER_prodeta_jet_jet",'F');
    loader.AddVariable("DER_deltar_tau_lep",'F');
    loader.AddVariable("DER_pt_tot",'F');
    loader.AddVariable("DER_sum_pt",'F');
    loader.AddVariable("DER_pt_ratio_lep_tau",'F');
    loader.AddVariable("DER_met_phi_centrality",'F');
    loader.AddVariable("DER_lep_eta_centrality",'F');
    loader.AddVariable("PRI_tau_pt",'F');
    loader.AddVariable("PRI_tau_eta",'F');
    loader.AddVariable("PRI_tau_phi",'F');
    loader.AddVariable("PRI_lep_pt",'F');
    loader.AddVariable("PRI_lep_eta",'F');
    loader.AddVariable("PRI_lep_phi",'F');
    loader.AddVariable("PRI_met",'F');
    loader.AddVariable("PRI_met_phi",'F');
    loader.AddVariable("PRI_met_sumet",'F');
    loader.AddVariable("PRI_jet_num",'F');
    loader.AddVariable("PRI_jet_leading_pt",'F');
    loader.AddVariable("PRI_jet_leading_eta",'F');
    loader.AddVariable("PRI_jet_leading_phi",'F');
    loader.AddVariable("PRI_jet_subleading_pt",'F');
    loader.AddVariable("PRI_jet_subleading_eta",'F');
    loader.AddVariable("PRI_jet_subleading_phi",'F');
    loader.AddVariable("PRI_jet_all_pt",'F');

    // Open the signal and background trees
    auto sigFile = TFile::Open("data/atlas-higgs-challenge-2014-v2-sig.root");
    TTree* sigtree = (TTree*)sigFile->Get("Stree");

    auto bkgFile = TFile::Open("data/atlas-higgs-challenge-2014-v2-bkg.root");
    TTree* bkgtree = (TTree*)bkgFile->Get("Btree");

    // Set the event weight for signal and background
    double SigWeight = 1.0; // because we have only signal in the signal file
    double bkgWeight = 1.0; // because we have only background in the background file

    // Add the signal and background trees to the dataloader
    loader.AddSignalTree(sigtree,SigWeight);
    loader.AddBackgroundTree(bkgtree,bkgWeight);


    TCut cutSig, cutBkg;

    loader.PrepareTrainingAndTestTree(cutSig, cutBkg,
    "NTrain_Signal=10000:NTrain_Background=20000:NTest_Signal=0:NTest_Background=0"
    "SplitMode=Random:NormMode=NumEvents:!V");
    
    //method booking 
    
    
    //Cuts
    factory.BookMethod(&loader, TMVA::Types::kCuts, "Cuts","!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );

    // Boosted Decision Trees
    factory.BookMethod(&loader, TMVA::Types::kBDT, "BDT",
                      "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:"
                       "UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

    // Fisher Discriminant
    factory.BookMethod(&loader, TMVA::Types::kFisher, "Fisher",
                        "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );
    
    //training
    factory.TrainAllMethods();

    //testing
    factory.TestAllMethods();
    factory.EvaluateAllMethods();

    //ROC curve
    auto c1 = factory.GetROCCurve(&loader);
    c1->Draw();

    outputFile->Close();
    sigFile->Close();
    bkgFile->Close();

}
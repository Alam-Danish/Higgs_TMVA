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


    TCut cutSig, cutBkg; // Signal and background cut

   
   
   
   
   
    // Prepare the training and test trees
    loader.PrepareTrainingAndTestTree(cutSig, // signal events cut
                                      cutBkg, // background events cut
                                      "NTrain_Signal=10000:NTrain_Background=20000:NTest_Signal=0:NTest_Background=0"
// The above options are set as follows:
// NTrain_Signal=10000  -- The number of signal events in the training sample
// NTrain_Background=20000  -- The number of background events in the training sample
// NTest_Signal=0  -- This means that all remaining signal events are used in the test sample
// NTest_Background=0  -- This means that all remaining background events are used in the test sample
                                      "SplitMode=Random:NormMode=NumEvents:!V");
// The above options are set as follows:
// SplitMode=Random  -- Randomly splits the data into training and test samples
// NormMode=NumEvents  -- normalization mode for the training and testing samples based on the number of events
// !V  -- Disable the verbose output 
    
    
    
    
    
    
    //method booking 
    
    
    //Cuts
    // This is a standard automatic rectangular cut optimisation
    factory.BookMethod(&loader, // the data loader object
                        TMVA::Types::kCuts, // the type of classifier
                        "Cuts",           // the name of the classifier
                        "!H:!V:FitMethod=MC:EffSel:SampleSize=20000:VarProp=FSmart" ); 
    // The above options are set as follows:
    // !H:!V:  -- don't print the histograms and verbose output during training and testing
    // FitMethod=MC  -- fitting method to be used for training, in this case, Monte Carlo (MC) fitting method
    // EffSel  -- Enables the efficiency-based event selection during the evaluation phase
    // SampleSize=20000  -- The number of events to be used for the training (signal + background)
    // VarProp=FSmart  -- The variables are preselected using a fast selection method (FSmart)




    // Boosted Decision Trees
    factory.BookMethod(&loader, // the data loader object
                        TMVA::Types::kBDT,  // the type of classifier
                        "BDT",              // the name of the classifier
                        "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:"
    // The above options are set as follows:
    // NTrees=850  -- the number of trees to be used for training
    // MinNodeSize=2.5%  -- minimum percentage of training events required in a leaf node
    // MaxDepth=3  -- the maximum depth of the decision tree allowed
    // BoostType=AdaBoost  -- the boosting algorithm to be used (Adaptive Boosting)
    // AdaBoostBeta=0.5  -- the learning rate for Adaptive Boosting (between 0 and 1)
                        "UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
    // The above options are set as follows:
    // UseBaggedBoost  -- to train multiple decision trees on different subsets of the training data and then combining their predictions to make a final decision
    // BaggedSampleFraction=0.5  -- the fraction of events to be used for training each bagged decision tree
    // SeparationType=GiniIndex  -- the node splitting criterion. Gini Index: measures the impurity of a node by calculating the probability of misclassifying a randomly chosen element from that node
    // nCuts=20  -- the number of grid points to be used when searching for the optimal cut in each node





    // Fisher Discriminant
    factory.BookMethod(&loader, 
                        TMVA::Types::kFisher,
                        "Fisher",
                        "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );
    // The above options are set as follows:
    // VarTransform=None  -- the variables are not transformed
    // CreateMVAPdfs  -- create PDFs for classifier outputs
    // PDFInterpolMVAPdf=Spline2  -- interpolation method for PDFs (Spline2: 2nd order spline interpolation)
    // NbinsMVAPdf=50  -- number of bins for PDFs
    // NsmoothMVAPdf=10  -- smoothing factor for PDFs




    // Multi-Layer Perceptron (Neural Network)
    factory.BookMethod(&loader,
                        TMVA::Types::kMLP,
                        "MLPBNN", // MLPBNN is the multi-layer perceptron with Bayesian regularization
                        "!H:!V:NeuronType=tanh:VarTransform=N:NCycles=500:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:UseRegulator" );
    // The above options are set as follows:
    // NeuronType=tanh  -- the activation function for the neurons in the hidden layer (tanh: hyperbolic tangent function)
    // VarTransform=N  -- no variable transformation
    // NCycles=500  -- the number of training cycles(epochs)
    // HiddenLayers=N+5  -- the number of hidden layers (N+5: N is the number of input variables)
    // TestRate=5  -- the number of training cycles after which the network is tested against the test sample
    // TrainingMethod=BFGS  -- the training algorithm to be used (BFGS: Broyden-Fletcher-Goldfarb-Shanno algorithm)
    // UseRegulator  -- use regulator to avoid overtraining
    
    //training
    factory.TrainAllMethods();

    //testing
    factory.TestAllMethods();

    //evaluation
    factory.EvaluateAllMethods();

    //ROC curve
    auto c1 = factory.GetROCCurve(&loader);
    c1->Draw();

    // Close the files
    outputFile->Close();
    sigFile->Close();
    bkgFile->Close();

}
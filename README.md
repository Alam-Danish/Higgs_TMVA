# Higgs Machine Learning challenge with Higgs to tau-tau events

## Introduction
This repository contains the code for the "multivariate analysis" of simulated data for Higgs Boson decaying in two tau leptons with three major backgrounds.

This data is obtained from [opendataCERN](https://opendata.cern.ch/record/328), where it was published for "Machine Learning Challenge".

The dataset contains 818238 events.

## Classifiers Used
For more details, please refer: [TMVA USER GUIDE](https://root.cern/download/doc/tmva/TMVAUsersGuide.pdf)

- **Cuts:** Automatic rectangular cut optimization

- **BDT:** Boosted Decision Trees

- **Fisher:** Fisher's linear discriminant

- **MLPBNN:** Multi-Layer Perceptron with Bayesian regulator

## Results

**ROC Curve**

![ROC Curve](rejBvsS.png)

**Classifiers cut efficiencies**

This result, using the default significance formula written for the TMVA classifier cut efficiencies.

![default](mvaeffs_default.png)

The result below is using Asimov's Approximate Median Significance (AMS) formula,

![AMS](mvaeffs.png)

- **Dataset** folder contains all the plots and weight results generated after classification.



## Code Description

- **prepare_dataset.cpp:** This macro is written to prepare signal and background data trees separately.

- **training_test.cpp:** This macro is used for training and test purposes. Comments are provided in the code for better understanding.

// RDataFrame allows to analyse data stored in TTrees (CSVs) with a high level interface.


void prepare_dataset()
{
    // Create a RDataFrame from a CSV file
    auto df = ROOT::RDF::MakeCsvDataFrame("data/atlas-higgs-challenge-2014-v2.csv");
    
    
    // Get the number of entries
    auto nevents = *df.Count();
    std::cout << "number of events: " << nevents << "\n";
    
    
    // Returns the names of the available columns.
    auto colNames = df.GetColumnNames();
    for (auto &&colName : colNames)  {
        std::cout << colName << ", " ;
    }
    std::cout << '\n';

    // Save selected signal and background columns in a new TTree in new separate files.
    df.Filter("Label == \"s\" ")
        .Snapshot("Stree", "data/atlas-higgs-challenge-2014-v2-sig.root");

    df.Filter("Label == \"b\" ")
        .Snapshot("Btree", "data/atlas-higgs-challenge-2014-v2-bkg.root");

}
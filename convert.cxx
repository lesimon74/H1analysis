//
//  convert.cpp
//  
//
//  Created by Simon Leisibach on 25.09.19.
//

#include "convert.h"
#include "h1event.h"

#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>

#include <TFile.h>
#include <TSystem.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;

// Note (lesimon): doesn't work with https://root....
const std::string fileLocation = "http://root.cern.ch/files/h1/";

// Converts a TTree file with H1analysis data to a ntuple file.
void convert(const std::string fileName) {
   const std::string fullPath = fileLocation + fileName;

   // Opens the .root file containing a TTree. By using a unique_ptr the statement f->Close() is not needed.
   std::unique_ptr<TFile> f(TFile::Open(fullPath.c_str()));
   assert(f.get() && ! f->IsZombie());

   // h42 is the name of the TTree
   TTreeReader reader("h42", f.get());

   // Only create a TTreeReaderValue or TTreeReaderArray for branches relevant for the analysis.
   TTreeReaderValue<float>     ptds_d(reader, "ptds_d");
   TTreeReaderValue<float>     etads_d(reader, "etads_d");
   TTreeReaderValue<float>     dm_d(reader, "dm_d");
   TTreeReaderValue<std::int32_t>       ik(reader, "ik");
   TTreeReaderValue<Int_t>       ipi(reader, "ipi");
   TTreeReaderValue<Int_t>       ipis(reader, "ipis");
   TTreeReaderValue<Float_t>     ptd0_d(reader, "ptd0_d");
   TTreeReaderValue<Float_t>     md0_d(reader, "md0_d");
   TTreeReaderValue<Float_t>     rpd0_t(reader, "rpd0_t");
   TTreeReaderValue<Int_t>       ntracks(reader, "ntracks");
   TTreeReaderArray<Int_t>       nhitrp(reader, "nhitrp");
   TTreeReaderArray<Float_t>     rstart(reader, "rstart");
   TTreeReaderArray<Float_t>     rend(reader, "rend");
   TTreeReaderArray<Float_t>     nlhk(reader, "nlhk");
   TTreeReaderArray<Float_t>     nlhpi(reader, "nlhpi");
   TTreeReaderValue<Int_t>       njets(reader, "njets");

   // Loads the dictionary for the H1event struct. Without the dictionary MakeField<H1Event> would fail.
   gSystem->Load("./libH1event.so");
   {
      // Create a ntuple model with a single field
      auto model = RNTupleModel::Create();
      auto ev = model->MakeField<H1Event>("event");

      // h42 refers to the name of the ntuple.
      auto ntuple = RNTupleWriter::Recreate(std::move(model), "h42", fileName);

      // Fills the ntuple with entries from the TTree.
      while(reader.Next()) {
         int size = *ntracks;
         std::vector<H1Event::ntrack> ntrackNTuple(size);
         for (int i = 0; i < size; ++i) {
            ntrackNTuple.at(i) = H1Event::ntrack{nhitrp[i], rstart[i], rend[i], nlhk[i], nlhpi[i]};
         }

         H1Event eventEntry{*ptds_d, *etads_d, *dm_d, *ik, *ipi, *ipis, *ptd0_d, *md0_d, *rpd0_t, *ntracks, ntrackNTuple, *njets};
         *ev = eventEntry;
         ntuple->Fill();
      }
   } // Upon destruction of the RNTupleWriter object, the data is flushed to a ntuple file.
}

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
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;

// Note (lesimon): doesn't work with https://root....
const std::string fileLocation = "http://root.cern.ch/files/h1/";

void convert(const std::string fileName) {
   std::cout << "Passed convert.cxx, will create " << fileName << "\n";
   const std::string fullPath = fileLocation + fileName;
   
   std::unique_ptr<TFile> f(TFile::Open(fullPath.c_str()));
   assert(f.get() && ! f->IsZombie());
   
   TTreeReader reader("h42", f.get());
   
   TTreeReaderValue<Float_t>     ptds_d(reader, "ptds_d");
   TTreeReaderValue<Float_t>     etads_d(reader, "etads_d");
   TTreeReaderValue<Float_t>     dm_d(reader, "dm_d");
   TTreeReaderValue<Int_t>       ik(reader, "ik");
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
   
   gSystem->Load("./libH1event.so");
   {
      auto model = RNTupleModel::Create();
      auto ev = model->MakeField<H1Event>("event");
      
      auto ntuple = RNTupleWriter::Recreate(std::move(model), "h42", fileName);
      
      while(reader.Next()) {
         int size = *ntracks;
         std::vector<int> nhitrpNTuple(size);
         std::vector<float> rstartNTuple(size);
         std::vector<float> rendNTuple(size);
         std::vector<float> nlhkNTuple(size);
         std::vector<float> nlhpiNTuple(size);
         for (int i = 0; i < size; ++i) {
            nhitrpNTuple[i] = nhitrp[i];
            rstartNTuple[i] = rstart[i];
            rendNTuple[i] = rend[i];
            nlhkNTuple[i] = nlhk[i];
            nlhpiNTuple[i] = nlhpi[i];
         }
         
         H1Event eventEntry{*ptds_d, *etads_d, *dm_d, *ik, *ipi, *ipis, *ptd0_d, *md0_d, *rpd0_t, *ntracks, nhitrpNTuple, rstartNTuple, rendNTuple, nlhkNTuple, nlhpiNTuple, *njets};
         *ev = eventEntry;
         ntuple->Fill();
      }
   }
   
   //tree->Print();
}

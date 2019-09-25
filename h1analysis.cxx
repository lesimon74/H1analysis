//
//  h1analysis.c
//  
//
//  Created by Simon Leisibach on 25.09.19.
//

#include "convert.h"
#include "h1event.h"

#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>

#include <TSystem.h>

using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;

const std::string_view filename = "h1event.root";
const std::string_view ntuplename = "eventdata";

void h1analysis()
{
   gSystem->Load("./libH1event.so");
   {
      auto model = RNTupleModel::Create();
      auto ev = model->MakeField<H1Event>("event");
      
      auto ntuple = RNTupleWriter::Recreate(std::move(model), ntuplename, filename);
      
      for (int i = 0; i < 3; ++i) {
         H1Event eventEntry{(float)i, (float)i*2, (float)i*3};
         *ev = eventEntry;
         ntuple->Fill();
      }
   }
   
   auto ntuple = RNTupleReader::Open(ntuplename, filename);
   ntuple->PrintInfo();
   
} // end main()

int main() {
   h1analysis();
   return 0;
}

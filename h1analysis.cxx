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

#include <TCanvas.h>
#include <TF1.h>
#include <TH2.h>
#include <TLine.h>
#include <TMath.h>
#include <TStyle.h>
#include <TSystem.h>

#include <fstream>

using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;

const std::string_view ntuplename = "eventdata";


const std::vector<std::string> ntupleFiles { "dstarmb.root", "dstarp1a.root", "dstarp1b.root", "dstarp2.root"};


const Double_t dxbin = (0.17-0.13)/40;   // Bin-width
const Double_t sigma = 0.0012;


Double_t fdm5(Double_t *xx, Double_t *par)
{
   Double_t x = xx[0];
   if (x <= 0.13957) return 0;
   Double_t xp3 = (x-par[3])*(x-par[3]);
   Double_t res = dxbin*(par[0]*TMath::Power(x-0.13957, par[1])
       + par[2] / 2.5066/par[4]*TMath::Exp(-xp3/2/par[4]/par[4]));
   return res;
}


Double_t fdm2(Double_t *xx, Double_t *par)
{
   Double_t x = xx[0];
   if (x <= 0.13957) return 0;
   Double_t xp3 = (x-0.1454)*(x-0.1454);
   Double_t res = dxbin*(par[0]*TMath::Power(x-0.13957, 0.25)
       + par[1] / 2.5066/sigma*TMath::Exp(-xp3/2/sigma/sigma));
   return res;
}

void h1analysis()
{
   // create non-existing ntuple-files by converting TTree files into ntuple files.
   for (const auto& f: ntupleFiles) {
      if (!std::ifstream(f)) {
         convert(f);
      }
   }
   
   // Loads the dictionary for H1event objects.
   gSystem->Load("./libH1event.so");
   
   auto ntuple = RNTupleReader::Open("h42", ntupleFiles);
   ntuple->PrintInfo();
   
   auto hdmd = new TH1F("hdmd","dm_d",40,0.13,0.17);
   auto h2   = new TH2F("h2","ptD0 vs dm_d",30,0.135,0.165,30,-3,6);

   auto dm_dView = ntuple->GetView<float>("event.dm_d");
   auto rpd0_tView = ntuple->GetView<float>("event.rpd0_t");
   auto ptd0_dView = ntuple->GetView<float>("event.ptd0_d");
   for (auto i : ntuple->GetViewRange()) {
      hdmd->Fill(dm_dView(i));
      h2->Fill(dm_dView(i),rpd0_tView(i)/0.029979*1.8646/ptd0_dView(i));
   }
   
   
   gStyle->SetOptFit();
   TCanvas *c1 = new TCanvas("c1","h1analysis analysis",10,10,800,600);
   c1->SetBottomMargin(0.15);
   hdmd->GetXaxis()->SetTitle("m_{K#pi#pi} - m_{K#pi}[GeV/c^{2}]");
   hdmd->GetXaxis()->SetTitleOffset(1.4);

   TF1 *f5 = new TF1("f5",fdm5,0.139,0.17,5);
   f5->SetParameters(1000000, .25, 2000, .1454, .001);
   hdmd->Fit("f5","lr");

   gStyle->SetOptFit(0);
   gStyle->SetOptStat(1100);
   TCanvas *c2 = new TCanvas("c2","tauD0",100,100,800,600);
   c2->SetGrid();
   c2->SetBottomMargin(0.15);
   
   TF1 *f2 = new TF1("f2",fdm2,0.139,0.17,2);
   f2->SetParameters(10000, 10);
   h2->FitSlicesX(f2,0,-1,1,"qln");
   TH1D *h2_1 = (TH1D*)gDirectory->Get("h2_1");
   h2_1->GetXaxis()->SetTitle("#tau[ps]");
   h2_1->SetMarkerStyle(21);
   h2_1->Draw();
   c2->Update();
   TLine *line = new TLine(0,0,0,c2->GetUymax());
   line->Draw();
   
   hdmd->Draw();
   h2->Draw();
} // end void h1analysis()

int main() {
   h1analysis();
   return 0;
}

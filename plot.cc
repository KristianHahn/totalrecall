// g++ `root-config --cflags` plot.cc -o plot.exe `root-config --libs` 

#include <iostream>
#include <vector>
#include <string.h>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH1F.h"

int main() { 

  std::vector<std::string> fnames;

  fnames.push_back("stats_O0.root");
  fnames.push_back("stats_Os.root");
  fnames.push_back("stats_O2.root");


 std::vector<EColor> colors;
  colors.push_back(kRed);
  colors.push_back(kOrange);
  colors.push_back(kYellow);
  // colors.push_back(kSpring);
  // colors.push_back(kGreen);
  // colors.push_back(kTeal);
  // colors.push_back(kCyan);
  // colors.push_back(kAzure);
  // colors.push_back(kBlue);
  // colors.push_back(kViolet);
  // colors.push_back(kMagenta);
  // colors.push_back(kPink);
  // colors.push_back(kGray);
  // colors.push_back(kBlack);
  // colors.push_back(kBlack);


  float size, rate;
  std::vector<float> vsize, vrate;
  std::vector<TGraph*> memcpy_graphs, memset_graphs, fwrite_graphs;

  for ( int i=0; i<fnames.size(); i++ ) {
    TFile * f = new TFile(fnames[i].c_str());
    std::cout << fnames[i].c_str() << std::endl;
    std::cout << "--------------------------" << std::endl;
    TTree * t = (TTree*)(f->Get("stats"));
    //    t->Print();

    // draw to the graphs
    int nent;
    nent = t->Draw("Size:Rate", "Op==\"memcpy\"", "goff");
    memcpy_graphs.push_back(new TGraph(nent,t->GetV1(),t->GetV2()) );
    (memcpy_graphs[i])->SetLineWidth(2);
    (memcpy_graphs[i])->SetMarkerSize(1.25);
    (memcpy_graphs[i])->SetMarkerStyle(20);
    (memcpy_graphs[i])->SetLineColor(colors[i]);
    (memcpy_graphs[i])->SetMarkerColor(colors[i]);
    (memcpy_graphs[i])->SetName(fnames[i].c_str());
    (memcpy_graphs[i])->Print();
    nent = t->Draw("Size:Rate", "Op==\"memset\"", "goff");
    memset_graphs.push_back(new TGraph(nent,t->GetV1(),t->GetV2()) );
    (memset_graphs[i])->SetLineWidth(2);
    (memset_graphs[i])->SetMarkerSize(1.25);
    (memset_graphs[i])->SetMarkerStyle(20);
    (memset_graphs[i])->SetLineColor(colors[i]);
    (memset_graphs[i])->SetMarkerColor(colors[i]);
    (memset_graphs[i])->SetName(fnames[i].c_str());
    (memset_graphs[i])->Print();
    nent = t->Draw("Size:Rate", "Op==\"fwrite\"", "goff");
    fwrite_graphs.push_back(new TGraph(nent,t->GetV1(),t->GetV2()) );
    (fwrite_graphs[i])->SetLineWidth(2);
    (fwrite_graphs[i])->SetMarkerSize(1.25);
    (fwrite_graphs[i])->SetMarkerStyle(20);
    (fwrite_graphs[i])->SetLineColor(colors[i]);
    (fwrite_graphs[i])->SetMarkerColor(colors[i]);
    (fwrite_graphs[i])->SetName(fnames[i].c_str());
    (fwrite_graphs[i])->Print();        
  }

  TCanvas * c = new TCanvas("c","c",0,0,1800,1200);
  TH1F * frame = (TH1F*)(c->DrawFrame(0,0,2049,50));
  frame->GetXaxis()->SetTitle("size [MB]");
  frame->GetYaxis()->SetTitle("rate [MB/s]");
  TLegend * leg = new TLegend(0.6,0.6,0.95,0.95);
  leg->SetFillColor(kWhite);
  for( int i=0; i<memcpy_graphs.size(); i++ ) {
    (memcpy_graphs[i])->Draw("LP");
    leg->AddEntry(fnames[i].c_str(),fnames[i].c_str(),"LP");
  }
  leg->Draw("same");
  c->Print("memcpy.png");


  TCanvas * cc = new TCanvas("cc","cc",0,0,1800,1200);
  TH1F * fframe = (TH1F*)(cc->DrawFrame(0,0,2049,50));
  fframe->GetXaxis()->SetTitle("size [MB]");
  fframe->GetYaxis()->SetTitle("rate [MB/s]");
  TLegend * lleg = new TLegend(0.6,0.6,0.95,0.95);
  lleg->SetFillColor(kWhite);
  for( int i=0; i<memset_graphs.size(); i++ ) {
    (memset_graphs[i])->Draw("LP");
    lleg->AddEntry(fnames[i].c_str(),fnames[i].c_str(),"LP");
  }
  lleg->Draw("same");
  cc->Print("memset.png");


  TCanvas * ccc = new TCanvas("ccc","ccc",0,0,1800,1200);
  TH1F * ffframe = (TH1F*)(ccc->DrawFrame(0,0,2049,50));
  ffframe->GetXaxis()->SetTitle("size [MB]");
  ffframe->GetYaxis()->SetTitle("rate [MB/s]");
  TLegend * llleg = new TLegend(0.6,0.6,0.95,0.95);
  llleg->SetFillColor(kWhite);
  for( int i=0; i<fwrite_graphs.size(); i++ ) {
    (fwrite_graphs[i])->Draw("LP");
    llleg->AddEntry(fnames[i].c_str(),fnames[i].c_str(),"LP");
  }
  llleg->Draw("same");
  ccc->Print("fwrite.png");    



}

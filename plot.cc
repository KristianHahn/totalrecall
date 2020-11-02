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

  fnames.push_back("results/ryzen_stats_Os.root");
  fnames.push_back("results/ryzen_stats_O0.root");
  fnames.push_back("results/ryzen_stats_O2.root");
  fnames.push_back("results/ryzen_stats_O2_RT.root");
  fnames.push_back("results/ryzen_stats_O2_RT_dropcache.root");

  fnames.push_back("results/XeonE5_stats_Os.root");
  fnames.push_back("results/XeonE5_stats_O0.root");
  fnames.push_back("results/XeonE5_stats_O2.root");
  fnames.push_back("results/XeonE5_stats_O2_RT.root");
  fnames.push_back("results/XeonE5_stats_O2_RT_dropcache.root");

  std::vector<EColor> colors;
  colors.push_back(kRed);
  colors.push_back(kOrange);
  colors.push_back(kYellow);
  colors.push_back(kSpring);
  //  colors.push_back(kGreen);
  colors.push_back(kTeal);
  // colors.push_back(kCyan);
  colors.push_back(kAzure);
  colors.push_back(kBlue);
  colors.push_back(kViolet);
  colors.push_back(kMagenta);
  //  colors.push_back(kPink);
  colors.push_back(kGray);
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
    (memcpy_graphs[i])->SetMarkerSize(.75);
    (memcpy_graphs[i])->SetMarkerStyle(20);
    (memcpy_graphs[i])->SetLineColor(colors[i]);
    (memcpy_graphs[i])->SetMarkerColor(colors[i]);
    (memcpy_graphs[i])->SetName(fnames[i].c_str());
    (memcpy_graphs[i])->Print();
    nent = t->Draw("Size:Rate", "Op==\"memset\"", "goff");
    memset_graphs.push_back(new TGraph(nent,t->GetV1(),t->GetV2()) );
    (memset_graphs[i])->SetLineWidth(2);
    (memset_graphs[i])->SetMarkerSize(.75);
    (memset_graphs[i])->SetMarkerStyle(20);
    (memset_graphs[i])->SetLineColor(colors[i]);
    (memset_graphs[i])->SetMarkerColor(colors[i]);
    (memset_graphs[i])->SetName(fnames[i].c_str());
    (memset_graphs[i])->Print();
    nent = t->Draw("Size:Rate", "Op==\"fwrite\"", "goff");
    fwrite_graphs.push_back(new TGraph(nent,t->GetV1(),t->GetV2()) );
    (fwrite_graphs[i])->SetLineWidth(2);
    (fwrite_graphs[i])->SetMarkerSize(.75);
    (fwrite_graphs[i])->SetMarkerStyle(20);
    (fwrite_graphs[i])->SetLineColor(colors[i]);
    (fwrite_graphs[i])->SetMarkerColor(colors[i]);
    (fwrite_graphs[i])->SetName(fnames[i].c_str());
    (fwrite_graphs[i])->Print();        
  }
  
  TCanvas * c = new TCanvas("c","c",0,0,1200,600);
  c->SetFillColor(kBlack);
  TH1F * frame = (TH1F*)(c->DrawFrame(0,0,2049,50));
  frame->SetFillColor(kBlack);
  frame->GetXaxis()->SetTitle("size [MB]");
  frame->GetXaxis()->SetTitleColor(kGray);
  frame->GetXaxis()->SetLabelColor(kGray);
  frame->GetXaxis()->SetAxisColor(kGray);
  frame->GetYaxis()->SetTitle("rate [MB/s]");
  frame->GetYaxis()->SetTitleColor(kGray);
  frame->GetYaxis()->SetLabelColor(kGray);
  frame->GetYaxis()->SetAxisColor(kGray);
  
  TLegend * leg = new TLegend(0.6,0.6,0.95,0.95);
  leg->SetFillColor(kBlack);
  leg->SetTextColor(kGray);
  for( int i=0; i<memcpy_graphs.size(); i++ ) {
    (memcpy_graphs[i])->Draw("LP");
    leg->AddEntry(fnames[i].c_str(),fnames[i].c_str(),"LP");
  }
  leg->Draw("same");
  c->Print("results/memcpy.png");
  frame->GetXaxis()->SetRange(0,frame->GetXaxis()->FindBin(257));
  c->Update();
  c->Print("results/memcpy_zoom.png");
  
  TCanvas * cc = new TCanvas("cc","cc",0,0,1200,600);
  cc->SetFillColor(kBlack);
  TH1F * fframe = (TH1F*)(cc->DrawFrame(0,0,2049,20));
  fframe->SetFillColor(kBlack);
  fframe->GetXaxis()->SetTitle("size [MB]");
  fframe->GetXaxis()->SetTitleColor(kGray);
  fframe->GetXaxis()->SetLabelColor(kGray);
  fframe->GetXaxis()->SetAxisColor(kGray);
  fframe->GetYaxis()->SetTitle("rate [MB/s]");
  fframe->GetYaxis()->SetTitleColor(kGray);
  fframe->GetYaxis()->SetLabelColor(kGray);
  fframe->GetYaxis()->SetAxisColor(kGray);
  TLegend * lleg = new TLegend(0.6,0.6,0.95,0.95);
  lleg->SetFillColor(kBlack);
  lleg->SetTextColor(kGray);
  for( int i=0; i<memset_graphs.size(); i++ ) {
    (memset_graphs[i])->Draw("LP");
    lleg->AddEntry(fnames[i].c_str(),fnames[i].c_str(),"LP");
  }
  lleg->Draw("same");
  cc->Print("results/memset.png");
  
  
  TCanvas * ccc = new TCanvas("ccc","ccc",0,0,1200,600);
  ccc->SetFillColor(kBlack);
  TH1F * ffframe = (TH1F*)(ccc->DrawFrame(0,0,2049,20));
  ffframe->SetFillColor(kBlack);
  ffframe->GetXaxis()->SetTitle("size [MB]");
  ffframe->GetXaxis()->SetTitleColor(kGray);
  ffframe->GetXaxis()->SetLabelColor(kGray);
  ffframe->GetXaxis()->SetAxisColor(kGray);
  ffframe->GetYaxis()->SetTitle("rate [MB/s]");
  ffframe->GetYaxis()->SetTitleColor(kGray);
  ffframe->GetYaxis()->SetLabelColor(kGray);
  ffframe->GetYaxis()->SetAxisColor(kGray);
  TLegend * llleg = new TLegend(0.6,0.6,0.95,0.95);
  llleg->SetFillColor(kBlack);
  llleg->SetTextColor(kGray);
  for( int i=0; i<fwrite_graphs.size(); i++ ) {
    (fwrite_graphs[i])->Draw("LP");
    llleg->AddEntry(fnames[i].c_str(),fnames[i].c_str(),"LP");
  }
  llleg->Draw("same");
  ccc->Print("results/fwrite.png");    
  
}

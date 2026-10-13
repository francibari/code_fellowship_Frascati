#include <string>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include "TCanvas.h"
#include "TH1D.h"


// questa funzione fa un plot di energia depositata per step, distinta per ognni particella; 
// fa anche un plot 2d dell'energia depositata per step in funzione dell'energia del fascio iniziale di neutroni
// vorrei implementare la stessa cosa anche con le tracce invece che con gli step
void plotPDGstep (const char* fileName) {
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  TFile *f = TFile::Open(fileName, "READ");
  if (!f || f->IsZombie()) {
      std::cout << "Errore: Impossibile aprire il file " << fileName << std::endl;
  }

  // Create a canvas and divide it into 2x2 pads
  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  c1->Divide(1,2);

  // Get ntuple
  TNtuple* ntuple = (TNtuple*)f->Get("Step");

  // creo gli istogrammi
  THStack* hs = new THStack("hs", "Energia depositata per step;Edep(MeV);Conteggi");
  TH1D* h_ep = new TH1D("h_ep", "electron_positron; Edep(MeV);Conteggi", 50, 0, 0.05);
  TH1D* h_heavy = new TH1D("h_heavy", "heavy_ion", 50, 0, 0.05);
  TH1D* h_proton = new TH1D("h_proton", "proton", 50, 0, 0.05);
  TH1D* h_gamma = new TH1D("h_gamma", "gamma", 50, 0, 0.05);

  std::vector<double> ybins = {1e-4, 1e-3, 1e-2, 1e-1, 1, 1e+1, 1e+2, 1e+3, 1e+4, 1e+5, 1e+6, 1e+7, 1e+8, 1e+9, 1e+10, 1e+11, 1e+12, 1e+13};
  int nbinsY = ybins.size() - 1; // Il numero di bin è la dimensione meno 1

  // crea il TH2D passando ybins.data()
  TH2D* h2d = new TH2D("h2d", "Energy deposited in the gas; Edep (MeV); BeamEnergy(eV)", 50, 0.0, 0.05, nbinsY, ybins.data());


  //c1->cd();
  //hs->Draw("HIST");

  // quin voglio plot di energia depositata dalle tracce nel gas, selezionate eventualmente per pdg
  // Create an link variable with the ntuple content

  Double_t edep = 0.;
  Int_t pdg = 0;
  Int_t evID = 0;
  Int_t track = 0;
  char logical[256];
  Double_t beam = 0.;

  Int_t prevID = 0;
  Int_t prevTrack = -1;
  Int_t prevPDG = 0;

  ntuple->SetBranchAddress("Edep", &edep);
  ntuple->SetBranchAddress("PDG", &pdg);
  ntuple->SetBranchAddress("EventID", &evID);
  ntuple->SetBranchAddress("TrackID", &track);
  ntuple->SetBranchAddress("LVName", &logical);
  ntuple->SetBranchAddress("BeamEnergy", &beam);


  TH1D* h = new TH1D("edep", "Energy deposited in totale [MeV]", 200, 0, 0.2);

  for (int i = 0; i<ntuple->GetEntries(); ++i) {
    ntuple->GetEntry(i);
    if (strcmp(logical, "GasGapLV") == 0 && edep > 0.) {
        h2d->Fill(edep, beam);
        switch (pdg) {
            case 11:
            h_ep->Fill(edep);
            break;
            case -11:
            h_ep->Fill(edep);
            break;
            case 22:
            h_gamma->Fill(edep);
            break;
            case 2212:
            h_proton->Fill(edep);
            break;
            default:
            h_heavy->Fill(edep);
        }
    }
  }

  c1->cd(1);
  h_ep->SetLineColor(kBlue);
  h_gamma->SetLineColor(kRed);
  h_heavy->SetLineColor(kGreen);
  h_proton->SetLineColor(kOrange);
  h_ep->DrawCopy("hist");
  h_gamma->DrawCopy("SAME");
  h_proton->DrawCopy("SAME");
  h_heavy->DrawCopy("SAME");
  
  // setto i colori che riempiono gli istogrammi
  h_ep->SetFillColor(kBlue);
  h_gamma->SetFillColor(kRed);
  h_heavy->SetFillColor(kGreen);
  h_proton->SetFillColor(kOrange);
  
  // li aggiungo allo stack
  hs->Add(h_proton);
  hs->Add(h_gamma);  
  hs->Add(h_ep);
  hs->Add(h_heavy);

  c1->cd(2);
  hs->DrawClone("hist");
  
  std::cout << "electrons and positrons = " << h_ep->GetEntries() << std::endl;
  std::cout << "heavy ions = " << h_heavy->GetEntries() << std::endl;
  std::cout << "protons = " << h_proton->GetEntries() << std::endl;
  std::cout << "gammas = " << h_gamma->GetEntries() << std::endl;

  TCanvas* c2 = new TCanvas("c2", "", 20, 20, 700, 700);
  c2->cd();
  gPad->SetLogy(1);
  h2d->DrawCopy("colz");

  TFile* outFile = TFile::Open("energy_result.root", "RECREATE");
  outFile->cd();
  c1->Write();
  h2d->Write();
  outFile->Close();
  delete outFile;

  f->cd();
  f->Close();
  delete f;
}















// plot energy deposited in each gap
void plotEnergy(const char* fileName) {
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  
  TFile *f = TFile::Open(fileName, "READ");
  if (!f || f->IsZombie()) {
      std::cout << "Errore: Impossibile aprire il file " << fileName << std::endl;
  }

  std::string outFileName = std::string("res_") + fileName;

  // Create a canvas and divide it into 2x2 pads
  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  TCanvas* c2 = new TCanvas("c2", "", 20, 20, 1000, 1000);
  //c1->Divide(2,2);

  // Get ntuple
  TNtuple* ntuple = (TNtuple*)f->Get("Step");

  // Create an link variable with the ntuple content
  Double_t de = 0.;

  Double_t edep = 0.;
  Double_t beam = 0.;
  Int_t evID = 0;
  Int_t layer = 0;
  char logical[256];

  Int_t prevID = 0;
  Double_t prevBeam = 0.;

  ntuple->SetBranchAddress("Edep", &edep);
  ntuple->SetBranchAddress("EventID", &evID);
  ntuple->SetBranchAddress("LVName", &logical);
  ntuple->SetBranchAddress("BeamEnergy", &beam);

  std::vector<double> ybins = {1e-4, 1e-3, 1e-2, 1e-1, 1, 1e+1, 1e+2, 1e+3, 1e+4, 1e+5, 1e+6, 1e+7, 1e+8, 1e+9, 1e+10, 1e+11, 1e+12, 1e+13};
  int nbinsY = ybins.size() - 1;

  TH1D* h = new TH1D("h", "Energy deposited in totale; Edep (MeV); Conteggi", 200, 0, 0.2);
  TH2D* h2d = new TH2D("h2d", "Energy deposited in the gas; Edep (MeV); BeamEnergy(eV)", 50, 0.0, 0.05, nbinsY, ybins.data());


  for (int i = 0; i<ntuple->GetEntries(); ++i) {
    ntuple->GetEntry(i);
    if (i == 0) {
        prevBeam = beam;
    }
    if (prevID == evID){
      if (strcmp(logical, "gasgapUp") == 0) {
        de += edep;
      }
    } else {
        if (de != 0.) {
          h->Fill(de);
          h2d->Fill(de, prevBeam);
        }       
	de = 0.;
	if (strcmp(logical, "gasgapUp") == 0) {
        de += edep;
	}
      }
    if (i == ntuple->GetEntries() -1) {
        h->Fill(de);
        h2d->Fill(de, beam);
    }
    prevID = evID;
    prevBeam = beam;
  }

  c1->cd();
  h->DrawCopy();
  c2->cd();
  h2d->DrawClone();

  //TFile* outFile = TFile::Open("energy_result.root", "UPDATE");
  //outFile->cd();
  //outFile->Close();
  //delete outFile;
  f->cd();
  delete f;
}








// plot momentum of the gamma produced
void plotGammaMomentum(const char* fileName) {
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  TFile *f = TFile::Open(fileName, "READ");
  if (!f || f->IsZombie()) {
      std::cout << "Errore: Impossibile aprire il file " << fileName << std::endl;
  }

  //std::string outFileName = "";
  // Create a canvas and divide it into 2x2 pads
  TCanvas* c1 = new TCanvas("mom_gamma", "", 20, 20, 1000, 1000);
  c1->Divide(1,2);

  // Get ntuple
  TNtuple* ntuple = (TNtuple*)f->Get("Step");

  // Create an link variable with the ntuple content
  Double_t xmom = 0.;
  Double_t ymom = 0.;
  Double_t zmom = 0.;
  Double_t momTot = 0.;
  Double_t momTot2 = 0.;

  int j = 0;

  Double_t edep = 0.;
  Int_t pdg = 0;
  Int_t track = 0;
  Int_t evID = 0;
  Int_t layer = 0;
  char logical[256];


  Int_t prevID = 0;
  Int_t prevTrack = 0;

  Double_t prevXMom = 0.;
  Double_t prevYMom = 0.;
  Double_t prevZMom = 0.;


  ntuple->SetBranchAddress("Xmom", &xmom);
  ntuple->SetBranchAddress("Ymom", &ymom);
  ntuple->SetBranchAddress("Zmom", &zmom);

  ntuple->SetBranchAddress("Edep", &edep);
  ntuple->SetBranchAddress("PDG", &pdg);
  ntuple->SetBranchAddress("TrackID", &track);
  ntuple->SetBranchAddress("EventID", &evID);
  ntuple->SetBranchAddress("LVName", &logical);


  TH1D* h = new TH1D("mom", "Momentum of photon [MeV/c]", 1000, 0, 10);
  TH1D* h2 = new TH1D("mom_zoom", "Momentum of photon [MeV/c]", 1000, 0, 0.1);

  for (int i = 0; i<ntuple->GetEntries(); ++i) {
    ntuple->GetEntry(i);
  
    if (evID == prevID && track == prevTrack /*&& prevXMom == xmom && prevYMom == ymom && prevZMom == zmom*/){ // commenta la condizione sui momenti se non vuoi contare più volte la stessa particella
    } else {
      if (pdg == 22){
        momTot2 = (xmom * xmom) + (ymom * ymom) + (zmom * zmom);
        momTot = sqrt(momTot2);

        //prevTrack = track;
        //prevID = evID;
        prevXMom = xmom;
        prevYMom = ymom;
        prevZMom = zmom;
        j += 1;
        h->Fill(momTot);
        h2->Fill(momTot);
      }
    }
    

    prevID = evID;
    prevTrack = track;
  }
  std::cout << "Fotoni totali = " << j << std::endl;



  TLine *l = new TLine(0.008, 0, 0.008, 1000);
  l->SetLineColor(kRed);
  l->SetLineStyle(2);

  TFile* outFile = TFile::Open("out.root", "RECREATE");
  outFile->cd();
  h->Write();
  h2->Write();
  c1->cd(1);
  gPad->SetLogy(1);
  h->DrawCopy();
  c1->cd(2);
  gPad->SetLogy(1);
  h2->DrawCopy();
  l->Draw("same");
  outFile->cd();
  c1->Write();
  outFile->Close();
  delete outFile;
  f->cd();
  f->Close();
  delete f;

/*
  c1->cd(1);
  //h->SetDirectory(0);
  h->DrawCopy();
  c1->cd(2);
  //h2->SetDirectory(0);
  h2->DrawCopy();
  //c1->Draw();
  f->cd();
  f->Close();
  delete f;
*/
}







void finalAnalysis() {
    TFile* f = TFile::Open("energy_result.root", "RECREATE");
    f->Close();
    delete f;

    plotPDGstep("B4.root");
}
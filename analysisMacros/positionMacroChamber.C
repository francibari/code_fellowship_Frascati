#include <string>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>


// Questa funzione disegna le posizioni Z degli step e deglli step che hanno rilasciato energia, 
// le posizioni X e Y dei neutroni iniziali (si può cmabiare) e l'energia rilasciata da ogni step (qualsiasi sia la particella e la sua licazione)
void plotZPosition (const char* fileName) {
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  TFile *f = TFile::Open(fileName, "READ");
  if (!f || f->IsZombie()) {
      std::cout << "Errore: Impossibile aprire il file " << fileName << std::endl;
  }

  // Create a canvas and divide it into 2x2 pads
  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  c1->Divide(2,2);

  // Get ntuple
  TNtuple* ntuple = (TNtuple*)f->Get("Step");

  // Draw Eabs histogram in the pad 1
  c1->cd(1);
  ntuple->Draw("Zpos");

  // Draw Labs histogram in the pad 2
  c1->cd(2);
  ntuple->Draw("Zpos", "Edep>0");

  c1->cd(3);
  ntuple->Draw("Xpos:Ypos", "TrackID == 1 && Zpos < -10", "colz");


  // Draw Lgap histogram in the pad 4
  c1->cd(4);
  //set logarithmic scale for y
  //gPad->SetLogy(1); 
  ntuple->Draw("Edep", "Edep>0");

  f->cd();
  f->Close();
  delete f;
}



// plot and save the position of the Z coordinates of the step
void saveZPosition (const char* fileName) {
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  // open file and check if it exists
  TFile *f = TFile::Open(fileName, "READ");
  if (!f || f->IsZombie()) {
    std::cout << "Errore: Impossibile aprire il file " << fileName << std::endl;
  }

  std::string outFileName = std::string("position_") + fileName;
  
  // get the ntuple
  TNtuple* ntuple = (TNtuple*)f->Get("Step");

  // prepare variables we want to read
  Double_t xpos = 0.;
  Double_t ypos = 0.;
  Double_t zpos = 0.;
  Double_t edep = 0.;
  char logical[256];
  Int_t track = 0;

  // link our variable with the ones in the ntuple
  ntuple->SetBranchAddress("Xpos", &xpos);
  ntuple->SetBranchAddress("Ypos", &ypos);
  ntuple->SetBranchAddress("Zpos", &zpos);
  ntuple->SetBranchAddress("Edep", &edep);
  ntuple->SetBranchAddress("LVName", &logical);
  ntuple->SetBranchAddress("TrackID", &track);

  TH1D* h1 = new TH1D ("zpos1", "Z coordinated of steps", 100, -40, 40 );
  TH1D* h2 = new TH1D ("zpos2", "Z coordinated of steps with energy deposited", 100, -40, 40);
  TH1D* h3 = new TH1D ("zpos3", "Z coordinated of steps with energy deposited in the vetronite or copper", 100, -40, 40);
  TH1D* h4 = new TH1D ("zpos4", "Z coordinated of primary particles", 100, -40, 40 );
  TH1D* h5 = new TH1D ("edepStep", "Energy deposited in each step [MeV]", 100, 0, 0.5);

  for (int i = 0; i<ntuple->GetEntries(); ++i) {
    ntuple->GetEntry(i);
    h1->Fill(zpos);
    if (track == 1) {
      h4->Fill(zpos);
    }
    if (edep > 0.) {
      h2->Fill(zpos);
      h5->Fill(edep);
      if (strcmp(logical, "SchiumaLV") != 0 && strcmp(logical, "GasGapLV") != 0 && strcmp(logical, "World") != 0) {
        h3->Fill(zpos);
      }
    }
  
  }

  TFile* outFile = TFile::Open(outFileName.c_str(), "UPDATE");
  outFile->cd();
  h1->Write();
  h2->Write();
  h3->Write();
  h4->Write();
  h5->Write();
  outFile->Close();
  delete outFile;
  f->cd();
  f->Close();
  delete f;
}








void plotVertexPosition(const char* fileName) {
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  
  TFile *f = TFile::Open(fileName, "READ");
  if (!f || f->IsZombie()) {
      std::cout << "Errore: Impossibile aprire il file " << fileName << std::endl;
  }

  // Open csv file to write the info
  std::ofstream csvFile("outMRWell.csv");
  if (!csvFile.is_open()) {
    std::cerr << "Impossibile creare il file CSV!" << std::endl;
    f->Close();
    return;
  }
  csvFile << "PDG,Edep[MeV],XposVertex[mm],YposVertex[mm],ZposVertex[mm],EventID,TrackID\n";

  std::string outFileName = "position_result.root";

  // create the canvas
  TCanvas* c1 = new TCanvas("v_pos", "vertex position and energy released per track", 20, 20, 1000, 1000);
  c1->Divide(2, 2);
  TCanvas* c2 = new TCanvas("v_XS", "vertex position ionization", 20, 20, 1000, 1000);

  // Get ntuple
  TNtuple* ntuple = (TNtuple*)f->Get("Step");

  // Prepare the variable to be read
  Int_t event = -1;
  Int_t track = 0;
  char logical[256];
  char procName[256];
  char preProcName[256];
  Double_t edep = 0.;
  Double_t xpos = 0.;
  Double_t ypos = 0.;
  Double_t zpos = -70.;
  Double_t zpost = 0.;
  Int_t pdg = 0;

  Double_t de = 0.;

  std::vector<Double_t> VecEdep;
  std::vector<std::string> VecLVName;
  std::vector<Double_t> VecZpos;
  std::vector<Double_t> VecXpos;
  std::vector<Double_t> VecYpos;
  std::vector<std::string> VecPreProcName;

  Int_t prevID = -2;
  Int_t prevTrack = -1;
  Int_t prevPDG = 2112;

  bool flag = false;
  TString name;

  ntuple->SetBranchAddress("EventID", &event);
  ntuple->SetBranchAddress("TrackID", &track);
  ntuple->SetBranchAddress("LVName", &logical);
  ntuple->SetBranchAddress("Edep", &edep);
  ntuple->SetBranchAddress("Zpos", &zpos);
  ntuple->SetBranchAddress("Xpos", &xpos);
  ntuple->SetBranchAddress("Ypos", &ypos);
  ntuple->SetBranchAddress("PDG", &pdg);
  ntuple->SetBranchAddress("ProcessName", &procName);
  ntuple->SetBranchAddress("Zpost", &zpost);
  ntuple->SetBranchAddress("PreProcessName", &preProcName);

  TH1D* h = new TH1D("edep", "Energy deposited per track;Edep(MeV);Counts", 500, 0, 0.05);
  //TH1D* h2 = new TH1D ("zpos", "Z coordinated of vertices that release energy in the gap [mm]", 200, -10, 10 );
  THStack* hs = new THStack("hs", "Vertices of tracks that deposit energy in the gas;z(mm);Counts");
  TH1D* pos_ep = new TH1D ("zpos_ep", "", 200, -40, 40 );
  TH1D* pos_heavy = new TH1D ("zpos_heavy", "", 200, -40, 40 );
  TH1D* pos_proton = new TH1D ("zpos_proton", "", 200, -40, 40 );
  TH1D* pos_gamma = new TH1D ("zpos_gamma", "", 200, -40, 40 );

  THStack* es = new THStack("es", "Vertices of electron that release energy in the gas;z(mm);Counts");
  TH1D* e_ion = new TH1D ("e_ion", "", 200, -40, 40 );
  TH1D* e_compt = new TH1D ("e_compt", "", 200, -40, 40 );
  TH1D* e_phot = new TH1D ("e_phot", "", 200, -40, 40 );
  TH1D* e_rad = new TH1D ("e_rad", "", 200, -40, 40 );


  TH1D* pos_primary = new TH1D ("zpos_primary", "Position of primary interaction vertices;z(mm);Counts", 200, -40, 40 );

  TH2D* h2_ion = new TH2D("h2_ion", "Vertices of electron; X(mm);Y(mm)", 4000, -2000.0, 2000.0, 3000, -1500.0, 1500.0);
  TH2D* h2_compt = new TH2D("h2_compt", "", 4000, -2000.0, 2000.0, 3000, -1500.0, 1500.0);
  TH2D* h2_phot = new TH2D("h2_phot", "", 4000, -2000.0, 2000.0, 3000, -1500.0, 1500.0);
  TH2D* h2_rad = new TH2D("h2_rad", "", 4000, -2000.0, 2000.0, 3000, -1500.0, 1500.0);

  


  for (int i = 0; i<ntuple->GetEntries(); ++i) {
    ntuple->GetEntry(i);
    //std::cout<< "-----------------------------------------------------------------------------------" << std::endl;
    //std::cout<< "Row = " << i << std::endl;
    //std::cout<< "Event = " << event << std::endl;
    //std::cout<< "PrecEvent = " << prevID << std::endl;
    //std::cout<< "Track = " << track << std::endl;
    //std::cout<< "PrecTrack = " << prevTrack << std::endl;
    //std::cout<< "Edep = " << edep << std::endl;
    //std::cout<< "LVName = " << logical << std::endl;
    if (track == 1){
      if (strcmp(procName, "noProcess") != 0 && strcmp(procName, "Transportation") != 0) {
        pos_primary->Fill(zpost);
        //std::cout<<"procName = " << procName << std::endl;
        //std::cout<<"zpost = " << zpost << std::endl;
      }
    }
    if (prevID == event && prevTrack==track) {
      //std::cout<< "Faccio push_back sui vettori" << std::endl;
      // fai un push_back sul vettore
      VecEdep.push_back(edep);
      VecLVName.push_back(logical);
      VecZpos.push_back(zpos);
      VecXpos.push_back(xpos);
      VecYpos.push_back(ypos);
      VecPreProcName.push_back(preProcName);
    } else {
      //std::cout<< "Size Vettori = " << VecEdep.size() << std::endl;
      for (int j = 0; j<VecEdep.size(); ++j) {
        if ( VecLVName[j] == "GasGapLV" ) {
          if (VecEdep[j]>0.) {
            //std::cout<<"Riempio l'istogramma con l'energia " << VecEdep[j] << std::endl;
            //h->Fill(VecEdep[j]);
            de += VecEdep[j];
            if (flag == false) {
              //h2->Fill(VecZpos[0]);
              switch (prevPDG) {
                case 11:
                pos_ep->Fill(VecZpos[0]);
                if (VecPreProcName[0] == "eIoni" || VecPreProcName[0] == "hIoni" || VecPreProcName[0] == "ionIoni") {
                  e_ion->Fill(VecZpos[0]);
                  h2_ion->Fill(VecXpos[0], VecYpos[0]);
                }
                if (VecPreProcName[0] == "phot") {
                  e_phot->Fill(VecZpos[0]);
                  h2_phot->Fill(VecXpos[0], VecYpos[0]);
                }
                if (VecPreProcName[0] == "compt") {
                  e_compt->Fill(VecZpos[0]);
                  h2_compt->Fill(VecXpos[0], VecYpos[0]);
                }
                if (VecPreProcName[0] == "Radioactivation") {
                  e_rad->Fill(VecZpos[0]);
                  h2_rad->Fill(VecXpos[0], VecYpos[0]);
                }
                std::cout<<"--------------- name = " << VecPreProcName[0] <<std::endl;
                break;
                case -11:
                pos_ep->Fill(VecZpos[0]);
                if (VecPreProcName[0] == "eIoni" || VecPreProcName[0] == "hIoni" || VecPreProcName[0] == "ionIoni") {
                  e_ion->Fill(VecZpos[0]);
                  h2_ion->Fill(VecXpos[0], VecYpos[0]);
                }
                if (VecPreProcName[0] == "phot") {
                  e_phot->Fill(VecZpos[0]);
                  h2_phot->Fill(VecXpos[0], VecYpos[0]);
                }
                if (VecPreProcName[0] == "compt") {
                  e_compt->Fill(VecZpos[0]);
                  h2_compt->Fill(VecXpos[0], VecYpos[0]);
                }
                if (VecPreProcName[0] == "Radioactivation") {
                  e_rad->Fill(VecZpos[0]);
                  h2_rad->Fill(VecXpos[0], VecYpos[0]);
                }
                std::cout<<"----------------- name = " << VecPreProcName[0] <<std::endl;
                break;
                case 22:
                pos_gamma->Fill(VecZpos[0]);
                std::cout<< "-----------------------------------------------------------------------------------" << std::endl;
                std::cout<< "prevID = " << prevID <<std::endl;
                std::cout<< "prevPDG = " << prevPDG <<std::endl;
                std::cout<< "PDG = " << pdg <<std::endl;
                break;
                case 2212:
                pos_proton->Fill(VecZpos[0]);
                break;
                default:
                pos_heavy->Fill(VecZpos[0]);
                break;
              }
              flag = true;
            }
            // attenzione a dove vai a riempire il csv e al valore delle variabilil in quel punto
            csvFile << prevPDG << "," << VecEdep[j] << "," << VecXpos[0] << "," << VecYpos[0] << "," << VecZpos[0] << "," << prevID << "," << prevTrack << "\n"; 
          }
        }
        if (j == VecEdep.size()-1 && de>0) {
          h->Fill(de);
          de = 0.;
        }
      }
      // clear the vectors
      VecEdep.clear();
      VecLVName.clear();
      VecZpos.clear();
      VecXpos.clear();
      VecYpos.clear();
      VecPreProcName.clear();
      // fill the vector with the first occurrence
      VecEdep.push_back(edep);
      VecLVName.push_back(logical);
      VecZpos.push_back(zpos);
      VecXpos.push_back(xpos);
      VecYpos.push_back(ypos);
      VecPreProcName.push_back(preProcName);
      flag = false;
    }
    prevID = event;
    prevTrack = track;
    prevPDG = pdg;
  }

  // leggo l'ultima traccia che sennò andrebbe persa
  //std::cout<< "Size Vettori = " << VecEdep.size() << std::endl;
  for (int j = 0; j<VecEdep.size(); ++j) {
    if ( VecLVName[j] == "GasGapLV" ) {
      if (VecEdep[j]>0.) {
        //std::cout<<"Riempio l'istogramma con l'energia " << VecEdep[j] << std::endl;
        h->Fill(VecEdep[j]);
        if (flag == false) {
          //h2->Fill(VecZpos[0]);
          switch (prevPDG) {
                case 11:
                pos_ep->Fill(VecZpos[0]);
                if (VecPreProcName[0] == "eIoni" || VecPreProcName[0] == "hIoni" || VecPreProcName[0] == "ionIoni") {
                  e_ion->Fill(VecZpos[0]);
                  h2_ion->Fill(VecXpos[0], VecYpos[0]);
                }
                if (VecPreProcName[0] == "phot") {
                  e_phot->Fill(VecZpos[0]);
                  h2_phot->Fill(VecXpos[0], VecYpos[0]);
                }
                if (VecPreProcName[0] == "compt") {
                  e_compt->Fill(VecZpos[0]);
                  h2_compt->Fill(VecXpos[0], VecYpos[0]);
                }
                if (VecPreProcName[0] == "Radioactivation") {
                  e_rad->Fill(VecZpos[0]);
                  h2_rad->Fill(VecXpos[0], VecYpos[0]);
                }
                break;
                case -11:
                pos_ep->Fill(VecZpos[0]);
                if (VecPreProcName[0] == "eIoni" || VecPreProcName[0] == "hIoni" || VecPreProcName[0] == "ionIoni") {
                  e_ion->Fill(VecZpos[0]);
                  h2_ion->Fill(VecXpos[0], VecYpos[0]);
                }
                if (VecPreProcName[0] == "phot") {
                  e_phot->Fill(VecZpos[0]);
                  h2_phot->Fill(VecXpos[0], VecYpos[0]);
                }
                if (VecPreProcName[0] == "compt") {
                  e_compt->Fill(VecZpos[0]);
                  h2_compt->Fill(VecXpos[0], VecYpos[0]);
                }
                if (VecPreProcName[0] == "Radioactivation") {
                  e_rad->Fill(VecZpos[0]);
                  h2_rad->Fill(VecXpos[0], VecYpos[0]);
                }
                break;
                case 22:
                pos_gamma->Fill(VecZpos[0]);
                break;
                case 2212:
                pos_proton->Fill(VecZpos[0]);
                break;
                default:
                pos_heavy->Fill(VecZpos[0]);
                break;
              }
          flag = true;
        }
        csvFile << prevPDG << "," << VecEdep[j] << "," << VecXpos[0] << "," << VecYpos[0] << "," << VecZpos[0] << "," << prevID << "," << prevTrack << "\n";
      }
    }
    if (j == VecEdep.size()-1 && de>0) {
      h->Fill(de);
      de = 0.;
    }
  }

  csvFile.close();
  

  // setto i colori che riempiono gli istogrammi
  pos_ep->SetFillColor(kBlue);
  pos_gamma->SetFillColor(kRed);
  pos_heavy->SetFillColor(kGreen);
  pos_proton->SetFillColor(kOrange);
  
  // li aggiungo allo stack
  hs->Add(pos_proton);
  hs->Add(pos_gamma);  
  hs->Add(pos_ep);
  hs->Add(pos_heavy);


  e_ion->SetFillColor(kRed);
  e_phot->SetFillColor(kBlue);
  e_compt->SetFillColor(kGreen);
  e_rad->SetFillColor(kOrange);

  es->Add(e_ion);  
  es->Add(e_phot);
  es->Add(e_compt);
  es->Add(e_rad);


  std::cout<< "Entries = " << pos_gamma->GetEntries() << "+" << pos_heavy->GetEntries() <<"+"<< pos_proton->GetEntries() <<"+"<< pos_ep->GetEntries() << std::endl;
  std::cout<< "Entries electron : " << std::endl;
  std::cout<<"Ionization = " << e_ion->GetEntries()<<std::endl;
  std::cout<<"Compton = "<< e_compt->GetEntries()<<std::endl;
  std::cout<<"Photoelectric = " << e_phot->GetEntries() << std::endl;
  std::cout<<"Radioactive = " << e_rad->GetEntries() << std::endl;

  c1->cd(1);
  h->Draw();
  c1->cd(2);
  hs->Draw();
  c1->cd(3);
  es->Draw();
  c1->cd(4);
  pos_primary->Draw();


  h2_ion->SetMarkerColor(kRed);
  h2_phot->SetMarkerColor(kBlue);
  h2_compt->SetMarkerColor(kGreen);
  h2_rad->SetMarkerColor(kOrange);

  c2->cd();
  h2_ion->Draw();
  h2_compt->Draw("same");
  h2_phot->Draw("same");
  h2_rad->Draw("same");



  TFile* outFile = TFile::Open("position_result.root", "RECREATE");
  c1->Write();
  c2->Write();
  outFile->Close();
  delete outFile;
  
  f->Close();
  delete f;

}










// plot z position of the produced gamma
void plotZposGamma(const char* fileName) {
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  TFile *f = TFile::Open(fileName, "READ");
  if (!f || f->IsZombie()) {
      std::cout << "Errore: Impossibile aprire il file " << fileName << std::endl;
  }

  std::string outFileName = std::string("res_") + fileName;

  // Get ntuple
  TNtuple* ntuple = (TNtuple*)f->Get("Step");

  // Create an link variable with the ntuple content
  Double_t zpos = 0.;

  int j = 0;

  Int_t pdg = 0;
  Int_t track = 0;
  Int_t evID = 0;


  Int_t prevID = 0;
  Int_t prevTrack = 0;



  ntuple->SetBranchAddress("Zpos", &zpos);
  ntuple->SetBranchAddress("PDG", &pdg);
  ntuple->SetBranchAddress("TrackID", &track);
  ntuple->SetBranchAddress("EventID", &evID);

  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);

  TH1D* h = new TH1D("zposGamma", "Position of produced photon [mm]", 1000, -40, 40);

  for (int i = 0; i<ntuple->GetEntries(); ++i) {
    ntuple->GetEntry(i);
  
    if (evID != prevID || track != prevTrack){ 
      if (pdg == 22){
        h->Fill(zpos);
        ++j;
      }
    }

    prevID = evID;
    prevTrack = track;
    
  }
  std::cout << "Fotoni totali = " << j << std::endl;

  TLine *l01 = new TLine(-35.15, 0, -35.15, 10000);
  TLine *l02 = new TLine(-35.12, 0, -35.12, 10000);
  TLine *l03 = new TLine(-34.32, 0, -34.32, 10000);
  TLine *l04 = new TLine(-26.92, 0, -26.92, 10000);
  TLine *l05 = new TLine(-26.12, 0, -26.12, 10000);
  TLine *l06 = new TLine(-26.09, 0, -26.09, 10000);
  
  TLine *l11 = new TLine(-21.09, 0, -21.09, 10000);
  TLine *l12 = new TLine(-21.06, 0, -21.06, 10000);
  TLine *l13 = new TLine(-20.26, 0, -20.26, 10000);
  TLine *l14 = new TLine(-12.86, 0, -12.86, 10000);
  TLine *l15 = new TLine(-12.06, 0, -12.06, 10000);
  TLine *l16 = new TLine(-12.03, 0, -12.03, 10000);

  TLine *l21 = new TLine(-7.03, 0, -7.03, 10000);
  TLine *l22 = new TLine(-7, 0, -7, 10000);
  TLine *l23 = new TLine(-6.2, 0, -6.2, 10000);
  TLine *l24 = new TLine(1.2, 0, 1.2, 10000);
  TLine *l25 = new TLine(2, 0, 2, 10000);
  TLine *l26 = new TLine(2.03, 0, 2.03, 10000);

  TLine *l31 = new TLine(7.03, 0, 7.03, 10000);
  TLine *l32 = new TLine(7.06, 0, 7.06, 10000);
  TLine *l33 = new TLine(7.86, 0, 7.86, 10000);
  TLine *l34 = new TLine(15.26, 0, 15.26, 10000);
  TLine *l35 = new TLine(16.06, 0, 16.06, 10000);
  TLine *l36 = new TLine(16.09, 0, 16.09, 10000);

  TLine *l41 = new TLine(21.09, 0, 21.09, 10000);
  TLine *l42 = new TLine(21.12, 0, 21.12, 10000);
  TLine *l43 = new TLine(21.92, 0, 21.92, 10000);
  TLine *l44 = new TLine(29.32, 0, 29.32, 10000);
  TLine *l45 = new TLine(30.12, 0, 30.12, 10000);
  TLine *l46 = new TLine(30.15, 0, 30.15, 10000);

  TLine *lf = new TLine(35.15, 0, 35.15, 10000);


  l01->SetLineColor(kBlue);
  l11->SetLineColor(kBlue);
  l21->SetLineColor(kBlue);
  l31->SetLineColor(kBlue);
  l41->SetLineColor(kBlue);
  lf->SetLineColor(kBlue);
  
  l06->SetLineColor(kBlue);
  l16->SetLineColor(kBlue);
  l26->SetLineColor(kBlue);
  l36->SetLineColor(kBlue);
  l46->SetLineColor(kBlue);
    
  l02->SetLineColor(kRed);
  l12->SetLineColor(kRed);
  l22->SetLineColor(kRed);
  l32->SetLineColor(kRed);
  l42->SetLineColor(kRed);
  
  l05->SetLineColor(kRed);
  l15->SetLineColor(kRed);
  l25->SetLineColor(kRed);
  l35->SetLineColor(kRed);
  l45->SetLineColor(kRed);

  l03->SetLineColor(kGreen);
  l13->SetLineColor(kGreen);
  l23->SetLineColor(kGreen);
  l33->SetLineColor(kGreen);
  l43->SetLineColor(kGreen);
  
  l04->SetLineColor(kGreen);
  l14->SetLineColor(kGreen);
  l24->SetLineColor(kGreen);
  l34->SetLineColor(kGreen);
  l44->SetLineColor(kGreen);
  



  // set line style, dotted for the 4,5,6 lines
  l06->SetLineStyle(2);
  l16->SetLineStyle(2);
  l26->SetLineStyle(2);
  l36->SetLineStyle(2);
  l46->SetLineStyle(2);
  
  l05->SetLineStyle(2);
  l15->SetLineStyle(2);
  l25->SetLineStyle(2);
  l35->SetLineStyle(2);
  l45->SetLineStyle(2);
  
  l04->SetLineStyle(2);
  l14->SetLineStyle(2);
  l24->SetLineStyle(2);
  l34->SetLineStyle(2);
  l44->SetLineStyle(2);
 


  h->DrawCopy(); 
  l01->Draw();
  l02->Draw();
  l03->Draw();
  l04->Draw();
  l05->Draw();
  l06->Draw();

  l11->Draw();
  l12->Draw();
  l13->Draw();
  l14->Draw();
  l15->Draw();
  l16->Draw();  

  l21->Draw();
  l22->Draw();
  l23->Draw();
  l24->Draw();
  l25->Draw();
  l26->Draw();

  l31->Draw();
  l32->Draw();
  l33->Draw();
  l34->Draw();
  l35->Draw();
  l36->Draw();

  l41->Draw();
  l42->Draw();
  l43->Draw();
  l44->Draw();
  l45->Draw();
  l46->Draw();

  lf->Draw();

  c1->SetLogy();

  TFile* outFile = TFile::Open(outFileName.c_str(), "UPDATE");
  outFile->cd();
  c1->Write();
  outFile->Close();
  delete outFile;
  
  c1->cd();
  c1->Close();
  delete c1;
  
  f->cd();
  f->Close();
  delete f;

}





// questa funzione cerca l'origine degli elettroni che rilasciano energia nel gas
void searchOrigin() {
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  TFile *f = TFile::Open("B4.root", "READ");
  if (!f || f->IsZombie()) {
      std::cout << "Errore: Impossibile aprire il file" << std::endl;
  }


  // Get ntuple
  TNtuple* ntuple = (TNtuple*)f->Get("Step");

  // Create an link variable with the ntuple content
  Double_t xpos = 0.;
  Double_t ypos = 0.;
  Double_t zpos = 0.;
  Double_t xpost = 0.;
  Double_t ypost = 0.;
  Double_t zpost = 0.;

  int j = 0;
  int b = 0;

  Double_t edep = 0.;
  Int_t pdg = 0;
  Int_t track = 0;
  Int_t motherTrack = 0;
  Int_t evID = 0;
  char logical[256];
  char procName[256];


  Int_t prevID = 0;
  Int_t prevTrack = 1;
  Double_t prevZpos = 0.;

  std::vector<Double_t> VecZpos;
  std::vector<Double_t> VecZpost;
  std::vector<Int_t> VecPDG;
  std::vector<Int_t> VecTID;
  std::vector<Int_t> VecMotherTID;
  std::vector<std::string> VecName;
  std::vector<int> Veci;


  ntuple->SetBranchAddress("Xpos", &xpos);
  ntuple->SetBranchAddress("Ypos", &ypos);
  ntuple->SetBranchAddress("Zpos", &zpos);
  ntuple->SetBranchAddress("Xpost", &xpost);
  ntuple->SetBranchAddress("Ypost", &ypost);
  ntuple->SetBranchAddress("Zpost", &zpost);

  ntuple->SetBranchAddress("Edep", &edep);
  ntuple->SetBranchAddress("PDG", &pdg);
  ntuple->SetBranchAddress("TrackID", &track);
  ntuple->SetBranchAddress("EventID", &evID);
  ntuple->SetBranchAddress("LVName", &logical);
  ntuple->SetBranchAddress("ProcessName", &procName);
  ntuple->SetBranchAddress("MotherTrackID", &motherTrack);


  for (int i = 0; i<1000000; i++) {
    ntuple->GetEntry(i);
    if (prevID == evID) {
      VecZpos.push_back(zpos);
      VecZpost.push_back(zpost);
      VecName.push_back(procName);
      VecPDG.push_back(pdg);
      VecTID.push_back(track);
      Veci.push_back(i);
      //if (prevTrack != track) {
        if (edep > 0. && strcmp(logical, "gasgapUp") == 0) {
          ++b;
          for (int a = 0; a < VecZpos.size()-1; a++) {
            if (VecZpost[a] == zpos && VecTID[a] == motherTrack) {
              // stampa o tira fuori info su PDG madre e processo
              std::cout<<"Riga = " << i << std::endl;
              std::cout<<"Evento = " << evID << std::endl;
              std::cout<<"Traccia = " << track << std::endl;
              std::cout<<"PDG = " << pdg << std::endl;
              std::cout<<"Processo = " << VecName[a] << std::endl;
              std::cout<<"Particella madre = " << VecPDG[a] << std::endl;
              std::cout<<"Volume = " << logical << std::endl;
              std::cout<<"Zpos = " << zpos << std::endl;
              std::cout<<"Zpost = " << VecZpost[a] << std::endl;
              std::cout<<"VecTID = " << VecTID[a] << std::endl;
              std::cout<<"MotherTrack = " << motherTrack << std::endl;
              std::cout<<"a = " << a << std::endl;
              std::cout<<"riga madre = " << Veci[a]<<std::endl;
              std::cout<<"---------------------------------------------------------------------"<< std::endl;
              ++j;
            }
          }
        }
      //}
    } else {
      VecZpos.clear();
      VecZpost.clear();
      VecPDG.clear();
      VecTID.clear();
      VecMotherTID.clear();
      VecName.clear();
      Veci.clear();
    }
    
    prevTrack = track;
    prevID = evID;

  }

  std::cout<<"Conteggi j = "<<j<<std::endl;
  std::cout<<"Conteggi b = "<<b<<std::endl;

}

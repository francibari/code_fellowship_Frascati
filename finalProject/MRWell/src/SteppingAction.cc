//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B4/B4a/src/SteppingAction.cc
/// \brief Implementation of the B4a::SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

using namespace B4;

namespace B4c
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
// Collect energy and track length step by step
  
  // get touchable of the current step
  if (step == nullptr){
    G4cout << "LO STEP è nullptr" << G4endl;
    return;
  }
  if (step->GetPreStepPoint() == nullptr){
    G4cout << "IL PRESTEPPOINT è nullptr" << G4endl;
    return;
  }
  auto touchable = step->GetPreStepPoint()->GetTouchable();
  if (touchable == nullptr) {
    G4cout << "IL TOUCHABLE è nullptr" << G4endl;
    return;
  }
  
  //auto replica = touchable->GetCopyNumber(1);
  auto edep = step->GetTotalEnergyDeposit();
  auto pdg = step->GetTrack()->GetDefinition()->GetPDGEncoding();
  auto TID = step->GetTrack()->GetTrackID();
  auto motherTID = step->GetTrack()->GetParentID();

  // info about position and momentum of the particle in the step
  auto xpos = step->GetPreStepPoint()->GetPosition().x();
  auto ypos = step->GetPreStepPoint()->GetPosition().y();
  auto zpos = step->GetPreStepPoint()->GetPosition().z();
  auto xmom = step->GetPreStepPoint()->GetMomentum().x();
  auto ymom = step->GetPreStepPoint()->GetMomentum().y();
  auto zmom = step->GetPreStepPoint()->GetMomentum().z();

  auto xpost = step->GetPostStepPoint()->GetPosition().x();
  auto ypost = step->GetPostStepPoint()->GetPosition().y();
  auto zpost = step->GetPostStepPoint()->GetPosition().z();


  // int index = -1;
  auto event = G4EventManager::GetEventManager()->GetConstCurrentEvent();
  G4int eventID = event->GetEventID();

  auto beamEnergy = event->GetPrimaryVertex(0)->GetPrimary()->GetKineticEnergy() / CLHEP::eV;

  auto processName = "prova";
  processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  
  // aggiunta per trovare il nome del processo che origina la particella figlia
  G4String preProcName = "none";
  auto track = step->GetTrack();
  if (track->GetParentID() > 0) {
    const G4VProcess* creatorProc = track->GetCreatorProcess();
    if (creatorProc != nullptr) {
      preProcName = creatorProc->GetProcessName();
    }
  }
  
  
  
  // SECONDO ME L'ERRORE STA IN QUESTE RIGHE DI CODICE E IN COME GESTISTO I PUNTATORI
  auto volume = touchable->GetVolume();
  if (volume == nullptr) {
    G4cout << "NON C'è IL VOLUME CORRENTE" << G4endl;
    return;
  }
  //auto LVname = "prova";
  int replica = -1;
  auto LVname = volume->GetLogicalVolume()->GetName();


  //G4double edep = 0.;
  //G4int pdg = 0;
  //G4int TID = 0;
  //G4int eventID = 0;
  //G4int motherTID = 0;
  
  //G4double xpos = 0.;
  //G4double ypos = 0.;
  //G4double zpos = 0.;
  //G4double xmom = 0.;
  //G4double ymom = 0.;
  //G4double zmom = 0.; 
  //G4double beamEnergy = 0.;

  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->FillNtupleDColumn(0, edep);
  analysisManager->FillNtupleIColumn(1, pdg);
  analysisManager->FillNtupleIColumn(2, TID);
  analysisManager->FillNtupleIColumn(3, eventID);
  analysisManager->FillNtupleSColumn(4, LVname);
  analysisManager->FillNtupleDColumn(5, beamEnergy);
  analysisManager->FillNtupleIColumn(6, motherTID);

  analysisManager->FillNtupleDColumn(7, xpos);
  analysisManager->FillNtupleDColumn(8, ypos);
  analysisManager->FillNtupleDColumn(9, zpos);
  analysisManager->FillNtupleDColumn(10, xmom);
  analysisManager->FillNtupleDColumn(11, ymom);
  analysisManager->FillNtupleDColumn(12, zmom);

  analysisManager->FillNtupleDColumn(13, xpost);
  analysisManager->FillNtupleDColumn(14, ypost);
  analysisManager->FillNtupleDColumn(15, zpost);

  analysisManager->FillNtupleSColumn(16, processName);
  analysisManager->FillNtupleSColumn(17, preProcName);


  analysisManager->AddNtupleRow();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

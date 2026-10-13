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
/// \file B4/B4c/include/CalorHit.hh
/// \brief Definition of the B4c::CalorHit class

#ifndef B4cCalorHit_h
#define B4cCalorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

#include <vector>

namespace B4c
{

/// Calorimeter hit class
///
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class CalorHit : public G4VHit
{
  public:
    CalorHit() = default;
    CalorHit(const CalorHit&) = default;
    ~CalorHit() override = default;

    // operators
    CalorHit& operator=(const CalorHit&) = default;
    G4bool operator==(const CalorHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    void Draw()  override{}
    void Print() override;

    // methods to handle data
    void AddStep(G4int pdg, G4double edep, G4int trackID, G4int replica, G4int motherTID, 
                              G4double xpos, G4double ypos, G4double zpos, G4double xmom, G4double ymom, G4double zmom);

    // get methods
    G4double GetEdep() const { return fEdep; };
    const std::vector<G4int>& GetPDGVector() const { return fPDGVector; };
    const std::vector<G4double>& GetEdepVector() const { return fEdepVector; };
    const std::vector<G4int>& GetTrackIDVector() const { return fTrackIDVector; };
    const std::vector<G4int>& GetReplicaVector() const { return fReplicaVector; };
    const std::vector<G4int>& GetMotherTrackIDVector() const { return fMotherTrackIDVector; };
    const std::vector<G4double>& GetXPositionVector() const { return fXPositionVector; };
    const std::vector<G4double>& GetYPositionVector() const { return fYPositionVector; };
    const std::vector<G4double>& GetZPositionVector() const { return fZPositionVector; };
    const std::vector<G4double>& GetXMomentumVector() const { return fXMomentumVector; };
    const std::vector<G4double>& GetYMomentumVector() const { return fYMomentumVector; };
    const std::vector<G4double>& GetZMomentumVector() const { return fZMomentumVector; };

  private:
    G4double fEdep; // energia totale rilasciata nella hit
    std::vector<G4int> fPDGVector;  // vettore con PDG delle particelle che rilasciano energia
    std::vector<G4double> fEdepVector; // vettore con energia delle particelle che rilasciano energia
    std::vector<G4int> fTrackIDVector; // vettore con ID traccia delle particelle che rilasciano energia
    std::vector<G4int> fReplicaVector;  // vettore con numero replica layer
    std::vector<G4int> fMotherTrackIDVector; // vettore con ID traccia delle particelle madri
    std::vector<G4double> fXPositionVector; // vettore con posizione X di interazione
    std::vector<G4double> fYPositionVector; // vettore con posizione Y di interazione
    std::vector<G4double> fZPositionVector; // vettore con posizione Z di interazione
    std::vector<G4double> fXMomentumVector; // vettore con coord X di momento
    std::vector<G4double> fYMomentumVector; // vettore con coord Y di momento
    std::vector<G4double> fZMomentumVector; // vettore con coord Z di momento
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using CalorHitsCollection = G4THitsCollection<CalorHit>;

extern G4ThreadLocal G4Allocator<CalorHit>* CalorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* CalorHit::operator new(size_t)
{
  if (!CalorHitAllocator) {
    CalorHitAllocator = new G4Allocator<CalorHit>;
  }
  void *hit;
  hit = (void *) CalorHitAllocator->MallocSingle();
  return hit;
}

inline void CalorHit::operator delete(void *hit)
{
  if (!CalorHitAllocator) {
    CalorHitAllocator = new G4Allocator<CalorHit>;
  }
  CalorHitAllocator->FreeSingle((CalorHit*) hit);
}

inline void CalorHit::AddStep(G4int pdg, G4double edep, G4int trackID, G4int replica, G4int motherTID, 
                              G4double xpos, G4double ypos, G4double zpos, G4double xmom, G4double ymom, G4double zmom) {
  fEdep += edep;
  fPDGVector.push_back(pdg);
  fEdepVector.push_back(edep);
  fTrackIDVector.push_back(trackID);
  fReplicaVector.push_back(replica);
  fMotherTrackIDVector.push_back(motherTID);
  fXPositionVector.push_back(xpos);
  fYPositionVector.push_back(ypos);
  fZPositionVector.push_back(zpos);
  fXMomentumVector.push_back(xmom);
  fYMomentumVector.push_back(ymom);
  fZMomentumVector.push_back(zmom);
}



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

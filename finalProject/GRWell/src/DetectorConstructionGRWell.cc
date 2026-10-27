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
/// \file B4/MRWELL/src/DetectorConstruction.cc
/// \brief Implementation of the MRWELL::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "CalorimeterSD.hh"

#include "G4AutoDelete.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4PhysicalConstants.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//                   DETECTOR CONSTRUCTION OF THE GRWell
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



namespace B4c
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  // Lead material defined using NIST Manager
  auto nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Pb");

  // Liquid argon material
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;
  G4double density;
  new G4Material("liquidArgon", z = 18., a = 39.95 * g / mole, density = 1.390 * g / cm3);
  // The argon by NIST Manager is a gas with a different density

  // Vacuum
  new G4Material("Galactic", z = 1., a = 1.01 * g / mole, density = universe_mean_density,
                 kStateGas, 2.73 * kelvin, 3.e-18 * pascal);



  
  nistManager->FindOrBuildMaterial("G4_Cu");
  nistManager->FindOrBuildMaterial("G4_CARBON_DIOXIDE CO_2");

  // G4double a;  // mass of a mole;
  // G4double z;  // z=mean number of protons;  
  // G4double density; 
  G4int natoms;
  
  G4double numel;
  G4double fractionMass;
  G4Element* elH = new G4Element("Hydrogen", "H", z=1., a = 1.01*g/mole);
  G4Element* elC = new G4Element("Carbon", "C", z=6., a = 12.0*g/mole);
  G4Element* elO = new G4Element("Oxygen", "O", z=8., a = 16.0*g/mole);
  G4Element* elN = new G4Element("Nitrogen", "N", z=7., a=14.01*g/mole);
  G4Element* elF = new G4Element("Fluorine", "F", z=9., a = 18.998*g/mole);
  G4Element* elSi = new G4Element("Silicon", "Si", z=14., a = 28.085*g/mole);
  G4Element* elB = new G4Element("Boron", "B", z=5., a = 10.81*g/mole);
  
  // gas delle gap (GasMaterial)
  // costituenti
  G4Material* matH = nistManager->FindOrBuildMaterial("G4_H");
  G4Material* matC = nistManager->FindOrBuildMaterial("G4_C");
  G4Material* matN = nistManager->FindOrBuildMaterial("G4_N");
  G4Material* matO = nistManager->FindOrBuildMaterial("G4_O");
  G4Material* matNa = nistManager->FindOrBuildMaterial("G4_Na");
  G4Material* matAl = nistManager->FindOrBuildMaterial("G4_Al");
  G4Material* matSi = nistManager->FindOrBuildMaterial("G4_Si");
  G4Material* matAr = nistManager->FindOrBuildMaterial("G4_Ar");
  G4Material* matCa = nistManager->FindOrBuildMaterial("G4_Ca");
  G4Material* matFe = nistManager->FindOrBuildMaterial("G4_Fe");
  G4Material* matMg = nistManager->FindOrBuildMaterial("G4_Mg");
  G4Material* matB = nistManager->FindOrBuildMaterial("G4_B");
  G4Material* matK = nistManager->FindOrBuildMaterial("G4_K");
  G4Material* matTi = nistManager->FindOrBuildMaterial("G4_Ti");
  G4Material* matF = nistManager->FindOrBuildMaterial("G4_F");
  G4Material* CO2 = nistManager->FindOrBuildMaterial("G4_CARBON_DIOXIDE");
  
  G4Material* CF4 = new G4Material("CF4" , density = 0.00372*g/cm3, numel=2);
  CF4->AddElement(elC, natoms=1);
  CF4->AddElement(elF, natoms=4);
  
  // Miscela Ar:CO2:CF4=45:15:40 in volume
  G4Material* gasMaterial = new G4Material("GasMaterial", density=0.001764*g/cm3, numel=3);
  gasMaterial->AddMaterial(matAr, fractionMass=0.30);
  gasMaterial->AddMaterial(CO2, fractionMass=0.11);
  gasMaterial->AddMaterial(CF4, fractionMass=0.59);
  
  
  // Vetronite secondo una abbastanza accurata descrizione di (Atlas-cms-LHCb)
  G4Material* Vetronite =  new G4Material("Vetronite", 1.86*g/cm3, 14);
  Vetronite->AddMaterial(matH,  0.0343);
  Vetronite->AddMaterial(matC,  0.2753);
  Vetronite->AddMaterial(matO,  0.3606);
  Vetronite->AddMaterial(matSi, 0.1514);
  Vetronite->AddMaterial(matB,  0.0140);
  Vetronite->AddMaterial(matAl, 0.0413);
  Vetronite->AddMaterial(matCa, 0.0815);
  Vetronite->AddMaterial(matMg, 0.0090);
  Vetronite->AddMaterial(matTi, 0.0029);
  Vetronite->AddMaterial(matNa, 0.0050);
  Vetronite->AddMaterial(matK, 0.0050);
  Vetronite->AddMaterial(matFe, 0.0021);
  Vetronite->AddMaterial(matF, 0.0042);
  Vetronite->AddMaterial(matN, 0.0142);

  // vetronite a cui abbiamo arricchito il Boro per aumentare la statistica delle alpha
  G4Material* VetBorata =  new G4Material("VetBorata", 1.86*g/cm3, 2);
  VetBorata->AddMaterial(Vetronite, fractionMass=0.5);
  VetBorata->AddMaterial(matB, fractionMass=0.5);
  
  // Define Kapton Material
  G4Material* matKapton = nistManager->FindOrBuildMaterial("G4_KAPTON");
  density = 1.42*g/cm3;
  G4int nel = 4;
  G4Material* kaptonMaterial = new G4Material("KaptonMaterial", density, nel);
  kaptonMaterial->AddElement(elC, 22);
  kaptonMaterial->AddElement(elH, 10);
  kaptonMaterial->AddElement(elN, 2);
  kaptonMaterial->AddElement(elO, 5);
  
  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);


  
  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}





  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // Geometry parameters
  fNofLayers = 10;
  G4double culayThickness1 = 0.005*mm;
  G4double culayThickness2 = 0.035*mm;
  G4double FR4UpThickness = 3.2*mm;
  G4double FR4DownThickness = 3.2*mm;
  G4double FR4PrepregThickness = 0.05*mm;
  G4double gasgapUpThickness = 3.0*mm;
  G4double gasgapDownThickness = 1.0*mm;
  G4double KaptonThickness = 0.05*mm;
 
  G4double chaSizeX = 300.*mm;
  G4double chaSizeY = 250.*mm;
  //G4double chaThickness = 30.*mm;

  G4double fieldThickness = 20.*mm;
 
  
  auto worldSizeX = 2 * chaSizeX;
  auto worldSizeY = 2 * chaSizeY;
  auto worldSizeZ  = 100 * cm; 
  
  // Get materials
  auto defaultMaterial = G4Material::GetMaterial("Galactic");
  auto culayMaterial = G4Material::GetMaterial("G4_Cu");
  auto vetronMaterial = G4Material::GetMaterial("Vetronite");
  auto gasMaterial = G4Material::GetMaterial("GasMaterial");
  auto kaptonMaterial = G4Material::GetMaterial("G4_KAPTON");
  //auto kaptonMaterial = G4Material::GetMaterial("KaptonMaterial");

  G4double zpos;

  //  printf("panel thickness: %f\n",panelThickness);
  
  if ( ! defaultMaterial || ! culayMaterial || ! vetronMaterial || ! gasMaterial || ! kaptonMaterial) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("B4DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }  
   
  //     
  // World
  //
  auto worldS = new G4Box("World", worldSizeX, worldSizeY, worldSizeZ); // its size
  
  auto worldLV = new G4LogicalVolume(worldS,           // its solid
				     defaultMaterial,  // its material
				     "World");         // its name
  
  auto worldPV = new G4PVPlacement(0,                // no rotation
				   G4ThreeVector(),  // at (0,0,0)
				   worldLV,          // its logical volume                         
				   "World",          // its name
				   0,                // its mother  volume
				   false,            // no boolean operation
				   0,                // copy number
				   fCheckOverlaps);  // checking overlaps 


  G4double TotTh = 4.*culayThickness2+3.*culayThickness1+
    FR4UpThickness+FR4PrepregThickness+FR4DownThickness+
    gasgapUpThickness+gasgapDownThickness+
    2.*KaptonThickness;
  
  //                               
  // Chamber
  //  
  //  auto chamberS = new G4Box("Chamber", chaSizeX/2, chaSizeY/2, chaThickness/2);
  auto chamberS = new G4Box("Chamber", chaSizeX/2, chaSizeY/2, TotTh/2);
  
  auto chamberLV = new G4LogicalVolume(chamberS,     // its solid
				       defaultMaterial,  // its material
				       "Chamber");   // its name



  // Definizione dei vari strati (volumi logici) di cui è costituita la camera
  // Si va in ordine di materiale (Rame, FR4, gas, Kapton)
  
  //                               
  // strato di rame da 35 micron
  //
  auto culay2S = new G4Box("CuLay2", chaSizeX/2, chaSizeY/2, culayThickness2/2); // its size
  
  auto culay2LV = new G4LogicalVolume(culay2S,             // its solid
				      culayMaterial,       // its material
				      "culay2LV");         // its name
  //                               
  // strato di rame da 5 micron
  //
  auto culay1S = new G4Box("CuLay1", chaSizeX/2, chaSizeY/2, culayThickness1/2); // its size
  
  auto culay1LV = new G4LogicalVolume(culay1S,             // its solid
				      culayMaterial,      // its material
				      "culay1LV");         // its name

  
  //                               
  // strato di vetronite da 3 mm (FR4Up)
  //
  auto FR4UpS = new G4Box("FR4Up", chaSizeX/2, chaSizeY/2, FR4UpThickness/2); // its size
  
  auto FR4UpLV = new G4LogicalVolume(FR4UpS,        // its solid
				     vetronMaterial, // its material
				     "FR4UpLV");        // its name
  
  //                               
  // strato di vetronite da 50 micron (FR4Prepreg)
  //
  auto FR4PrepregS = new G4Box("FR4Prepreg", chaSizeX/2, chaSizeY/2, FR4PrepregThickness/2); // its size
  
  auto FR4PrepregLV = new G4LogicalVolume(FR4PrepregS,        // its solid
					  vetronMaterial, // its material
					  "FR4PrepregLV");        // its name
  
  //                               
  // strato di vetronite da 3.2 mm (FR4Down)
  //
  auto FR4DownS = new G4Box("FR4Down", chaSizeX/2, chaSizeY/2, FR4DownThickness/2); // its size
  
  auto FR4DownLV = new G4LogicalVolume(FR4DownS,        // its solid
				       vetronMaterial, // its material
				       "FR4DownLV");        // its name

  
  // 
  // gas gap (above GEM foil - 3mm)
  //
  auto gasgapUpS  = new G4Box("gasgapUp", chaSizeX/2, chaSizeY/2, gasgapUpThickness/2);
  
  auto gasgapUpLV = new G4LogicalVolume(gasgapUpS,           // its solid
					gasMaterial,  // its material
					"gasgapUp");         // its name

  // 
  // gas gap (belowe GEM foil - 1mm)
  //
  auto gasgapDownS  = new G4Box("gasgapDown", chaSizeX/2, chaSizeY/2, gasgapDownThickness/2);
  
  auto gasgapDownLV = new G4LogicalVolume(gasgapDownS,           // its solid
					  gasMaterial,  // its material
					  "gasgapDown");         // its name

  // 
  // strato di Kapton (GEM e RWELL)
  //
  auto KaptonS  = new G4Box("Kapton", chaSizeX/2, chaSizeY/2, KaptonThickness/2);
  
  auto KaptonLV = new G4LogicalVolume(KaptonS,           // its solid
				      kaptonMaterial,  // its material
				      "KaptonLV");         // its name






  
  // Posizionamento dei vari volumi nella camera
  
  // Cu2 + FR4Up + Cu2 (vetronite superiore)
  // gasUp 
  // Cu1 + Kapton + Cu1 (foglio di GEM)
  // gasDown 
  // Cu1 + Kapton (RWELL)
  // Prepreg + Cu2 + FR4Down + Cu2 (vetronite inferiore)

  
  
  printf("TotTh: %f\n",TotTh);

  //                               
  // strato di rame
  //
  zpos = -1*TotTh/2+culayThickness2/2;
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0,0,zpos),  // at (0,0,0)
		    culay2LV,        // its logical volume                         
		    "culay2LV",       // its name
		    chamberLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps 

  //                               
  // FR4 da 3mm
  //
  zpos = -1*TotTh/2+culayThickness2+FR4UpThickness/2;
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., 0., zpos), // its position
		    FR4UpLV,         // its logical volume                         
		    "FR4Up",      // its name
		    chamberLV,          // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  
  //                               
  // strato di rame
  //
  zpos = -1*TotTh/2+culayThickness2+FR4UpThickness+culayThickness2/2;
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0,0,zpos),  // at (0,0,0)
		    culay2LV,        // its logical volume                         
		    "culay2LV",       // its name
		    chamberLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps 

  //                               
  // gap di gas superiore
  //
  zpos = -1*TotTh/2+culayThickness2+FR4UpThickness+culayThickness2+gasgapUpThickness/2;
  printf("zpos: %f\n",zpos);
  new G4PVPlacement(0,
		    G4ThreeVector(0., 0., zpos),
		    gasgapUpLV,
		    "gasgapUp",
		    chamberLV,
		    false,
		    0,
		    fCheckOverlaps);

  //                               
  // strato di rame superiore della GEM
  //
  zpos = -1*TotTh/2+culayThickness2+FR4UpThickness+culayThickness2+gasgapUpThickness
    +culayThickness1/2;
  printf("zpos: %f\n",zpos);
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., 0., zpos),  // at (0,0,0)
		    culay1LV,         // its logical volume                         
		    "culay1",         // its name
		    chamberLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  
  //                               
  // strato di kapton della GEM
  //
  zpos = -1*TotTh/2+culayThickness2+FR4UpThickness+culayThickness2+gasgapUpThickness
    +culayThickness1+KaptonThickness/2;
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., 0., zpos),  // at (0,0,0)
		    KaptonLV,         // its logical volume                         
		    "Kapton",         // its name
		    chamberLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps 
  
  //                               
  // strato di rame inferiore della GEM
  //
  zpos = -1*TotTh/2+culayThickness2+FR4UpThickness+culayThickness2+gasgapUpThickness
    +culayThickness1+KaptonThickness+culayThickness1/2;
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., 0., zpos),  // at (0,0,0)
		    culay1LV,         // its logical volume                         
		    "culay1",         // its name
		    chamberLV,        // its mother  volume
		    false,            // no boolean operation
		    1,                // copy number
		    fCheckOverlaps);  // checking overlaps
  
  //                               
  // gap di gas inferiore
  //
  zpos = -1*TotTh/2+culayThickness2+FR4UpThickness+culayThickness2+gasgapUpThickness
    +culayThickness1+KaptonThickness+culayThickness1+gasgapDownThickness/2;
  new G4PVPlacement(0,
		    G4ThreeVector(0., 0., zpos),
		    gasgapDownLV,
		    "gasgapDown",
		    chamberLV,
		    false,
		    0,
		    fCheckOverlaps);
  
  //                               
  // sottile strato di rame dello strato RWELL
  //
  zpos = -1*TotTh/2+culayThickness2+FR4UpThickness+culayThickness2+gasgapUpThickness
    +culayThickness1+KaptonThickness+culayThickness1+gasgapDownThickness+culayThickness1/2;
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., 0., zpos),  // at (0,0,0)
		    culay1LV,         // its logical volume                         
		    "culay1",         // its name
		    chamberLV,        // its mother  volume
		    false,            // no boolean operation
		    2,                // copy number
		    fCheckOverlaps);  // checking overlaps
  
  //                               
  // strato di kapton dello strato RWELL
  //
  zpos = -1*TotTh/2+culayThickness2+FR4UpThickness+culayThickness2+gasgapUpThickness
    +culayThickness1+KaptonThickness+culayThickness1+gasgapDownThickness+culayThickness1
    +KaptonThickness/2;
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., 0., zpos),  // at (0,0,0)
		    KaptonLV,         // its logical volume                         
		    "Kapton",         // its name
		    chamberLV,        // its mother  volume
		    false,            // no boolean operation
		    1,                // copy number
		    fCheckOverlaps);  // checking overlaps 
  
  //                               
  // strato sottile di Prepreg
  //
  zpos = -1*TotTh/2+culayThickness2+FR4UpThickness+culayThickness2+gasgapUpThickness
    +culayThickness1+KaptonThickness+culayThickness1+gasgapDownThickness+culayThickness1
    +KaptonThickness+FR4PrepregThickness/2;
  new G4PVPlacement(0,
		    G4ThreeVector(0., 0., zpos),
		    FR4PrepregLV,
		    "FR4Prepreg",
		    chamberLV,
		    false,
		    0,
		    fCheckOverlaps);
  
  //                               
  // strato di rame superiore del piano di FR4 basso
  //
  zpos = -1*TotTh/2+culayThickness2+FR4UpThickness+culayThickness2+gasgapUpThickness
    +culayThickness1+KaptonThickness+culayThickness1+gasgapDownThickness+culayThickness1
    +KaptonThickness+FR4PrepregThickness+culayThickness2/2;
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., 0., zpos),
		    culay2LV,         // its logical volume                         
		    "culay2",         // its name
		    chamberLV,        // its mother  volume
		    false,            // no boolean operation
		    2,                // copy number
		    fCheckOverlaps);  // checking overlaps 
  
  //                               
  // strato di FR4 basso
  //
  zpos = -1*TotTh/2+culayThickness2+FR4UpThickness+culayThickness2+gasgapUpThickness
    +culayThickness1+KaptonThickness+culayThickness1+gasgapDownThickness+culayThickness1
    +KaptonThickness+FR4PrepregThickness+culayThickness2+FR4DownThickness/2;
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., 0., zpos), // its position
		    FR4DownLV,        // its logical volume                         
		    "FR4Down",        // its name
		    chamberLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  
  //                               
  // strato di FR4 basso
  //
  zpos = -1*TotTh/2+culayThickness2+FR4UpThickness+culayThickness2+gasgapUpThickness
    +culayThickness1+KaptonThickness+culayThickness1+gasgapDownThickness+culayThickness1
    +KaptonThickness+FR4PrepregThickness+culayThickness2+FR4DownThickness+culayThickness2/2;
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., 0., zpos), // its position
		    culay2LV,         // its logical volume                         
		    "culay2",         // its name
		    chamberLV,        // its mother  volume
		    false,            // no boolean operation
		    3,                // copy number
		    fCheckOverlaps);  // checking overlaps 

  // posiziono la chamber
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0, 0, -1.5*TotTh),  // at (0,0,0)
		    chamberLV,        // its logical volume                         
		    "Chamber",        // its name
		    worldLV,          // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps 
      
    new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0, 0, -0.5*TotTh),  // at (0,0,0)
		    chamberLV,        // its logical volume                         
		    "Chamber",        // its name
		    worldLV,          // its mother  volume
		    false,            // no boolean operation
		    1,                // copy number
		    fCheckOverlaps);  // checking overlaps

    new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0, 0, 0.5*TotTh),  // at (0,0,0)
		    chamberLV,        // its logical volume                         
		    "Chamber",        // its name
		    worldLV,          // its mother  volume
		    false,            // no boolean operation
		    2,                // copy number
		    fCheckOverlaps);  // checking overlaps

    new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0, 0, 1.5*TotTh),  // at (0,0,0)
		    chamberLV,        // its logical volume                         
		    "Chamber",        // its name
		    worldLV,          // its mother  volume
		    false,            // no boolean operation
		    3,                // copy number
		    fCheckOverlaps);  // checking overlaps




  // Field che racchiude il gas
  auto fieldUpDownS = new G4Box("fieldUpDown" ,chaSizeX/2, fieldThickness/2, (gasgapUpThickness+gasgapDownThickness+2*culayThickness1+KaptonThickness)/2);
  auto fieldUpDownLV = new G4LogicalVolume(fieldUpDownS, vetronMaterial, "FieldUpDownLV");
  auto fieldRightLeftS = new G4Box("fieldRightLeft" ,fieldThickness/2, fieldThickness+chaSizeY/2, (gasgapUpThickness+gasgapDownThickness+2*culayThickness1+KaptonThickness)/2);
  auto fieldRightLeftLV = new G4LogicalVolume(fieldRightLeftS, vetronMaterial, "FieldRightLeftLV");

  //    chamber 0
  zpos = -2*TotTh+culayThickness2+FR4UpThickness+culayThickness2
    +(gasgapUpThickness+gasgapDownThickness+2*culayThickness1+KaptonThickness)/2;
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., (chaSizeY+fieldThickness)/2, zpos), // its position
		    fieldUpDownLV,         // its logical volume                         
		    "FieldUp",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., -(chaSizeY+fieldThickness)/2, zpos), // its position
		    fieldUpDownLV,         // its logical volume                         
		    "FieldDown",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector((chaSizeX+fieldThickness)/2, 0., zpos), // its position
		    fieldRightLeftLV,         // its logical volume                         
		    "FieldRight",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(-(chaSizeX+fieldThickness)/2, 0., zpos), // its position
		    fieldRightLeftLV,         // its logical volume                         
		    "FieldLeft",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps

  //    chamber 1
  zpos = -1*TotTh+culayThickness2+FR4UpThickness+culayThickness2
    +(gasgapUpThickness+gasgapDownThickness+2*culayThickness1+KaptonThickness)/2;
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., (chaSizeY+fieldThickness)/2, zpos), // its position
		    fieldUpDownLV,         // its logical volume                         
		    "FieldUp",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., -(chaSizeY+fieldThickness)/2, zpos), // its position
		    fieldUpDownLV,         // its logical volume                         
		    "FieldDown",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector((chaSizeX+fieldThickness)/2, 0., zpos), // its position
		    fieldRightLeftLV,         // its logical volume                         
		    "FieldRight",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(-(chaSizeX+fieldThickness)/2, 0., zpos), // its position
		    fieldRightLeftLV,         // its logical volume                         
		    "FieldLeft",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps

  //    chamber 2
  zpos = culayThickness2+FR4UpThickness+culayThickness2
    +(gasgapUpThickness+gasgapDownThickness+2*culayThickness1+KaptonThickness)/2;
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., (chaSizeY+fieldThickness)/2, zpos), // its position
		    fieldUpDownLV,         // its logical volume                         
		    "FieldUp",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., -(chaSizeY+fieldThickness)/2, zpos), // its position
		    fieldUpDownLV,         // its logical volume                         
		    "FieldDown",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector((chaSizeX+fieldThickness)/2, 0., zpos), // its position
		    fieldRightLeftLV,         // its logical volume                         
		    "FieldRight",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(-(chaSizeX+fieldThickness)/2, 0., zpos), // its position
		    fieldRightLeftLV,         // its logical volume                         
		    "FieldLeft",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps

  //    chamber 3
  zpos = TotTh+culayThickness2+FR4UpThickness+culayThickness2
    +(gasgapUpThickness+gasgapDownThickness+2*culayThickness1+KaptonThickness)/2;
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., (chaSizeY+fieldThickness)/2, zpos), // its position
		    fieldUpDownLV,         // its logical volume                         
		    "FieldUp",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(0., -(chaSizeY+fieldThickness)/2, zpos), // its position
		    fieldUpDownLV,         // its logical volume                         
		    "FieldDown",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector((chaSizeX+fieldThickness)/2, 0., zpos), // its position
		    fieldRightLeftLV,         // its logical volume                         
		    "FieldRight",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps
  new G4PVPlacement(0,                // no rotation
		    G4ThreeVector(-(chaSizeX+fieldThickness)/2, 0., zpos), // its position
		    fieldRightLeftLV,         // its logical volume                         
		    "FieldLeft",         // its name
		    worldLV,        // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    fCheckOverlaps);  // checking overlaps        


  //                                        
  // Visualization attributes
  //
  worldLV->SetVisAttributes (G4VisAttributes::GetInvisible());

  auto simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  chamberLV->SetVisAttributes(simpleBoxVisAtt);

  //
  // Always return the physical World
  //
  return worldPV;
}



  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void DetectorConstruction::ConstructSDandField()
{
  // G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // 
  // Sensitive detectors
  //
  //auto gapSD = new CalorimeterSD("GapSD", "GapHitsCollection", fNofLayers);
  //G4SDManager::GetSDMpointer()->AddNewDetector(gapSD);
  //SetSensitiveDetector("gasgapUp",gapSD);
 
  //auto absoSD = new CalorimeterSD("AbsorberSD", "AbsorberHitsCollection", fNofLayers);
  //G4SDManager::GetSDMpointer()->AddNewDetector(absoSD);
  //SetSensitiveDetector("gasgapDown", absoSD); 
  // 
  // Magnetic field
  //
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}


// void DetectorConstruction::ConstructSDandField()
// {
//   // G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

//   //
//   // Sensitive detectors
//   //
//   auto absoSD = new CalorimeterSD("AbsorberSD", "AbsorberHitsCollection", fNofLayers);
//   G4SDManager::GetSDMpointer()->AddNewDetector(absoSD);
//   SetSensitiveDetector("AbsoLV", absoSD);

//   auto gapSD = new CalorimeterSD("GapSD", "GapHitsCollection", fNofLayers);
//   G4SDManager::GetSDMpointer()->AddNewDetector(gapSD);
//   SetSensitiveDetector("GapLV", gapSD);

//   //
//   // Magnetic field
//   //
//   // Create global magnetic field messenger.
//   // Uniform magnetic field is then created automatically if
//   // the field value is not zero.
//   G4ThreeVector fieldValue;
//   fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
//   fMagFieldMessenger->SetVerboseLevel(1);

//   // Register the field messenger for deleting
//   G4AutoDelete::Register(fMagFieldMessenger);
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace MRWELL

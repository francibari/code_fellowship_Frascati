# code_fellowship_Frascati
This repository contains the code that I developed during my fellowship at the LNF and all the files and instructions needed to run it and analyse the results produced. Two different subdirectories are present, a root file and a `.mac` file.

## finalProject
This directory contains the code I used for the simulations. It contains three different subdirectories, one for each different design of the detector. In each subdirectories there are the files required by Geant4, in particular the most important ones are: `PrimaryGenerationAction`, `DetectorConstruction`, `SteppingAction` and `RunAction`. Rember that initial neutrons of the beam are generated with energy distribution from the ROOT files, direction parallel to the positive z-axis (perpendicular to the cross sectional area of the detector), with a fixed z position outside the detector and uniformly distributed along x and y coordinates to cover all the cross sectional area of the detector.

## analysisMacros
This directory contains the macros that I used to analyse the data. Two different analysis macro classes are present: positionMacro e energyMacro, to produce plots of position and energy respectively. Each macro is present in three different versions, one for each design of the detector, for a total number of six files in this directory. 

## ROOT file
This file contains the initial distribution of neutrons that I used in the beam. This ROOT file contains three different TH1D histograms. `h1d` is the differential fluence, the `h1d_yield` is the fluence (differential one times the energy bin width) I used in the simulations, while `h1d_yield_random` is the histogram of a 1 million of extractions from the previous distribution (just to check). This file must be copied in the `finalProject-build` directory before running the simulation.

## macro.mac file
It is the macro to run with the executable in order to not use the graphical part of Geant4 and speed up the simulation time. This macro contains the command to generate 1 million of events. This file must be copied in each subdirectories in which the executable will be present.

## Download and run the simulations
The first thing to do is to download the repository using the command:

```sh
git clone https://github.com/francibari/code_fellowship_Frascati.git
```

Then, you must build the project using CMake. In particular, create a new folder at the same level of finalProject:

```sh
mkdir finalProject-build
cd finalProject-build
cmake ../finalProject
make -j4
```

Now, you will find inside this new folder `finalProject-build`, three different subdirectories with the same name of the detector design, each one containing an executable: Chamber, MRWell and GRWell. That executables are the files to run to start the simulation. For example, in the Chamber case, to run the simulation, the user will run the command:
```sh
./Chamber -m macro.mac
```

Same command but with the proper name for the other designs. Remind to copy the initial root file with neutron distribution in the `finalProject-build` directory before running the simulation.


The simulations will produce a ROOT file named `B4.root` containing a ntuple with different information per step. Each row of this ntuple is a step while each column is a different information on that step. In particular, we have:
- Edep (Double_t): the energy deposited in the step by the particle.
- PDG (Int_t): PDG code of the particle.
- TrackID (Int_t): ID of the track to which that step belongs.
- EventID (Int_t): ID of the event to which the step belongs.
- LVName (Char_t): name of the logical volume in which the step is.
- BeamEnergy (Double_t): kinetic energy of the initial neutron of the beam.
- MotherTrackID (Int_t): ID of the track of the mother particle from which the particle in the step is generated. If there is no mother particle, it is 0.
- Xpos (Double_t): x-coord of the PreStepPoint.
- Ypos (Double_t): y-coord of the PreStepPoint.
- Zpos (Double_t): z-coord of the PreStepPoint.
- Xmom (Double_t): x-coord of the momentum of the particle in the PreStepPoint.
- Ymom (Double_t): y-coord of the momentum of the particle in the PreStepPoint.
- Zmom (Double_t): z-coord of the momentum of the particle in the PreStepPoint.
- Xpost (Double_t): x-coord of the PostStepPoint.
- Ypost (Double_t): y-coord of the PostStepPoint.
- Zpost (Double_t): z-coord of the PostStepPoint.
- ProcessName (Char_t): name of the process responsible of the energy deposition of that step of the particle.
- PreProcessName (Char_t): name of the process that generate the particle. Default value is "none".

To analyse that file, you must run the macros in the analysisMacro directory, using ROOT. You must copy each analysis macro in the proper directory containing the `B4.root` file. For each version of the detector design, two different files are present: `positionMacro.C` and `energyMacro.C`, respectively to plot position and energy of the particles.
In particular, `positionMacro.C` contains different macros. The one I used in the presentation of 30th September is:
- `plotVertexPosition` is a macro that create a root file with two canvas. The first canvas contains four plots. On the top left there is the plot of energy deposited by a track in the gas. On the top right there is the interaction vertex that created that track: in other words, it is the position of the first step of the track that has an energy deposition in the gas (active volume). There is color code to distinguish the various particles: electrons/positrons (blue), heavy ions (green), protons (yellow) and gammas (red). On the bottom left, there is the plot of the vertices of the track of electrons that release energy in the active volume. Also here, it is present a color code to distinguish the different processes that create the electron: ionization (red), photoelectric effect (blue), compton scattering (green), radioactive beta decay (yellow). Finally, on the bottom right, there is the plot of the z-coordinates of vertices of primary neutrons that undergo interactions different from `Transport` and `noProcess`. To run this macro, the user has to give the path of the `B4.root` file as argument of the macro.
The macros used in the `energyMacro.C` file are:
- `plotGammaMomentum` is a macro to plot the momentum of the gammas produced in the detector. It doesn't matter the position where the gamma is generated, if the gamma interacts with the detector or the process that produced that gamma. Two different plots are produced: one considering all gammas, the other one is a zoom of gammas under 0.1 MeV. Both histograms are stored in the `out.root` file where also a `TCanvas` containing both is present. To run this macro, the user has to give the name of the path of the `B4.root` file as argument. 
- `plotPDGstep` is a macro to plot the energy deposited in the active volume (gas) by the particle in each step. This macro produces a root file named `energy_result.root`. This files contains a `TCanvas` with the energy deposition distribution per step in the active volume. There is color code to distinguish the variuos particles: electrons/positrons (blue), gammas (red), heavy ions (green) and protons (yellow). In the canvas the same plot is present in two different versions: on the top the superimposition of the four chategories pf particles while on the bottom, the stacked histogram for the four chategories. In the same root file, it is present also a TH2D histogram that plots the energy deposition per step and the initial energy of the beam, in order to verify if there are some dependencies between the energy deposition and the initial energy of the beam. Also in this case, to run this macro, the user has to gise as argument the path of the `B4.root` file.
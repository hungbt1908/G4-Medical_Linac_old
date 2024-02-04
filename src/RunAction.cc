#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "Run.hh"
#include "G4LogicalVolume.hh"

#include "G4RunManager.hh"
#include "G4Threading.hh"
#include "G4THitsMap.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"  

#include <fstream>
#include "G4ios.hh"
#include <ctime>
#include <iostream>

using namespace std;

double RunAction::diffclock(clock_t clock1, clock_t clock2)
{
    double diffticks = clock1 - clock2;
    double diffms = (diffticks*1000)/CLOCKS_PER_SEC;
    return diffms;
}

RunAction::RunAction(DetectorConstruction *detConstruction)
  : G4UserRunAction(), fNx(0), fNy(0), fNz(0), fDetConstruction(detConstruction)
{
    // Prepare data member for RE02Run.
    // vector represents a list of MultiFunctionalDetector names.
    fSDName.push_back(G4String("PhantomSD"));
}

RunAction::~RunAction()
{
    fSDName.clear();
}

G4Run* RunAction::GenerateRun()
{
    // Generate new RUN object, which is specially dedicated for MultiFunctionalDetector scheme.
    return new Run(fSDName);
}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
    begin = clock();
}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
    end = clock();
    if(!IsMaster()) return;

    // Dump all socred quantities involved in Run.
    Run* reRun = (Run*)aRun;
    reRun->DumpAllScorer();

    // Get phantom info
    const DetectorConstruction* detector = (const DetectorConstruction*)(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    detector->GetNumberOfSegmentsInPhantom(fNx,fNy,fNz); //Fill fNx,y,z.
    detector->GetNumberOfPhantomDimension(dx, dy, dz);
    VoxelVolume = (dx/fNx)*(dy/fNy)*(dz/fNz);
    voxelMass = fDetConstruction->GetVoxelPhysicalVolume()->GetLogicalVolume()->GetMass();
    voxelDensity = fDetConstruction->GetVoxelPhysicalVolume()->GetLogicalVolume()->GetMaterial()->GetDensity();
    phantomMaterialName = fDetConstruction->GetVoxelPhysicalVolume()->GetLogicalVolume()->GetMaterial()->GetName();
    // Dump accumulated quantities for this RUN.
    G4THitsMap<G4double>* totEdep  = reRun->GetHitsMap("PhantomSD/totalEDep");


    G4cout << "============================================================="
            << G4endl;
    G4cout << " Number of event processed : "<< aRun->GetNumberOfEvent() << G4endl;
    G4cout << "============================================================="
           << G4endl;
    G4cout << std::setw(16) << totEdep->GetName() << G4endl;
    G4int ix = fNx/2;  
    G4int iy = fNy/2;
    G4int iz;

    // Get edep
    for ( iz = 0; iz < fNz; iz++)
    { 
        G4double* totED = (*totEdep)[CopyNo(ix,iy,iz)];
        if ( !totED ) totED = new G4double(0.0);

        G4cout << std::setw( 6) << iz << "  "
               << std::setw(12) << G4BestUnit(*totED,"Energy")
               << G4endl;
    }
    G4cout << "============================================="<<G4endl;

    // Get created time
    time_t theTime= time(NULL);
    struct std::tm* aTime = localtime(&theTime);

    // Get number of threads
    int _NumberOfRunningWorkerThreads           = G4Threading::GetNumberOfRunningWorkerThreads();
    
    // Get number of events
    G4RunManager* runManager                    = G4RunManager::GetRunManager();
    int Total_Events_To_Be_Processed            = runManager->GetNumberOfEventsToBeProcessed();
   
    std::ofstream  file("totED.txt");

    file << "# Date of file creation:     \t" << asctime(aTime); 
    file << "# Number of Running threads: \t" << _NumberOfRunningWorkerThreads << "\n"; 
    file << "# Number of Events:          \t" << Total_Events_To_Be_Processed << "\n";
    file << "# Phantom dimension along X: \t" << dx/cm << " cm" << "\n";
    file << "# Phantom dimension along Y: \t" << dy/cm << " cm" << "\n";
    file << "# Phantom dimension along Z: \t" << dz/cm << " cm" << "\n";
    file << "# Number of Voxels along X:  \t" << fNx << "\n";
    file << "# Number of Voxels along Y:  \t" << fNy << "\n"; 
    file << "# Number of Voxels along Z:  \t" << fNz << "\n";
    file << "# Voxel size along X:        \t" << (dx/fNx)/cm << " cm" << "\n";
    file << "# Voxel size along Y:        \t" << (dy/fNy)/cm << " cm" << "\n";
    file << "# Voxel size along Z:        \t" << (dz/fNz)/cm << " cm" << "\n";
    file << "# Voxel volume:              \t" << VoxelVolume/cm3 << " cm3" << "\n";
    file << "# Voxel mass:                \t" << voxelMass/g << " g" << "\n";
    file << "# Phantom material:          \t" << phantomMaterialName << "\n";
    file << "# Voxel density:             \t" << voxelDensity/(g/cm3) << " g/cm3" << "\n";
    file << "# Elapsed time:              \t" << (double(diffclock(end,begin)/1000)) << " seconds" << G4endl;
    file << "---------------------------------------------------------------------------------------- " << "\n"; 
    file << setw(0)  << "Voxel_X_ID" 
         << setw(15) << "Voxel_Y_ID" 
         << setw(15) << "Voxel_Z_ID" 
         << setw(15) << "eDep(MeV)"
         << setw(15) << "Dose(Gy)" 
         << "\n";

    for ( iz = 0; iz < fNz; iz++)
    {   
        for ( iy = 0; iy < fNy; iy++)
        { 
            for ( ix = 0; ix < fNx; ix++)
            { 
                G4double* totED = (*totEdep)[CopyNo(ix,iy,iz)];
                //G4int*  entries = (*numberOfEvent)[CopyNo(ix,iy,iz)];
                if (!totED) totED = new G4double(0.0);
                G4double totDose = *totED/voxelMass;
                file << setw(5)  << ix 
                     << setw(15) << iy
                     << setw(15) << iz
                     << setw(15) << *totED/MeV 
                     << setw(15) << totDose/gray
                     << G4endl;
            }
        }
    }
    file.close();
}

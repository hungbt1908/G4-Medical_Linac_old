#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <vector>

class DetectorConstruction;
class G4Run;
class G4LogicalVolume;
class DetectorConstruction;

class RunAction : public G4UserRunAction
{
    public:
        RunAction(DetectorConstruction *detConstruction);
        virtual ~RunAction();

    public:
        virtual G4Run* GenerateRun();
        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);

    public:
        // Converting segment number of  water phantom to copyNo of HitsMap.
        G4int CopyNo(G4int ix, G4int iy, G4int iz)
        {  
            return (iy*(fNx*fNz)+ix*fNz+iz);
        }

    public:
        // Time analysis
        double diffclock(clock_t ,clock_t );
        clock_t begin;
        clock_t end;

    private:
        // Data member 
        // - vector of MultiFunctionalDetecor names.
        std::vector<G4String> fSDName;  

        // for conversion of sengment number to copyNo.
        G4int fNx, fNy, fNz;
        G4double dx, dy, dz;

        G4double voxelMass, voxelDensity, VoxelVolume;
        G4String phantomMaterialName;
        G4LogicalVolume* logicVolume;
        DetectorConstruction *fDetConstruction = nullptr;
  
};

#endif
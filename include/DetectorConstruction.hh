#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class G4VisAttributes;
class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;
class PhantomSD;
class PhantomROGeometry;
class DetectorMessenger;
class G4Box;
class G4Cons;

using namespace std;

class DetectorConstruction : public G4VUserDetectorConstruction 
{
public:
    // methods
    DetectorConstruction();
    ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    void ConstructSDandField();
    void DefineMaterial();
    void DefineAttribute();
    G4VPhysicalVolume* ConstructVolume();
    void ConstructTarget1();
    void ConstructPrimaryCollimator1();
    void ConstructTarget2();
    void ConstructPrimaryCollimator2();
    void ConstructVacuumWindow2();
    void ConstrucIonizationChamber2();
    void ConstructMirror2();
    void ConstructJaw1X();
    void ConstructJaw2X();
    void ConstructJaw1Y();
    void ConstructJaw2Y();
    void ConstructAcc1();
    void ConstructAcc2();    
    void ConstructWaterPhantom();
    void PrintInformation();
 
public:
    // attributes

    G4bool checkOverlap = false;

    // materials 
    G4Material* Air;
    G4Material* Copper;
    G4Material* Tungsten;
    G4Material* Vacuum;
    G4Material* Lead;
    G4Material* Berylium;
    G4Material* Kapton;
    G4Material* Mylar;
    G4Material* Water;

    // visualization
    G4VisAttributes* worldAtt;
    G4VisAttributes* copperAtt;
    G4VisAttributes* tungstenAtt;
    G4VisAttributes* Tungsten_Color;

    // volumes
    G4Box* solidWorld;
    G4LogicalVolume* logicWorld;
    G4VPhysicalVolume* physWorld;

    G4double isoCentre;
    G4double jaw1XAperture;
    G4double jaw2XAperture;
    G4double jaw1YAperture;
    G4double jaw2YAperture;

    DetectorMessenger* pDetectorMessenger;
};

#endif

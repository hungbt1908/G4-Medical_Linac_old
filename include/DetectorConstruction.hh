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
    void ConstructTarget();
    void PrintInformation();
 
public:
    // attributes

    G4bool checkOverlap = false;

    // materials 
    G4Material* Air;
    G4Material* Copper;
    G4Material* Tungsten;

    // visualization
    G4VisAttributes* worldAtt;
    G4VisAttributes* copperAtt;
    G4VisAttributes* tungstenAtt;
    G4VisAttributes* Tungsten_Color;

    // volumes
    G4Box* solidWorld;
    G4LogicalVolume* logicWorld;
    G4VPhysicalVolume* physWorld;

    DetectorMessenger* pDetectorMessenger;
};

#endif

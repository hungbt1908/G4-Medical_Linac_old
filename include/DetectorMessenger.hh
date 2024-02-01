#ifndef DetecorMessenger_h
#define DetecorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

class DetectorMessenger: public G4UImessenger
{
public:
    DetectorMessenger(DetectorConstruction* );
    ~DetectorMessenger();

    void SetNewValue(G4UIcommand*, G4String);
    
private:
    DetectorConstruction* fDetectorConstruction;

    G4UIdirectory* linacDir;
    G4UIcmdWithADoubleAndUnit* setJaw1XApertureCmd;
    G4UIcmdWithADoubleAndUnit* SetJaw2XApertureCmd;
    G4UIcmdWithADoubleAndUnit* SetJaw1YApertureCmd;
    G4UIcmdWithADoubleAndUnit* SetJaw2YApertureCmd;
    G4UIcmdWithADoubleAndUnit* SetIsocenterCmd;
    G4UIcmdWithADoubleAndUnit* setPhantomXDimCmd;
    G4UIcmdWithADoubleAndUnit* setPhantomYDimCmd;
    G4UIcmdWithADoubleAndUnit* setPhantomZDimCmd;
    G4UIcmdWithAnInteger* setNumberOfVoxelsAlongXCmd;
    G4UIcmdWithAnInteger* setNumberOfVoxelsAlongYCmd;
    G4UIcmdWithAnInteger* setNumberOfVoxelsAlongZCmd;
};



#endif

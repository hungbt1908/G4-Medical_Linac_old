#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* pDetectorConstruction)
    : G4UImessenger(),fDetectorConstruction(pDetectorConstruction)
{
    //
    linacDir = new G4UIdirectory("/LINAC/Geometry/");
    linacDir -> SetGuidance("Command to change LGK setups");

    //
    setPhantomXDimCmd = new G4UIcmdWithADoubleAndUnit("/LINAC/Geometry/SetPhantomXDim", this);
    setPhantomXDimCmd->SetGuidance("Set phantom dimension along X axis.");
    setPhantomXDimCmd->SetParameterName("phantomXDim", false);
    setPhantomXDimCmd->SetUnitCategory("Length");
	setPhantomXDimCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //
    setPhantomYDimCmd = new G4UIcmdWithADoubleAndUnit("/LINAC/Geometry/SetPhantomYDim", this);
    setPhantomYDimCmd->SetGuidance("Set phantom dimension along Y axis.");
    setPhantomYDimCmd->SetParameterName("phantomYDim", false);
    setPhantomYDimCmd->SetUnitCategory("Length");
	setPhantomYDimCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //
    setPhantomZDimCmd = new G4UIcmdWithADoubleAndUnit("/LINAC/Geometry/SetPhantomZDim", this);
    setPhantomZDimCmd->SetGuidance("Set phantom dimension along Z axis.");
    setPhantomZDimCmd->SetParameterName("phantomZDim", false);
    setPhantomZDimCmd->SetUnitCategory("Length");
	setPhantomZDimCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //
    setNumberOfVoxelsAlongXCmd = new G4UIcmdWithAnInteger("/LINAC/Geometry/SetNumberOfVoxelsAlongX", this);
    setNumberOfVoxelsAlongXCmd->SetGuidance("Set number of voxels along X axis.");
    setNumberOfVoxelsAlongXCmd->SetParameterName("nOfVoxelsX", false);
    setNumberOfVoxelsAlongXCmd->AvailableForStates(G4State_Idle, G4State_PreInit);

    //
    setNumberOfVoxelsAlongYCmd = new G4UIcmdWithAnInteger("/LINAC/Geometry/SetNumberOfVoxelsAlongY", this);
    setNumberOfVoxelsAlongYCmd->SetGuidance("Set number of voxels along Y axis.");
    setNumberOfVoxelsAlongYCmd->SetParameterName("nOfVoxelsY", false);
    setNumberOfVoxelsAlongYCmd->AvailableForStates(G4State_Idle, G4State_PreInit);

    //
    setNumberOfVoxelsAlongZCmd = new G4UIcmdWithAnInteger("/LINAC/Geometry/SetNumberOfVoxelsAlongZ", this);
    setNumberOfVoxelsAlongZCmd->SetGuidance("Set number of voxels along Z axis.");
    setNumberOfVoxelsAlongZCmd->SetParameterName("nOfVoxelsZ", false);
    setNumberOfVoxelsAlongZCmd->AvailableForStates(G4State_Idle, G4State_PreInit);
}

DetectorMessenger::~DetectorMessenger()
{
    delete linacDir;
    delete setPhantomXDimCmd;
    delete setPhantomYDimCmd;
    delete setPhantomZDimCmd;
    delete setNumberOfVoxelsAlongXCmd;
    delete setNumberOfVoxelsAlongYCmd;
    delete setNumberOfVoxelsAlongZCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{       
    if(command == setPhantomXDimCmd)
    {
       fDetectorConstruction->SetPhantomXDim(setPhantomXDimCmd->GetNewDoubleValue(newValue));
    }

    if(command == setPhantomYDimCmd)
    {
       fDetectorConstruction->SetPhantomYDim(setPhantomYDimCmd->GetNewDoubleValue(newValue));
    }

    if(command == setPhantomZDimCmd)
    {
       fDetectorConstruction->SetPhantomZDim(setPhantomZDimCmd->GetNewDoubleValue(newValue));
    }

    if(command == setNumberOfVoxelsAlongXCmd)
    {
        fDetectorConstruction->SetNumberOfVoxelsAlongX(setNumberOfVoxelsAlongXCmd->GetNewIntValue(newValue));
    }

    if(command == setNumberOfVoxelsAlongYCmd)
    {
        fDetectorConstruction->SetNumberOfVoxelsAlongY(setNumberOfVoxelsAlongYCmd->GetNewIntValue(newValue));
    }

    if(command == setNumberOfVoxelsAlongZCmd)
    {
        fDetectorConstruction->SetNumberOfVoxelsAlongZ(setNumberOfVoxelsAlongZCmd->GetNewIntValue(newValue));
    }
}
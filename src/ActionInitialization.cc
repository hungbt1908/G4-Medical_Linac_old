#include "ActionInitialization.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "DetectorConstruction.hh"

ActionInitialization::ActionInitialization(DetectorConstruction *detConstruction)
	: fDetConstruction(detConstruction)
{}

ActionInitialization::~ActionInitialization()
{}

// void ActionInitialization::BuildForMaster() const
// {
// 	G4UserRunAction* run_action = new RunAction(fDetConstruction);
//   	SetUserAction(run_action);

// }

void ActionInitialization::Build() const
{
	PrimaryGeneratorAction *generator = new PrimaryGeneratorAction();
	SetUserAction(generator);

	SetUserAction(new RunAction(fDetConstruction));

	SetUserAction(new EventAction);
}  


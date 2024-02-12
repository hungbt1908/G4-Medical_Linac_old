#include "ActionInitialization.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "DetectorConstruction.hh"

ActionInitialization::ActionInitialization(DetectorConstruction *detConstruction)
	: fDetConstruction(detConstruction)
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{}

void ActionInitialization::Build() const
{
	PrimaryGeneratorAction *generator = new PrimaryGeneratorAction();
	SetUserAction(generator);

	SetUserAction(new RunAction(fDetConstruction));

	SetUserAction(new EventAction);
}  


#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"

ActionInitialization::ActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{}

void ActionInitialization::Build() const
{
	PrimaryGeneratorAction* generator = new PrimaryGeneratorAction();
	SetUserAction(generator);
}  


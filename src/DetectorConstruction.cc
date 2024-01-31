#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4MultiUnion.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction()
{
  	pDetectorMessenger = new DetectorMessenger(this); 
}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// clear old geometry
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();

	DefineMaterial();
	DefineAttribute();
	return ConstructVolume();
}

void DetectorConstruction::DefineMaterial()
{
	// NIST
	G4NistManager* nist = G4NistManager::Instance();

	// Air
	Air = nist->FindOrBuildMaterial("G4_AIR");

	// Copper
	Copper = nist->FindOrBuildMaterial("G4_Cu");
	copperAtt = new G4VisAttributes(G4Colour(0.5, 0.1, 0.1, 0.1));
	copperAtt->SetVisibility(true);
	copperAtt->SetForceSolid(true);

	// Tungsten
	Tungsten = nist->FindOrBuildMaterial("G4_W");
	tungstenAtt = new G4VisAttributes(G4Colour(1., 0.3, 0.3));
	tungstenAtt->SetVisibility(true);
	tungstenAtt->SetForceWireframe(true);
	tungstenAtt->SetForceAuxEdgeVisible(true);

	// Print materials
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

void DetectorConstruction::DefineAttribute()
{}

G4VPhysicalVolume* DetectorConstruction::ConstructVolume()
{

	// world volume
	solidWorld = new G4Box("solidWorld", 500 * cm, 500 * cm, 500 * cm);
	logicWorld = new G4LogicalVolume(solidWorld, Air, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, checkOverlap);
	logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
	
	// PrintInformation();

	return physWorld;
}

void DetectorConstruction::PrintInformation()
{}

void DetectorConstruction::ConstructSDandField()
{

}
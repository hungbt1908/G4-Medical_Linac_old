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
#include "G4PVReplica.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction()
{
  	pDetectorMessenger = new DetectorMessenger(this); 
	// isoCentre = 100*cm;
	// jaw1XAperture = -10.*cm;
	// jaw2XAperture = 10.*cm;
	// jaw1YAperture = -10.*cm;
	// jaw2YAperture = 10.*cm;
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

	// 
	Vacuum = nist->FindOrBuildMaterial("G4_Galactic");

	//
	Lead = nist->FindOrBuildMaterial("G4_Pb");

	//
	Berylium = nist->FindOrBuildMaterial("G4_Be"); 

	//
	Kapton = nist->FindOrBuildMaterial("G4_KAPTON");

	//
	Mylar = nist->FindOrBuildMaterial("G4_MYLAR");

	//
	Water = nist->FindOrBuildMaterial("G4_WATER");

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
	// ConstructAcc1();
	ConstructAcc2();
	ConstructWaterPhantom();
	return physWorld;
}

void DetectorConstruction::ConstructAcc1()
{
	ConstructTarget1();
	ConstructPrimaryCollimator1();
}

void DetectorConstruction::ConstructAcc2()
{
	ConstructTarget2();
	ConstructPrimaryCollimator2();	
	ConstructVacuumWindow2();
	ConstrucIonizationChamber2();
	ConstructMirror2();
	ConstructJaw1X();
	ConstructJaw2X();
	ConstructJaw1Y();
	ConstructJaw2Y();
}

void DetectorConstruction::ConstructTarget1()
{
	//
	G4double targetADim_x = 0.6*cm;
	G4double targetADim_y = 0.6*cm;
	G4double targetADim_z = 0.04445*cm;
	G4Box* targetA_box = new G4Box("targetA_box",targetADim_x,targetADim_y,targetADim_z);
	G4LogicalVolume *targetA_log = new G4LogicalVolume(targetA_box,Tungsten,"targetA_log",0,0,0);
	G4double targetAPos_x = 0.0*m;
	G4double targetAPos_y = 0.0*m;
	G4double targetAPos_z = 0.20055*cm;
	new G4PVPlacement(0, G4ThreeVector(targetAPos_x,targetAPos_y,targetAPos_z), "targetA",targetA_log,physWorld,false,0);
	targetA_log->SetVisAttributes(tungstenAtt);
	//
	G4double targetBDim_x = 0.6*cm;
	G4double targetBDim_y = 0.6*cm;
	G4double targetBDim_z = 0.07874*cm;
	G4Box* targetB_box = new G4Box("targetB_box",targetBDim_x,targetBDim_y,targetBDim_z);
	G4LogicalVolume *targetB_log = new G4LogicalVolume(targetB_box,Copper,"targetB_log",0,0,0);
	G4double targetBPos_x = 0.0*m;
	G4double targetBPos_y = 0.0*m;
	G4double targetBPos_z = 0.07736*cm;
	new G4PVPlacement(0, G4ThreeVector(targetBPos_x,targetBPos_y,targetBPos_z), "targetB",targetB_log,physWorld,false,0);
}

void DetectorConstruction::ConstructTarget2()
{
	G4Box *targetABox = new G4Box("targetABox", 5.*mm, 5.*mm, (.035*25.4/2.)*mm);
	G4LogicalVolume *targetALV = new G4LogicalVolume(targetABox, Tungsten, "targetALV", 0, 0, 0);
	targetALV->SetVisAttributes(tungstenAtt);

	G4Box *targetBBox = new G4Box("targetABox", 5.*mm, 5.*mm, (0.062*25.4/2.)*mm);
	G4LogicalVolume *targetBLV = new G4LogicalVolume(targetBBox, Copper, "targetBLV", 0, 0, 0);

	// specific translations for the various parts of the component
	new G4PVPlacement(
			0,
			G4ThreeVector(0.,0.,targetABox->GetZHalfLength()),
			"targetAPV",
			targetALV,
			physWorld,
			false,
			0);

	new G4PVPlacement(
			0,
			G4ThreeVector(0.,0.,targetABox->GetZHalfLength()*2.+targetBBox->GetZHalfLength()),
			"targetBPV",
			targetBLV,
			physWorld,
			false,
			0);
}

void DetectorConstruction::ConstructPrimaryCollimator1()
{
	G4RotationMatrix* rotateMatrix = new G4RotationMatrix();
	rotateMatrix->rotateX(180.0*deg);

	//--------------------upper collimator----------------
	G4double innerRadiusOfTheTubeEx = 1.0*cm;
	G4double outerRadiusOfTheTubeEx = 8.*cm;
	G4double hightOfTheTubeEx = 3.0*cm;
	G4double startAngleOfTheTubeEx = 0.*deg;
	G4double spanningAngleOfTheTubeEx = 360.*deg;
	G4Tubs* UpperCollimator = new G4Tubs("UpperCollimator",
			innerRadiusOfTheTubeEx,
			outerRadiusOfTheTubeEx,
			hightOfTheTubeEx,
			startAngleOfTheTubeEx,
			spanningAngleOfTheTubeEx);
	G4LogicalVolume *UpperCollimator_log = new G4LogicalVolume(UpperCollimator, Tungsten, "UpperCollimator_log", 0, 0, 0);

	G4double UpperCollimatorPosX = 0.*cm;
	G4double UpperCollimatorPosY = 0.*cm;
	G4double UpperCollimatorPosZ = -1.*cm;
	new G4PVPlacement(0,
			G4ThreeVector(UpperCollimatorPosX, UpperCollimatorPosY, UpperCollimatorPosZ),
			"UpperCollimator",
			UpperCollimator_log,
			physWorld,
			false,
			0);

	G4double  pRmin1 = 0.*cm;
	G4double  pRmax1 = 0.5*cm;
	G4double  pRmin2 = 0.*cm;
	G4double  pRmax2 = 1.7658592*cm;
	G4double  hightOfTheCone =3.2*cm;
	G4double  startAngleOfTheCone = 0.*deg;
	G4double  spanningAngleOfTheCone = 360.*deg;

	G4Cons* collim_cone = new G4Cons("collim_cone",pRmin1,pRmax1,pRmin2,
		  pRmax2,hightOfTheCone,startAngleOfTheCone,
		  spanningAngleOfTheCone);
	G4LogicalVolume *collim_log = new G4LogicalVolume(collim_cone,Vacuum,"collim_log",0,0,0);


	G4double innerRadiusOfTheTube = 0.*cm;
	G4double outerRadiusOfTheTube = 8.*cm;
	G4double hightOfTheTube = 3.1*cm;
	G4double startAngleOfTheTube = 0.*deg;
	G4double spanningAngleOfTheTube = 360.*deg;
	G4Tubs* tracker_tube = new G4Tubs("tracker_tube",innerRadiusOfTheTube,
			outerRadiusOfTheTube,hightOfTheTube,
			startAngleOfTheTube,spanningAngleOfTheTube);

	G4SubtractionSolid* CylMinusCone = new G4SubtractionSolid("Cyl-Cone",
			tracker_tube,collim_cone);
	G4LogicalVolume *CylMinusCone_log = new G4LogicalVolume(CylMinusCone,Tungsten,"CylminusCone_log",0,0,0);
	G4double CminusCPos_x = 0.*cm;
	G4double CminusCPos_y = 0.*cm;
	G4double CminusCPos_z = +6.2*cm;
	new G4PVPlacement(rotateMatrix,
			G4ThreeVector(CminusCPos_x, CminusCPos_y, CminusCPos_z),
			"CylMinusCone",
			CylMinusCone_log,
			physWorld,
			false,
			0);
}

void DetectorConstruction::ConstructPrimaryCollimator2()
{
	G4double totalHeight = 80.0*mm;

	// //-------------------- upper part----------------
	G4ThreeVector centre=G4ThreeVector(0.,0.,16.)-G4ThreeVector(0.,0.,(16. + totalHeight-76.)/2.);
	G4Tubs *PCUTube = new G4Tubs("PrimaryCollimatorUTube", 10.*mm, 40.0*mm, 10.*mm, 0.*deg, 360.*deg);
	G4LogicalVolume *PCUTubeLV = new G4LogicalVolume(PCUTube, Lead, "PrimaryCollimatorUTubeLV", 0, 0, 0);

	new G4PVPlacement(0, centre, "PrimaryCollimatorUTubePV", PCUTubeLV, physWorld, false, 0);

	// //-------------------- lower part----------------
	// Tube
	G4Tubs* PCLTube = new G4Tubs("PrimaryCollimatorLTube", 0., 40.*mm, 60./2.*mm, 0.*deg, 360.*deg);
	// Cone
	G4double coneAperture = 14.*deg;
	G4Cons* collimCone = new G4Cons("PrimaryCollimatorLCone", 0., (16.*std::tan(coneAperture))*mm, 0., (76.*std::tan(coneAperture))*mm, 30.*mm,  0.*deg, 360.*deg);

	G4LogicalVolume* PCLTubeLV = new G4LogicalVolume(PCLTube, Lead, "PCLTubeLV",0,0,0);
	G4LogicalVolume* collimConeLV = new G4LogicalVolume(collimCone, Vacuum, "collimConeLV",0,0,0);
	centre = G4ThreeVector(0.,0.,16.+60./2.);
	G4VPhysicalVolume *PCLTubePV = new G4PVPlacement(0, centre, "PCLTubePV", PCLTubeLV, physWorld, false, 0);

	centre = G4ThreeVector(0.,0.,0.);
	new G4PVPlacement(0, centre, "TubeMinusConeLPV", collimConeLV, PCLTubePV, false, 0);
}

void DetectorConstruction::ConstructVacuumWindow2()
{
	G4Tubs* BeWTube = new G4Tubs("BeWindowTube", 0., 50.*mm, 0.12*mm, 0.*deg, 360.*deg);
	G4LogicalVolume *BeWTubeLV = new G4LogicalVolume(BeWTube, Berylium, "BeWTubeLV", 0, 0, 0);
	new G4PVPlacement(0, G4ThreeVector(0.,0.,90.*mm), "BeWTubePV", BeWTubeLV, physWorld, false, 0);
}

void DetectorConstruction::ConstrucIonizationChamber2()
{
	G4Tubs* ICTubeW = new G4Tubs("ionizationChamberTube", 0., 3.75*2.54*10.*mm, 0.005*25.4*mm, 0.*deg, 360.*deg);
	G4Tubs* ICTubeP = new G4Tubs("ionizationChamberTube", 0., 3.75*2.54*10.*mm, 0.002*25.4*mm, 0.*deg, 360.*deg);

	G4ThreeVector centre;
	// W1
	centre.set(0.,0.,148.35*mm);
	G4LogicalVolume *PCUTubeW1LV = new G4LogicalVolume(ICTubeW, Kapton, "ionizationChamberTubeW1LV", 0, 0, 0);
	new G4PVPlacement(0, centre, "ionizationChamberTubeW1PV", PCUTubeW1LV, physWorld, false, 0);

	// P1
	centre.set(0.,0.,150.73*mm);
	G4LogicalVolume *PCUTubeP1LV = new G4LogicalVolume(ICTubeP, Kapton, "ionizationChamberTubeP1LV", 0, 0, 0);
	new G4PVPlacement(0, centre, "ionizationChamberTubeP1PV", PCUTubeP1LV, physWorld, false, 0);

	// W2
	centre.set(0.,0.,155.5*mm);
	G4LogicalVolume *PCUTubeW2LV = new G4LogicalVolume(ICTubeW, Kapton, "ionizationChamberTubeW2LV", 0, 0, 0);
	new G4PVPlacement(0, centre, "ionizationChamberTubeW2PV", PCUTubeW2LV, physWorld, false, 0);

	// P2
	centre.set(0.,0.,153.12*mm);
	G4LogicalVolume *PCUTubeP2LV = new G4LogicalVolume(ICTubeP, Kapton, "ionizationChamberTubeP2LV", 0, 0, 0);
	new G4PVPlacement(0, centre, "ionizationChamberTubeP2PV", PCUTubeP2LV, physWorld, false, 0);

	// W3
	centre.set(0.,0.,162.65*mm);
	G4LogicalVolume *PCUTubeW3LV = new G4LogicalVolume(ICTubeW, Kapton, "ionizationChamberTubeW3LV", 0, 0, 0);
	//new G4PVPlacement(0, centre, "ionizationChamberTubeW3PV", PCUTubeW3LV, physWorld, false, 0);

	// P3
	centre.set(0.,0.,157.88*mm);
	G4LogicalVolume *PCUTubeP3LV = new G4LogicalVolume(ICTubeP, Kapton, "ionizationChamberTubeP3LV", 0, 0, 0);
	new G4PVPlacement(0, centre, "ionizationChamberTubeP3PV", PCUTubeP3LV, physWorld, false, 0);

	// P4
	centre.set(0.,0.,160.27*mm);
	G4LogicalVolume *PCUTubeP4LV = new G4LogicalVolume(ICTubeP, Kapton, "ionizationChamberTubeP4LV", 0, 0, 0);
	new G4PVPlacement(0, centre, "ionizationChamberTubeP4PV", PCUTubeP4LV, physWorld, false, 0);	
}

void DetectorConstruction::ConstructMirror2()
{
	G4Tubs* MirrorTube = new G4Tubs("MirrorTube", 0., 86.*mm, 1.*mm, 0.*deg, 360.*deg);
	G4LogicalVolume *MirrorTubeLV = new G4LogicalVolume(MirrorTube, Mylar, "MirrorTubeLV", 0, 0, 0);
	G4RotationMatrix *cRotation = new G4RotationMatrix();
	cRotation -> rotateY(35.0*deg);
	new G4PVPlacement(cRotation, G4ThreeVector(0., 0., 220.*mm), "MirrorTubePV", MirrorTubeLV,physWorld, false, 0);
}

void DetectorConstruction::ConstructJaw1X()
{	
	G4String name = "Jaws1X";
	G4ThreeVector centre, halfSize;
	G4RotationMatrix *cRotation = new G4RotationMatrix();
	centre.set(0.,0.,(367.+78./2.)*mm);
	halfSize.set(50.*mm, 90.*mm, 78./2.*mm);
	G4Box *box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
	G4LogicalVolume *logVol = new G4LogicalVolume(box, Tungsten, name+"LV", 0, 0, 0);
	//SetJawAperture(1, centre, halfSize,cRotation);
	G4double theta, x, y, z, dx, dy; //, dz, top;
	x=centre.getX();
	y=centre.getY();
	z=centre.getZ();
	dx=halfSize.getX();
	dy=halfSize.getY();
	theta=fabs(atan(jaw1XAperture/isoCentre));
	centre.set(z*sin(theta)+dx*cos(theta), y, z*cos(theta)-dx*sin(theta));
	cRotation->rotateY(-theta);
	new G4PVPlacement(cRotation, centre, name+"PV", logVol, physWorld, false, 0);
}

void DetectorConstruction::ConstructJaw2X()
{
	G4String name="Jaws2X";
	G4ThreeVector centre, halfSize;
	G4RotationMatrix *cRotation = new G4RotationMatrix();
	centre.set(0.,0.,(367.+78./2.)*mm);
	halfSize.set(50.*mm, 90.*mm, 78./2.*mm);
	G4Box *box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
	G4LogicalVolume *logVol = new G4LogicalVolume(box, Tungsten, name+"LV", 0, 0, 0);
	//SetJawAperture(2, centre, halfSize, cRotation);
	G4double theta, x, y, z, dx, dy; //, dz, top;
	x=centre.getX();
	y=centre.getY();
	z=centre.getZ();
	dx=halfSize.getX();
	dy=halfSize.getY();
	theta=fabs(atan(jaw2XAperture/isoCentre));
	centre.set(-(z*sin(theta)+dx*cos(theta)), y, z*cos(theta)-dx*sin(theta));
	cRotation->rotateY(theta);
	new G4PVPlacement(cRotation, centre, name+"PV", logVol, physWorld, false, 0);
}

void DetectorConstruction::ConstructJaw1Y()
{
	G4String name="Jaws1Y";
	G4ThreeVector centre, halfSize;
	G4RotationMatrix *cRotation=new G4RotationMatrix();
	centre.set(0.,0.,(280.+78./2.)*mm);
	halfSize.set(90.*mm, 50.*mm, 78./2.*mm);
	G4Box *box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
	G4LogicalVolume *logVol = new G4LogicalVolume(box, Tungsten, name+"LV", 0, 0, 0);
	//SetJawAperture(3, centre, halfSize, cRotation);
	G4double theta, x, y, z, dx, dy; //, dz, top;
	x=centre.getX();
	y=centre.getY();
	z=centre.getZ();
	dx=halfSize.getX();
	dy=halfSize.getY();
	theta=fabs(atan(jaw1YAperture/isoCentre));
	centre.set(x, z*sin(theta)+dy*cos(theta), z*cos(theta)-dy*sin(theta));
	cRotation->rotateX(theta);
	new G4PVPlacement(cRotation, centre, name+"PV", logVol, physWorld, false, 0);
}

void DetectorConstruction::ConstructJaw2Y()
{
	G4String name = "Jaws2Y";
	G4ThreeVector centre, halfSize;
	G4RotationMatrix *cRotation=new G4RotationMatrix();
	centre.set(0.,0.,(280.+78./2.)*mm);
	halfSize.set(90.*mm, 50.*mm, 78./2.*mm);
	G4Box *box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
	G4LogicalVolume *logVol = new G4LogicalVolume(box, Tungsten, name+"LV", 0, 0, 0);
	//SetJawAperture(4, centre, halfSize, cRotation);
	G4double theta, x, y, z, dx, dy; //, dz, top;
	x=centre.getX();
	y=centre.getY();
	z=centre.getZ();
	dx=halfSize.getX();
	dy=halfSize.getY();
	theta=fabs(atan(jaw2YAperture/isoCentre));
	centre.set(x, -(z*sin(theta)+dy*cos(theta)), z*cos(theta)-dy*sin(theta));
	cRotation->rotateX(-theta);
	new G4PVPlacement(cRotation, centre, name+"PV", logVol, physWorld, false, 0);
}

void DetectorConstruction::ConstructWaterPhantom()
{
	G4ThreeVector centre, halfSize;
    G4ThreeVector fPhantomSize;   // Size of Water Phantom
	centre.set(0., 0., 115*cm);

	fPhantomSize.setX(phantomXDim);
  	fPhantomSize.setY(phantomYDim);
  	fPhantomSize.setZ(phantomZDim);
	halfSize.setX(fPhantomSize.getX()/2.);
	halfSize.setY(fPhantomSize.getY()/2.);
	halfSize.setZ(fPhantomSize.getZ()/2.);

	G4Box *fullWaterPhantomBox = new G4Box("fullWaterPhantomBox", halfSize.getX(), halfSize.getY(), halfSize.getZ());
 	G4LogicalVolume *fullWaterPhantomLV = new G4LogicalVolume(fullWaterPhantomBox, Water, "fullWaterPhantomLV", 0, 0, 0);
	G4VPhysicalVolume* physWaterPhantom = new G4PVPlacement(0, centre, "fullWaterPhantomPV", fullWaterPhantomLV, physWorld, false, 0);

	// Division along X axis
	G4double halfXVoxelSizeX = fPhantomSize.getX()/2./numberOfVoxelsAlongX;
	G4double halfXVoxelSizeY = fPhantomSize.getY()/2.;
	G4double halfXVoxelSizeZ = fPhantomSize.getZ()/2.;
	G4double voxelXThickness = 2*halfXVoxelSizeX;

	RODetectorXDivision = new G4Box("RODetectorXDivision", halfXVoxelSizeX, halfXVoxelSizeY, halfXVoxelSizeZ);
	RODetectorXDivisionLog = new G4LogicalVolume(RODetectorXDivision, Water, "RODetectorXDivisionLog", 0,0,0);
	RODetectorXDivisionPhys = new G4PVReplica("RODetectorXDivisionPhys", RODetectorXDivisionLog, physWaterPhantom, kXAxis, numberOfVoxelsAlongX, voxelXThickness);

	// Division along Y axis
	G4double halfYVoxelSizeX = halfXVoxelSizeX;
  	G4double halfYVoxelSizeY = fPhantomSize.getY()/2./numberOfVoxelsAlongY;
  	G4double halfYVoxelSizeZ = fPhantomSize.getZ()/2.;
  	G4double voxelYThickness = 2*halfYVoxelSizeY;

	RODetectorYDivision = new G4Box("RODetectorYDivision", halfYVoxelSizeX, halfYVoxelSizeY, halfYVoxelSizeZ);
  	RODetectorYDivisionLog = new G4LogicalVolume(RODetectorYDivision, Water, "RODetectorYDivisionLog", 0,0,0);
  	RODetectorYDivisionPhys = new G4PVReplica("RODetectorYDivisionPhys", RODetectorYDivisionLog, RODetectorXDivisionPhys, kYAxis, numberOfVoxelsAlongY, voxelYThickness);

	// Division along Z axis
	G4double halfZVoxelSizeX = halfXVoxelSizeX;
  	G4double halfZVoxelSizeY = halfYVoxelSizeY;
  	G4double halfZVoxelSizeZ = fPhantomSize.getZ()/2./numberOfVoxelsAlongZ;
  	G4double voxelZThickness = 2*halfZVoxelSizeZ;

	RODetectorZDivision = new G4Box("RODetectorZDivision",  halfZVoxelSizeX, halfZVoxelSizeY, halfZVoxelSizeZ);
  	RODetectorZDivisionLog = new G4LogicalVolume(RODetectorZDivision, Water, "RODetectorZDivisionLog", 0,0,0);
  	RODetectorZDivisionPhys = new G4PVReplica("RODetectorZDivisionPhys", RODetectorZDivisionLog, RODetectorYDivisionPhys, kZAxis, numberOfVoxelsAlongZ, voxelZThickness);
}

void DetectorConstruction::PrintInformation()
{}

void DetectorConstruction::ConstructSDandField()
{

}
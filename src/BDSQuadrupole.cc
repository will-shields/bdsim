//  
//   BDSIM, (C) 2001-2006 
//   
//   version 0.3
//  
//
//
//   Quadrupole class
//
//
//   History
//
//     21 Nov 2006 by Agapov,  v.0.3
//     22 Mar 2005 by Agapov, Carter,  v.0.2
//     x  x   2002 by Blair
//
//



#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSQuadrupole.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"

#include <map>

const int DEBUG = 1;


//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;

extern G4RotationMatrix* RotY90;
//============================================================

BDSQuadrupole::BDSQuadrupole(G4String aName,G4double aLength, 
			     G4double bpRad,G4double FeRad,
			     G4double bGrad, G4double tilt, G4String spec):
  BDSMultipole(aName,aLength, bpRad, FeRad,SetVisAttributes()),
  itsBGrad(bGrad)
{
 
  if(DEBUG) G4cout<<"BDSQUADRUPOLE : SPEC : "<<spec<<G4endl;

  itsType="quad";
  itsTilt = tilt;

  // get specific quadrupole type

  G4String qtype = getParameterValueString(spec, "type");

   if(DEBUG) G4cout<<"qtype : "<<qtype<<G4endl;

  // get outer radius

  G4double outR = getParameterValue(spec, "outR");
   if(DEBUG) G4cout<<"outR : "<<outR<<G4endl;
  
  if( outR < bpRad)
    {
      G4cerr <<"WARNING: "<<aName<<" has outer radius smaller than aperture!"<<G4endl;
      G4cerr <<"aper= "<<bpRad<<" mm, outR= "<<outR<<"mm"<<G4endl;
      G4cerr << "Setting to default - 1*mm"<<G4endl;
      outR = bpRad + 1*mm;
    }

  SetOuterRadius(outR*mm);

  

  if (!(*LogVolCount)[itsName])
    {
      BuildBPFieldAndStepper();
      BuildBPFieldMgr(itsStepper,itsMagField);
      BuildDefaultMarkerLogicalVolume();
	  
      BuildBeampipe(itsLength);

      // visual attributes
      G4VisAttributes* VisAtt = 
	new G4VisAttributes(G4Colour(1., 1., 1.));
      VisAtt->SetForceSolid(false);
      itsInnerBPLogicalVolume->SetVisAttributes(VisAtt);
      
      G4VisAttributes* VisAtt1 = 
	new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
      VisAtt1->SetForceSolid(true);
      itsBeampipeLogicalVolume->SetVisAttributes(VisAtt1);
      
      G4VisAttributes* VisAtt2 = 
      new G4VisAttributes(G4Colour(1., 0., 0.));
      VisAtt2->SetForceSolid(true);

      // build outer logical volume according to quad type
      if(qtype=="standard") 
	BuildOuterLogicalVolume(); // standard - quad with poles and pockets
      else if(qtype=="cylinder")  
	{ 
	  BuildDefaultOuterLogicalVolume(itsLength); // cylinder outer volume
	  itsOuterLogicalVolume->SetVisAttributes(VisAtt2);
	}
      else //default
	{ 
	  BuildDefaultOuterLogicalVolume(itsLength); // cylinder outer volume
	  itsOuterLogicalVolume->SetVisAttributes(VisAtt2);
	}

      SetMultipleSensitiveVolumes(itsBeampipeLogicalVolume);
      SetMultipleSensitiveVolumes(itsOuterLogicalVolume);
      //SetSensitiveVolume(itsBeampipeLogicalVolume);// for synchrotron
      //SetSensitiveVolume(itsOuterLogicalVolume);// for laserwire

         
      if(BDSGlobals->GetIncludeIronMagFields())
	{
	  G4double polePos[4];
	  G4double Bfield[3];

	  polePos[0]=-BDSGlobals->GetMagnetPoleRadius()/sqrt(2.0);
	  polePos[1]=BDSGlobals->GetMagnetPoleRadius()/sqrt(2.0);
	  polePos[2]=0.;
	  polePos[3]=-999.;//flag to use polePos rather than local track
	                   //coordinate in GetFieldValue
	    
	    
	  itsMagField->GetFieldValue(polePos,Bfield);
	  G4double BFldIron=
	    sqrt(Bfield[0]*Bfield[0]+Bfield[1]*Bfield[1])*
	    BDSGlobals->GetMagnetPoleSize()/
	    (BDSGlobals->GetComponentBoxSize()/2-
	     BDSGlobals->GetMagnetPoleRadius());
	  // Magnetic flux from a pole is divided in two directions
	  BFldIron/=2.;

	  BuildOuterFieldManager(4, BFldIron,pi/4);
	}
	  
      (*LogVolCount)[itsName]=1;
      (*LogVol)[itsName]=itsMarkerLogicalVolume;
    }
  else
    {
      (*LogVolCount)[itsName]++;
      if(BDSGlobals->GetSynchRadOn()&& BDSGlobals->GetSynchRescale())
	{
	  // with synchrotron radiation, the rescaled magnetic field
	  // means elements with the same name must have different
	  //logical volumes, becuase they have different fields
	  itsName+=BDSGlobals->StringFromInt((*LogVolCount)[itsName]);
	  BuildBPFieldAndStepper();
	  BuildBPFieldMgr(itsStepper,itsMagField);
	  BuildDefaultMarkerLogicalVolume();
	  
	  BuildBeampipe(itsLength);

	  // build outer logical volume according to quad type
	  if(qtype=="standard") 
	    BuildOuterLogicalVolume(); // standrd - quad with poles and pockets
	  else if(qtype=="cylinder")   
	    BuildDefaultOuterLogicalVolume(itsLength); // cylinder outer volume
	  else //default
	    BuildDefaultOuterLogicalVolume(itsLength); // cylinder outer volume


	  SetSensitiveVolume(itsBeampipeLogicalVolume);// for synchrotron
	  //SetSensitiveVolume(itsOuterLogicalVolume);// for laserwire      
	  (*LogVol)[itsName]=itsMarkerLogicalVolume;
	}
      else
	{
	  itsMarkerLogicalVolume=(*LogVol)[itsName];
	}      
    }


}
  
void BDSQuadrupole::SynchRescale(G4double factor)
{
  itsStepper->SetBGrad(factor*itsBGrad);
  itsMagField->SetBGrad(factor*itsBGrad);
  if(DEBUG) G4cout << "Quad " << itsName << " has been scaled" << G4endl;
}


G4VisAttributes* BDSQuadrupole::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(1,0,0));
  return itsVisAttributes;
}


void BDSQuadrupole::BuildBPFieldAndStepper()
{
  // set up the magnetic field and stepper
  //  G4ThreeVector Bfield(0.,0.,0.);
  //  itsMagField=new G4UniformMagField(Bfield);
  itsMagField=new BDSQuadMagField(itsBGrad);

  itsEqRhs=new G4Mag_UsualEqRhs(itsMagField);
  
  itsStepper=new BDSQuadStepper(itsEqRhs);
  itsStepper->SetBGrad(itsBGrad);
  itsStepper->SetVolLength(itsLength);
}



void BDSQuadrupole::BuildOuterLogicalVolume()
{

  //G4cout<<"length="<<aLength<<G4endl;

  // compute saggita:
  G4double sagitta=0.;

  G4double outerRadius = itsOuterR;
  if(itsOuterR==0) outerRadius = BDSGlobals->GetComponentBoxSize()/2;

  if(itsNSegments>1)
    {
      sagitta=itsLength/itsAngle*(1.-cos(itsAngle/2.));
    }

  // marker volume
  itsOuterLogicalVolume=
    new G4LogicalVolume(new G4Tubs(itsName+"_solid",
				   itsInnerIronRadius+sagitta,
				   outerRadius * sqrt(2.0),
				   itsLength/2,
				   0,twopi*radian),
			theMaterials->LCVacuum,
			itsName+"_outer");
  
  // create one quadrant of the qoadrupole

  G4LogicalVolume* lQuadrant = 
    new G4LogicalVolume(new G4Tubs(itsName+"_solid",
				   itsInnerIronRadius+sagitta,
				   outerRadius * sqrt(2.0),
				   itsLength/2,
				   0,pi/ 2 *radian),
			theMaterials->LCVacuum,
			itsName+"_outer");
  
  // pole 

  G4double poleR = itsBpRadius;
  G4double phiStart = -pi / 4;
  G4double dPhi = pi / 2;

  G4LogicalVolume* lPole = 
    new G4LogicalVolume(new G4Tubs(itsName+"_pole",
				   0,
				   poleR,
				   itsLength/2,
				   phiStart,
				   dPhi),
			theMaterials->LCIron,
			itsName+"pole_outer");

  G4RotationMatrix* rotPole = new G4RotationMatrix;
  //rotPole = NULL;
  rotPole->rotateZ(3.*pi / 4.);

  G4double xPole = (poleR + itsBpRadius) / sqrt(2.0);
  G4double yPole = (poleR + itsBpRadius) / sqrt(2.0);


  G4VPhysicalVolume* itsPhysiQPole1 = 
    new G4PVPlacement(
		      rotPole,			  // rotation
		      G4ThreeVector(xPole,yPole,0),                      // its position
		      lPole,   // its logical volume
		      itsName+"_solid",	  // its name
		      lQuadrant,  // its mother  volume
		      false,		  // no boolean operation
		      0);		          // copy number
  
  // color-coding for the pole
  
  G4VisAttributes* VisAtt = 
    new G4VisAttributes(G4Colour(1., 0., 0.));
  VisAtt->SetForceSolid(true);
  lPole->SetVisAttributes(VisAtt);


  // yoke pieces

  G4double rYoke = itsOuterR - poleR - itsBpRadius + poleR * cos(dPhi / 2);

  if(rYoke > 0 ) // place yoke
    {

      // random ...
      G4double rYoke1 =  itsOuterR; // outer length
      G4double rYoke2 =  itsBpRadius;  // inner length 

      G4LogicalVolume* lYoke1 = 
	new G4LogicalVolume(new G4Trd(itsName+"_yoke1",
				      rYoke1 / 2,
				      rYoke2 / 2,
				      itsLength/2,
				      itsLength/2,
				      rYoke/2),
			    theMaterials->LCIron,
			    itsName+"yoke_outer1");

      G4RotationMatrix* rotYoke = new G4RotationMatrix;
      //rotYoke = NULL;
      rotYoke->rotateX( - pi / 2.);
      rotYoke->rotateY(  pi / 4.);

      G4double xYoke = (poleR - poleR * cos(dPhi / 2) + itsBpRadius + rYoke/2) / sqrt(2.0);
      G4double yYoke = (poleR - poleR * cos(dPhi / 2) + itsBpRadius + rYoke/2) / sqrt(2.0);


      G4VPhysicalVolume* itsPhysiQYoke1 = 
	new G4PVPlacement(
			  rotYoke,			  // rotation
			  G4ThreeVector(xYoke,yYoke,0),                      // its position
			  lYoke1,   // its logical volume
			  itsName+"_yoke_solid",	  // its name
			  lQuadrant,  // its mother  volume
			  false,		  // no boolean operation
			  0);		          // copy number

      // color-coding 
      
      G4VisAttributes* VisAtt1 = 
	new G4VisAttributes(G4Colour(1., 0., 0.4));
      VisAtt1->SetForceSolid(true);
      lYoke1->SetVisAttributes(VisAtt1);

      G4double xYoke2 = rYoke1;
      G4double yYoke2 = rYoke2;


      G4LogicalVolume* lYoke2 = 
	new G4LogicalVolume(new G4Box(itsName+"_yoke1",
				      rYoke1 / 2,
				      rYoke2 / 2,
				      itsLength/2),
			    theMaterials->LCIron,
			    itsName+"yoke_outer1");

     //  G4VPhysicalVolume* itsPhysiQYoke2 = 
// 	new G4PVPlacement(
// 			  NULL,			  // rotation
// 			  G4ThreeVector(xYoke2,yYoke2,0),                      // its position
// 			  lYoke2,   // its logical volume
// 			  itsName+"_yoke_solid",	  // its name
// 			  lQuadrant,  // its mother  volume
// 			  false,		  // no boolean operation
// 			  0);		          // copy number

      G4VisAttributes* VisAtt2 = 
	new G4VisAttributes(G4Colour(1., 0., 0.4));
      VisAtt2->SetForceSolid(true);
      lYoke2->SetVisAttributes(VisAtt1);
      

    }
  else
    {
      G4cerr<<"Not enough place for yoke..."<<G4endl;
    }


  // put all quadrants in the outer volume

 
  G4VPhysicalVolume* itsPhysiQuadrant1 = 
    new G4PVPlacement(
		      NULL,			  // rotation
		      0,                      // its position
		      lQuadrant,   // its logical volume
		      itsName+"_solid",	  // its name
		      itsOuterLogicalVolume,  // its mother  volume
		      false,		  // no boolean operation
		      0);		          // copy number

  G4RotationMatrix* rotQ2= new  G4RotationMatrix;
  rotQ2->rotateZ( pi / 2.);

  G4VPhysicalVolume* itsPhysiQuadrant2 = 
    new G4PVPlacement(
		      rotQ2,			  // rotation
		      0,                      // its position
		      lQuadrant,   // its logical volume
		      itsName+"_solid",	  // its name
		      itsOuterLogicalVolume,  // its mother  volume
		      false,		  // no boolean operation
		      0);		          // copy number

  G4RotationMatrix* rotQ3= new  G4RotationMatrix;
  rotQ3->rotateZ( pi );
  
  G4VPhysicalVolume* itsPhysiQuadrant3 = 
    new G4PVPlacement(
		      rotQ3,			  // rotation
		      0,                      // its position
		      lQuadrant,   // its logical volume
		      itsName+"_solid",	  // its name
		      itsOuterLogicalVolume,  // its mother  volume
		      false,		  // no boolean operation
		      0);		          // copy number


  G4RotationMatrix* rotQ4= new  G4RotationMatrix;
  rotQ4->rotateZ( 3. * pi / 2.);
  
  G4VPhysicalVolume* itsPhysiQuadrant4 = 
    new G4PVPlacement(
		      rotQ4,			  // rotation
		      0,                      // its position
		      lQuadrant,   // its logical volume
		      itsName+"_solid",	  // its name
		      itsOuterLogicalVolume,  // its mother  volume
		      false,		  // no boolean operation
		      0);		          // copy number


  //rotQ->rotateZ( pi / 4.);


  // insert the outer volume into the marker volume

  G4RotationMatrix* Rot=NULL;
  if(itsAngle!=0) Rot=RotY90;
  
  G4VPhysicalVolume* itsPhysiComp = 
    new G4PVPlacement(
		      Rot,			  // rotation
		      0,                      // its position
		      itsOuterLogicalVolume,   // its logical volume
		      itsName+"_solid",	  // its name
		      itsMarkerLogicalVolume,  // its mother  volume
		      false,		  // no boolean operation
		      0);		          // copy number
  
  itsOuterUserLimits =
    new G4UserLimits("quadrupole cut",itsLength,DBL_MAX,DBL_MAX,
		     BDSGlobals->GetThresholdCutCharged());
  //  itsOuterUserLimits->SetMaxAllowedStep(aLength);
  itsOuterLogicalVolume->SetUserLimits(itsOuterUserLimits);
  
   
}




BDSQuadrupole::~BDSQuadrupole()
{
  delete itsVisAttributes;
  delete itsMarkerLogicalVolume;
  delete itsOuterLogicalVolume;
  delete itsPhysiComp;
  delete itsMagField;
  delete itsEqRhs;
  delete itsStepper;
}

/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
*/
#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSSectorBend.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4IntersectionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"
#include "G4SubtractionSolid.hh"

#include <map>

const int DEBUG = 0;

//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;
extern G4RotationMatrix* RotY90;

//============================================================

BDSSectorBend::BDSSectorBend(G4String aName,G4double aLength, 
			     G4double bpRad,G4double FeRad,
			     G4double bField, G4double angle, G4double outR,
			     G4double tilt,  G4double bGrad, G4int nSegments):
  BDSMultipole(aName,aLength, bpRad, FeRad,SetVisAttributes(),0,0,angle)
{
  SetOuterRadius(outR);
  itsTilt = tilt;
  itsBField = bField;
  itsBGrad = bGrad;

  itsType="sbend";

  if (!(*LogVolCount)[itsName] )
    {
      BuildSBMarkerLogicalVolume();

      BuildBeampipe2(this,itsLength,itsAngle);

      BuildBPFieldAndStepper();

      BuildBPFieldMgr(itsStepper,itsMagField);

      BuildDefaultOuterLogicalVolume(itsLength);
      //BuildDefaultOuterLogicalVolume2(this,itsLength,itsAngle);

      SetMultipleSensitiveVolumes(itsBeampipeLogicalVolume);
      SetMultipleSensitiveVolumes(itsOuterLogicalVolume);

      //SetSensitiveVolume(itsBeampipeLogicalVolume);// for synchrotron
      //SetSensitiveVolume(itsOuterLogicalVolume);// otherwise

      if(BDSGlobals->GetIncludeIronMagFields())
	{
	  G4double polePos[4];
	  G4double Bfield[3];

	  polePos[0]=0.;
	  polePos[1]=BDSGlobals->GetMagnetPoleRadius();
	  polePos[2]=0.;
	  polePos[0]=-999.;//flag to use polePos rather than local track
	  //coordinate in GetFieldValue	    
	  
      	  itsMagField->GetFieldValue(polePos,Bfield);
	  G4double BFldIron=
	  sqrt(Bfield[0]*Bfield[0]+Bfield[1]*Bfield[1])*
	  BDSGlobals->GetMagnetPoleSize()/
	  (BDSGlobals->GetComponentBoxSize()/2-
	  BDSGlobals->GetMagnetPoleRadius());

	  // Magnetic flux from a pole is divided in two directions
	  BFldIron/=2.;
	  
	  //BuildOuterFieldManager(2, BFldIron,0);
	}

      
      // vis attr
      G4VisAttributes* VisAtt = 
	new G4VisAttributes(G4Colour(0., 0., 0));
      VisAtt->SetForceSolid(true);
      itsInnerBPLogicalVolume->SetVisAttributes(VisAtt);
      
      G4VisAttributes* VisAtt1 = 
	new G4VisAttributes(G4Colour(0.1, 0.1, 0.1));
      VisAtt1->SetForceSolid(true);
      itsBeampipeLogicalVolume->SetVisAttributes(VisAtt1);
      
      G4VisAttributes* VisAtt2 = 
	new G4VisAttributes(G4Colour(0., 0., 1.));
      VisAtt2->SetForceSolid(true);
      itsOuterLogicalVolume->SetVisAttributes(VisAtt2);
      


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

	  BuildSBMarkerLogicalVolume();

	  BuildBeampipe2(this,itsLength,itsAngle);
	
	  BuildBPFieldAndStepper();

	  BuildBPFieldMgr(itsStepper,itsMagField);

	  BuildDefaultOuterLogicalVolume(itsLength);
	  //  BuildDefaultOuterLogicalVolume2(this,itsLength,itsAngle);

	  SetMultipleSensitiveVolumes(itsBeampipeLogicalVolume);
	  SetMultipleSensitiveVolumes(itsOuterLogicalVolume);

	  // vis attr
	  G4VisAttributes* VisAtt = 
	    new G4VisAttributes(G4Colour(0., 0., 0));
	  VisAtt->SetForceSolid(true);
	  itsInnerBPLogicalVolume->SetVisAttributes(VisAtt);
	  
	  G4VisAttributes* VisAtt1 = 
	    new G4VisAttributes(G4Colour(0.1, 0.1, 0.1));
	  VisAtt1->SetForceSolid(true);
	  itsBeampipeLogicalVolume->SetVisAttributes(VisAtt1);
	  
	  G4VisAttributes* VisAtt2 = 
	    new G4VisAttributes(G4Colour(0., 0., 1.));
	  VisAtt2->SetForceSolid(true);
	  itsOuterLogicalVolume->SetVisAttributes(VisAtt2);
	  
	  
	  (*LogVol)[itsName]=itsMarkerLogicalVolume;
	}
      else
	{
	  itsMarkerLogicalVolume=(*LogVol)[itsName];
	}      
    }

}

void BDSSectorBend::SynchRescale(G4double factor)
{
  itsStepper->SetBGrad(itsBGrad*factor);
  itsStepper->SetBField(-itsBField*factor);
  // note that there are no methods to set the BDSSBendMagField as this
  // class does not do anything with the BFields.
  if(DEBUG) G4cout << "Sbend " << itsName << " has been scaled" << G4endl;
}

G4VisAttributes* BDSSectorBend::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0,1,1));
  return itsVisAttributes;
}


void BDSSectorBend::BuildBPFieldAndStepper()
{
  // set up the magnetic field and stepper
  G4ThreeVector Bfield(0.,-itsBField,0.);
  itsMagField=new BDSSbendMagField(Bfield,itsLength,itsAngle);
  
  
  itsEqRhs=new G4Mag_UsualEqRhs(itsMagField);  
  
  
  //itsStepper=new BDSHelixStepper(itsEqRhs);
  itsStepper = new myQuadStepper(itsEqRhs); // note the - sign...
  itsStepper->SetBField(-itsBField);
 
  itsStepper->SetBGrad(itsBGrad);

  BuildBPFieldMgr(itsStepper,itsMagField);

  itsBeampipeLogicalVolume->SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
  itsInnerBPLogicalVolume->SetFieldManager(itsBPFieldMgr,false);

}


void BDSSectorBend::BuildSBMarkerLogicalVolume()
{
  G4double LCComponentBoxSize=BDSGlobals->GetComponentBoxSize();

  G4double xHalfLengthMinus=(itsLength/itsAngle)*sin(itsAngle/2)
      -fabs(cos(itsAngle/2))*LCComponentBoxSize*tan(itsAngle/2)/2
      +BDSGlobals->GetLengthSafety()/2;
  
    G4double xHalfLengthPlus=(itsLength/itsAngle)*sin(itsAngle/2)
      +fabs(cos(itsAngle/2))*LCComponentBoxSize*tan(itsAngle/2)/2
      +BDSGlobals->GetLengthSafety()/2;
  
 
  G4String LocalLogicalName=itsName;
  
  itsMarkerLogicalVolume=    
    new G4LogicalVolume(new G4Trd(itsName+"_marker" ,             
				  xHalfLengthPlus,      // x hlf lgth at +z
				  xHalfLengthMinus,     // x hlf lgth at -z
				  LCComponentBoxSize/2, // y hlf lgth at +z
				  LCComponentBoxSize/2, // y hlf lgth at -z
				  fabs(cos(itsAngle/2))*LCComponentBoxSize/2),// z hlf lgth
			theMaterials->LCVacuum,
			LocalLogicalName+"_marker");

  itsMarkerUserLimits = new G4UserLimits(DBL_MAX,DBL_MAX,DBL_MAX);
  itsMarkerUserLimits->SetMaxAllowedStep(itsLength);
  itsMarkerLogicalVolume->SetUserLimits(itsMarkerUserLimits);
}


// construct a beampipe for sector bend
//void BDSSectorBend::BuildBeampipe2(G4double length, G4double angle)
void BuildBeampipe2(BDSSectorBend *sb,G4double length, G4double angle)
{

 G4double LCComponentBoxSize=BDSGlobals->GetComponentBoxSize();
 
 G4double xHalfLengthMinus=(sb->itsLength/sb->itsAngle)*sin(sb->itsAngle/2)
    -fabs(cos(sb->itsAngle/2)) * LCComponentBoxSize*tan(sb->itsAngle/2)/2
    +BDSGlobals->GetLengthSafety()/2;

 G4double xHalfLengthPlus=(sb->itsLength/sb->itsAngle)*sin(sb->itsAngle/2)
    +fabs(cos(sb->itsAngle/2)) * LCComponentBoxSize*tan(sb->itsAngle/2)/2
    +BDSGlobals->GetLengthSafety()/2;

  // use default beampipe material
  G4Material *material =  theMaterials->GetMaterial( BDSGlobals->GetPipeMaterialName());
  

  // *** toroidal beampipes - problems with G4Torus::DistanceToTorus
  // *** when calling G4PolynomialSolver.Solve()
  /*
 G4double halfLength = 0.5 * (xHalfLengthMinus + xHalfLengthPlus) ;

 G4double rSwept = halfLength /  fabs( sin( angle / 2 ) );

 G4Torus *pipeTube = new G4Torus(sb->itsName+"_pipe_outer",
				  sb->itsBpRadius-BDSGlobals->GetBeampipeThickness(),  // inner R
				  sb->itsBpRadius, // outer R
				  rSwept , // swept R
				  0, // starting phi
				  fabs(angle) ); // delta phi


  G4Torus *pipeInner = new G4Torus(sb->itsName+"_pipe_inner",
				   0,   // inner R
				   sb->itsBpRadius-BDSGlobals->GetBeampipeThickness()-0.000*mm,// outer
				   rSwept , // swept R
				   0, // starting phi
				   fabs(angle) ); // delta phi
  
  sb->itsBeampipeLogicalVolume=	
    new G4LogicalVolume(pipeTube,
			theMaterials->LCAluminium,
			sb->itsName+"_bmp_logical");
  
  sb->itsInnerBPLogicalVolume=	
    new G4LogicalVolume(pipeInner,
			theMaterials->LCVacuum,
			sb->itsName+"_bmp_Inner_log");
  
  G4RotationMatrix* Rot= new G4RotationMatrix;
  
  Rot->rotateX(pi/2 * rad);
  Rot->rotateZ(- ( pi/2 - fabs(angle)/2 ) * rad);

  G4ThreeVector pipeTorusOrigin(0, 0, rSwept * fabs( cos(angle / 2) ) );

  if(angle < 0)
    {
      //Rot->rotateY(pi);
      Rot->rotateZ(pi);
      pipeTorusOrigin.setZ(-rSwept * fabs( cos(angle / 2) ) );
    }
  

  G4VPhysicalVolume* PhysiInner = 
    new G4PVPlacement(
		      Rot,		       // rotation
		      pipeTorusOrigin,	               // at (0,0,0)
		      sb->itsInnerBPLogicalVolume, // its logical volume
		      sb->itsName+"_InnerBmp",     // its name
		      sb->itsMarkerLogicalVolume, // its mother  volume
		      false,		       // no boolean operation
		      0);		       // copy numbe

  G4VPhysicalVolume* PhysiComp = 
    new G4PVPlacement(
		      Rot,			     // rotation
		      pipeTorusOrigin,	                     // at (0,0,0)
		      sb->itsBeampipeLogicalVolume,  // its logical volume
		      sb->itsName+"_bmp",	     // its name
		      sb->itsMarkerLogicalVolume,     // its mother  volume
		      false,		     // no boolean operation
		      0);		             // copy number
  
  */  
  // *** try with tubes - but should really be rectangular apertures
    
  G4double tubLen = 0.5 * ( xHalfLengthPlus + xHalfLengthMinus );

  G4Tubs *pipeTubs = new G4Tubs(sb->itsName+"_pipe_outer",
				 sb->itsBpRadius-BDSGlobals->GetBeampipeThickness(),  // inner R
				 sb->itsBpRadius, // outer R
				 tubLen , // length
				 0, // starting phi
				 twopi * rad ); // delta phi
  
   G4Tubs *pipeInner = new G4Tubs(sb->itsName+"_pipe_inner",
				 0,  // inner R
				 sb->itsBpRadius-BDSGlobals->GetBeampipeThickness(), // outer R
				 tubLen , // length
				 0, // starting phi
				 twopi * rad ); // delta phi

  /* uncomment if using intersection solids below
  G4Trd *markerBox = new G4Trd(sb->itsName+"_marker_box" ,             
			       xHalfLengthPlus,      // x hlf lgth at +z
			       xHalfLengthMinus,     // x hlf lgth at -z
			       LCComponentBoxSize/2, // y hlf lgth at +z
			       LCComponentBoxSize/2, // y hlf lgth at -z
			       fabs(cos(sb->itsAngle/2))*LCComponentBoxSize/2);
  */

  G4RotationMatrix* InvRot= new G4RotationMatrix;
  G4RotationMatrix* Rot= new G4RotationMatrix;

  G4double rotAngle = -pi/2 * rad; // * sign(sb->itsAngle);
  InvRot->rotateY(-rotAngle + pi);
  Rot->rotateY(rotAngle);

  //G4Transform3D transform(*InvRot,G4ThreeVector(0,0,0) );  

  // remember to uncomment markerBox above if using these
  // G4IntersectionSolid *pTubsIntersMarker = 
    // new G4IntersectionSolid("_inner_tubs_intersects_marker", pipeTubs, 
// 			    markerBox, transform ); 
  
//   G4IntersectionSolid *pInnerIntersMarker = 
//     new G4IntersectionSolid("_inner_pipe_intersects_marker", pipeInner, 
// 			    markerBox, transform ); 


 //  sb->itsBeampipeLogicalVolume=	
//     new G4LogicalVolume(pTubsIntersMarker,
// 			theMaterials->LCAluminium,
// 			sb->itsName+"_bmp_logical");
  
//   sb->itsInnerBPLogicalVolume=	
//     new G4LogicalVolume(pInnerIntersMarker,
// 			theMaterials->LCVacuum,
// 			sb->itsName+"_bmp_Inner_log");



   sb->itsBeampipeLogicalVolume=	
    new G4LogicalVolume(pipeTubs,
			material,
			sb->itsName+"_bmp_logical");
  
  sb->itsInnerBPLogicalVolume=	
    new G4LogicalVolume(pipeInner,
			material,
			sb->itsName+"_bmp_Inner_log");

  G4VPhysicalVolume* PhysiInner;
  PhysiInner = new G4PVPlacement(
		      Rot,		       // rotation
		      0,	               // at (0,0,0)
		      sb->itsInnerBPLogicalVolume, // its logical volume
		      sb->itsName+"_InnerBmp",     // its name
		      sb->itsMarkerLogicalVolume, // its mother  volume
		      false,		       // no boolean operation
		      0);		       // copy number
  
  G4VPhysicalVolume* PhysiComp;
  PhysiComp = new G4PVPlacement(
		      Rot,			     // rotation
		      0,	                     // at (0,0,0)
		      sb->itsBeampipeLogicalVolume,  // its logical volume
		      sb->itsName+"_bmp",	     // its name
		      sb->itsMarkerLogicalVolume,     // its mother  volume
		      false,		     // no boolean operation
		      0);		             // copy number
  

  //


  // **** Try using tube with opening and closing angles....
  // The geometry building WORKS - but the reference frame needs to be changed
  // in the stepper - so do not use!!
  /*  
  // length of tube be the Vertical Gap

  // Inner and Outer Radius will be the Horizontal Gap of beampipe
  G4double centerR = halfLength /  fabs( sin( angle / 2 ) );
  G4double InnerR = centerR - sb->itsBpRadius;
  G4double OuterR = centerR + sb->itsBpRadius;

  // Inner and Outer Radius will be the Horizontal size of outer solid
  G4double outerRadius = sb->itsOuterR;
  if(outerRadius==0) outerRadius = BDSGlobals->GetComponentBoxSize()/2;
  G4double SolidOuterR = centerR + outerRadius;
  G4double SolidInnerR = centerR - outerRadius;

  //start phi and delta phi

  G4double sPhi = 3*pi/2-fabs(angle/2);
  G4double dPhi = fabs(angle);

  if(angle<0) sPhi-=pi;

  G4Tubs *pipeInner = new G4Tubs(sb->itsName+"_pipe_inner",
				 InnerR+BDSGlobals->GetBeampipeThickness(),
				 OuterR-BDSGlobals->GetBeampipeThickness(),
				 sb->itsBpRadius-2*BDSGlobals->GetBeampipeThickness(),
				 sPhi, // starting phi
				 dPhi); // delta phi

  G4Tubs *pipeOuter_tmp = new G4Tubs(sb->itsName+"_pipe_outer_tmp",
				 InnerR,
				 OuterR,
				 sb->itsBpRadius,
				 sPhi, // starting phi
  				 dPhi); // delta phi

  G4Tubs *OuterSolid_tmp = new G4Tubs(sb->itsName+"_solid_tmp",
				  SolidInnerR,
				  SolidOuterR,
				  outerRadius,
				  sPhi, // starting phi
				  dPhi); // delta phi
  
  G4SubtractionSolid *pipeOuter = new G4SubtractionSolid("_pipe_outer",
							 pipeOuter_tmp,
							 pipeInner);
  G4SubtractionSolid *OuterSolid = new G4SubtractionSolid("_solid",
							  OuterSolid_tmp,
							  pipeOuter_tmp);
							

  sb->itsBeampipeLogicalVolume=	
    new G4LogicalVolume(pipeOuter,
			theMaterials->LCAluminium,
			sb->itsName+"_bmp_logical");
  
  sb->itsInnerBPLogicalVolume=	
    new G4LogicalVolume(pipeInner,
			theMaterials->LCVacuum,
			sb->itsName+"_bmp_Inner_log");

  sb->itsOuterLogicalVolume=	
    new G4LogicalVolume(OuterSolid,
			theMaterials->LCIron,
			sb->itsName+"_outer");

  G4RotationMatrix* InvRot= new G4RotationMatrix;
  G4RotationMatrix* Rot= new G4RotationMatrix;

  G4double rotAngle = -pi/2 * rad; // * sign(sb->itsAngle);
  InvRot->rotateY(-rotAngle + pi);
  Rot->rotateX(rotAngle);

   G4ThreeVector pipeTorusOrigin(0, 0, centerR * fabs( cos(angle / 2) ) );

   if(angle < 0)
     {
       //Rot->rotateY(pi);
       //Rot->rotateZ(pi);
       pipeTorusOrigin.setZ(-centerR * fabs( cos(angle / 2) ) );
     }

  new G4PVPlacement(
		    Rot,		       // rotation
		    pipeTorusOrigin,	               // at (0,0,0)
		    sb->itsInnerBPLogicalVolume, // its logical volume
		    sb->itsName+"_InnerBmp",     // its name
		    sb->itsMarkerLogicalVolume, // its mother  volume
		    false,		       // no boolean operation
		    0);		       // copy number

  new G4PVPlacement(
		    Rot,		       // rotation
		    pipeTorusOrigin,	               // at (0,0,0)
		    sb->itsBeampipeLogicalVolume, // its logical volume
		    sb->itsName+"_bmp",     // its name
		    sb->itsMarkerLogicalVolume, // its mother  volume
		    false,		       // no boolean operation
		    0);		       // copy number

  new G4PVPlacement(
		    Rot,		       // rotation
		    pipeTorusOrigin,         // at (0,0,0)
		    sb->itsOuterLogicalVolume, // its logical volume
		    sb->itsName+"_solid",     // its name
		    sb->itsMarkerLogicalVolume, // its mother  volume
		    false,		       // no boolean operation
		    0);		       // copy number
  

  */  
  sb->itsBeampipeUserLimits =
    new G4UserLimits("beampipe cuts",DBL_MAX,DBL_MAX,DBL_MAX,
  		     BDSGlobals->GetThresholdCutCharged());
  
  sb->itsInnerBeampipeUserLimits =
    new G4UserLimits("inner beamipe cuts",DBL_MAX,DBL_MAX,DBL_MAX,
  		     BDSGlobals->GetThresholdCutCharged());
  /*  
  sb->itsOuterUserLimits =
    new G4UserLimits("sbend cut",sb->itsLength,DBL_MAX,DBL_MAX,
		     BDSGlobals->GetThresholdCutCharged());
  
  sb->itsOuterLogicalVolume->SetUserLimits(sb->itsOuterUserLimits);
  */
  sb->itsBeampipeUserLimits->SetMaxAllowedStep(sb->itsLength);
  sb->itsBeampipeLogicalVolume->SetUserLimits(sb->itsBeampipeUserLimits);
  
  sb->itsInnerBeampipeUserLimits->SetMaxAllowedStep(sb->itsLength);
  sb->itsInnerBPLogicalVolume->SetUserLimits(sb->itsInnerBeampipeUserLimits);

  // zero field in the marker volume
  sb->itsMarkerLogicalVolume->
    SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
 
  
}
/*
void BuildDefaultOuterLogicalVolume2(BDSSectorBend* sb,G4double aLength, G4double angle, G4bool OuterMaterialIsVacuum){
 G4double LCComponentBoxSize=BDSGlobals->GetComponentBoxSize();
 
 G4double xHalfLengthMinus=(sb->itsLength/sb->itsAngle)*sin(sb->itsAngle/2)
    -fabs(cos(sb->itsAngle/2)) * LCComponentBoxSize*tan(sb->itsAngle/2)/2
    +BDSGlobals->GetLengthSafety()/2;

 G4double xHalfLengthPlus=(sb->itsLength/sb->itsAngle)*sin(sb->itsAngle/2)
    +fabs(cos(sb->itsAngle/2)) * LCComponentBoxSize*tan(sb->itsAngle/2)/2
    +BDSGlobals->GetLengthSafety()/2;

 G4double halfLength = 0.5 * (xHalfLengthMinus + xHalfLengthPlus) ;

 G4double rSwept = halfLength /  fabs( sin( angle / 2 ) );

  //G4cout<<"length="<<aLength<<G4endl;

  // compute saggita:
  G4double sagitta=0.;

  G4double outerRadius = sb->itsOuterR;
  if(sb->itsOuterR==0) outerRadius = BDSGlobals->GetComponentBoxSize()/2;

  if(sb->itsNSegments>1)
    {
      sagitta=sb->itsLength/sb->itsAngle*(1.-cos(sb->itsAngle/2.));
    }
  if(OuterMaterialIsVacuum)
    {
    sb->itsOuterLogicalVolume=
      new G4LogicalVolume(new G4Torus(sb->itsName+"_solid",
                                     sb->itsInnerIronRadius+sagitta,
                                     outerRadius,
                                     rSwept,
				      0,fabs(angle)),
                          theMaterials->LCVacuum,
                          sb->itsName+"_outer");
    }
  if(!OuterMaterialIsVacuum)
    {
    sb->itsOuterLogicalVolume=
      new G4LogicalVolume(new G4Torus(sb->itsName+"_solid",
                                     sb->itsInnerIronRadius+sagitta,
                                     outerRadius,
                                     rSwept,
                                     0,fabs(angle)),
                          theMaterials->LCIron,
                          sb->itsName+"_outer");
    }

  G4RotationMatrix* Rot= new G4RotationMatrix;
  
  Rot->rotateX(pi/2 * rad);
  Rot->rotateZ(- ( pi/2 - fabs(angle)/2 ) * rad);

  G4ThreeVector outerTorusOrigin(0, 0, rSwept * fabs( cos(angle / 2) ) );

  if(angle < 0)
    {
      //Rot->rotateY(pi);
      Rot->rotateZ(pi);
      outerTorusOrigin.setZ(-rSwept * fabs( cos(angle / 2) ) );
    }
  
  G4VPhysicalVolume* itsPhysiComp =
    new G4PVPlacement(
                      Rot,                        // rotation
                      outerTorusOrigin,           // its position
                      sb->itsOuterLogicalVolume,   // its logical volume
                      sb->itsName+"_solid",   // its name
                      sb->itsMarkerLogicalVolume,  // its mother  volume
                      false,              // no boolean operation
                      0);                         // copy number

  sb->itsOuterUserLimits =
    new G4UserLimits("multipole cut",aLength,DBL_MAX,DBL_MAX,
                     BDSGlobals->GetThresholdCutCharged());
  //  itsOuterUserLimits->SetMaxAllowedStep(aLength);
  sb->itsOuterLogicalVolume->SetUserLimits(sb->itsOuterUserLimits);

}
*/
BDSSectorBend::~BDSSectorBend()
{
  
  delete itsVisAttributes;
  delete itsMarkerLogicalVolume;
  delete itsOuterLogicalVolume;
  delete itsPhysiComp;
  delete itsMagField;
  delete itsEqRhs;
  delete itsStepper;
}

/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
*/
#include "BDSGlobalConstants.hh" // must be first in include list

#include "mySectorBend.hh"
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

#include <map>

//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;
extern G4RotationMatrix* RotY90;

//============================================================

mySectorBend::mySectorBend(G4String aName,G4double aLength, 
			     G4double bpRad,G4double FeRad,
			     G4double bField, G4double angle,
			     G4double tilt,  G4double bGrad, G4int nSegments):
  BDSMultipole(aName,aLength, bpRad, FeRad,SetVisAttributes(),0,0,angle)
{
  itsTilt = tilt;
  itsBField = bField;
  itsBGrad = bGrad;

  itsType="sbend";

  if (!(*LogVolCount)[itsName])
    {
      BuildSBMarkerLogicalVolume();

      BuildBeampipe2(this,itsLength,itsAngle);

      BuildBPFieldAndStepper();

      BuildBPFieldMgr(itsStepper,itsMagField);

      //SetSensitiveVolume(itsBeampipeLogicalVolume);// for synchrotron
      SetSensitiveVolume(itsOuterLogicalVolume);// otherwise

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

      (*LogVolCount)[itsName]=1;
      (*LogVol)[itsName]=itsMarkerLogicalVolume;
    }
  else
    {
      (*LogVolCount)[itsName]++;

	itsMarkerLogicalVolume=(*LogVol)[itsName];

    }


}


G4VisAttributes* mySectorBend::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0,1,1));
  return itsVisAttributes;
}


void mySectorBend::BuildBPFieldAndStepper()
{
  // set up the magnetic field and stepper
  G4ThreeVector Bfield(0.,-itsBField,0.);
  itsMagField=new BDSSbendMagField(Bfield,itsLength,itsAngle);
  
  
  itsEqRhs=new G4Mag_UsualEqRhs(itsMagField);  
  
  
  //itsStepper=new BDSHelixStepper(itsEqRhs);
  itsStepper = new myQuadStepper(itsEqRhs);
  itsStepper->SetBField(itsBField);
 
  itsStepper->SetBGrad(itsBGrad);

  BuildBPFieldMgr(itsStepper,itsMagField);

  itsBeampipeLogicalVolume->SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
  itsInnerBPLogicalVolume->SetFieldManager(itsBPFieldMgr,false);

}


void mySectorBend::BuildSBMarkerLogicalVolume()
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
//void mySectorBend::BuildBeampipe2(G4double length, G4double angle)
void BuildBeampipe2(mySectorBend *sb,G4double length, G4double angle)
{

 G4double LCComponentBoxSize=BDSGlobals->GetComponentBoxSize();
 
 G4double xHalfLengthMinus=(sb->itsLength/sb->itsAngle)*sin(sb->itsAngle/2)
    -fabs(cos(sb->itsAngle/2)) * LCComponentBoxSize*tan(sb->itsAngle/2)/2
    +BDSGlobals->GetLengthSafety()/2;

 G4double xHalfLengthPlus=(sb->itsLength/sb->itsAngle)*sin(sb->itsAngle/2)
    +fabs(cos(sb->itsAngle/2)) * LCComponentBoxSize*tan(sb->itsAngle/2)/2
    +BDSGlobals->GetLengthSafety()/2;



  //G4double halfLength = 0.5 * (xHalfLengthMinus + xHalfLengthPlus) ;
  

  // *** toroidal beampipes - problems with G4Torus::DistanceToTorus
  // *** when calling G4PolynomialSolver.Solve()

 // G4double rSwept = halfLength /  fabs( sin( angle / 2 ) );

//  G4Torus *pipeTube = new G4Torus(sb->itsName+"_pipe_outer",
// 				  sb->itsBpRadius-BDSGlobals->GetBeampipeThickness(),  // inner R
// 				  sb->itsBpRadius, // outer R
// 				  rSwept , // swept R
// 				  0, // starting phi
// 				  fabs(angle) ); // delta phi


//   G4Torus *pipeInner = new G4Torus(sb->itsName+"_pipe_inner",
// 				   0,   // inner R
// 				   sb->itsBpRadius-BDSGlobals->GetBeampipeThickness()-0.000*mm,// outer
// 				   rSwept , // swept R
// 				   0, // starting phi
// 				   fabs(angle) ); // delta phi
  
//   sb->itsBeampipeLogicalVolume=	
//     new G4LogicalVolume(pipeTube,
// 			theMaterials->LCAluminium,
// 			sb->itsName+"_bmp_logical");
  
//   sb->itsInnerBPLogicalVolume=	
//     new G4LogicalVolume(pipeInner,
// 			theMaterials->LCVacuum,
// 			sb->itsName+"_bmp_Inner_log");
  
//   G4RotationMatrix* Rot= new G4RotationMatrix;
  
//   Rot->rotateX(pi/2 * rad);
//   Rot->rotateZ(- ( pi/2 - fabs(angle)/2 ) * rad);

//   G4ThreeVector pipeTorusOrigin(0, 0, rSwept * fabs( cos(angle / 2) ) );

//   if(angle < 0)
//     {
//       //Rot->rotateY(pi);
//       Rot->rotateZ(pi);
//       pipeTorusOrigin.setZ(-rSwept * fabs( cos(angle / 2) ) );
//     }
  

//   G4VPhysicalVolume* PhysiInner = 
//     new G4PVPlacement(
// 		      Rot,		       // rotation
// 		      pipeTorusOrigin,	               // at (0,0,0)
// 		      sb->itsInnerBPLogicalVolume, // its logical volume
// 		      sb->itsName+"_InnerBmp",     // its name
// 		      sb->itsMarkerLogicalVolume, // its mother  volume
// 		      false,		       // no boolean operation
// 		      0);		       // copy numbe

//   G4VPhysicalVolume* PhysiComp = 
//     new G4PVPlacement(
// 		      Rot,			     // rotation
// 		      pipeTorusOrigin,	                     // at (0,0,0)
// 		      sb->itsBeampipeLogicalVolume,  // its logical volume
// 		      sb->itsName+"_bmp",	     // its name
// 		      sb->itsMarkerLogicalVolume,     // its mother  volume
// 		      false,		     // no boolean operation
// 		      0);		             // copy number
  
  
  // *** try with tubes 

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

  /* remember to uncomment this if using intersection solid below
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

  // remember to uncomment markerBox if using this
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
			theMaterials->LCAluminium,
			sb->itsName+"_bmp_logical");
  
  sb->itsInnerBPLogicalVolume=	
    new G4LogicalVolume(pipeInner,
			theMaterials->LCVacuum,
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


  sb->itsBeampipeUserLimits =
    new G4UserLimits("beampipe cuts",DBL_MAX,DBL_MAX,DBL_MAX,
  		     BDSGlobals->GetThresholdCutCharged());
  
  sb->itsInnerBeampipeUserLimits =
    new G4UserLimits("inner beamipe cuts",DBL_MAX,DBL_MAX,DBL_MAX,
  		     BDSGlobals->GetThresholdCutCharged());
  
  sb->itsBeampipeUserLimits->SetMaxAllowedStep(sb->itsLength);
  sb->itsBeampipeLogicalVolume->SetUserLimits(sb->itsBeampipeUserLimits);
  
  sb->itsInnerBeampipeUserLimits->SetMaxAllowedStep(sb->itsLength);
  sb->itsInnerBPLogicalVolume->SetUserLimits(sb->itsInnerBeampipeUserLimits);

  // zero field in the marker volume
  sb->itsMarkerLogicalVolume->
    SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
  
}

mySectorBend::~mySectorBend()
{
  
  delete itsVisAttributes;
  delete itsMarkerLogicalVolume;
  delete itsOuterLogicalVolume;
  delete itsPhysiComp;
  delete itsMagField;
  delete itsEqRhs;
  delete itsStepper;
}

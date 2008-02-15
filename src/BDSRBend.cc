#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSRBend.hh"
#include "G4Tubs.hh"
#include "G4IntersectionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"

#include <map>

const int DEBUG = 0;

//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;

extern G4RotationMatrix* RotY90;
extern G4RotationMatrix* RotYM90;

//============================================================

// NOTE: BDSRBend is constructed by passing the geometric length,
// but internally uses the arc length

BDSRBend::BDSRBend(G4String aName, G4double aLength, 
		   G4double bpRad, G4double FeRad,
		   G4double bField, G4double angle, G4double outR,
		   G4double tilt, G4double bGrad, 
		   G4String aMaterial, G4int nSegments):
  BDSMultipole(aName, aLength, bpRad, FeRad, SetVisAttributes(), aMaterial,
	       0, 0, angle)
{
  SetOuterRadius(outR);
  itsTilt=tilt;
  itsBField=bField;
  itsBGrad=bGrad;
  itsType="rbend";

  if (!(*LogVolCount)[itsName])
    {
      //
      // build external volume
      // 
      BuildRBMarkerLogicalVolume();

      //
      // build beampipe (geometry + magnetic field)
      //
      BuildBPFieldAndStepper();
      BuildBPFieldMgr(itsStepper,itsMagField);
      BuildRBBeampipe();

      //
      // build magnet (geometry + magnetic field)
      //
      BuildRBOuterLogicalVolume();
      if(BDSGlobals->GetIncludeIronMagFields())
	{
	  G4double polePos[4];
	  G4double Bfield[3];

	  //coordinate in GetFieldValue
	  polePos[0]=0.;
	  polePos[1]=BDSGlobals->GetMagnetPoleRadius();
	  polePos[2]=0.;
	  polePos[3]=-999.;//flag to use polePos rather than local track

      	  itsMagField->GetFieldValue(polePos,Bfield);
	  G4double BFldIron=
	  sqrt(Bfield[0]*Bfield[0]+Bfield[1]*Bfield[1])*
	  BDSGlobals->GetMagnetPoleSize()/
	  (BDSGlobals->GetComponentBoxSize()/2-
	  BDSGlobals->GetMagnetPoleRadius());

	  // Magnetic flux from a pole is divided in two directions
	  BFldIron/=2.;
	  
	  BuildOuterFieldManager(2, BFldIron,pi/2);
	}

      //
      // define sensitive volumes for hit generation
      //
      SetMultipleSensitiveVolumes(itsBeampipeLogicalVolume);
      SetMultipleSensitiveVolumes(itsOuterLogicalVolume);

      //
      // set visualization attributes
      //
      itsVisAttributes=SetVisAttributes();
      itsVisAttributes->SetForceSolid(true);
      itsOuterLogicalVolume->SetVisAttributes(itsVisAttributes);

      //
      // append marker logical volume to volume map
      //
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
	  // logical volumes, because they have different fields
	  itsName+=BDSGlobals->StringFromInt((*LogVolCount)[itsName]);

	  //
	  // build external volume
	  // 
	  BuildRBMarkerLogicalVolume();
	  
	  //
	  // build beampipe (geometry + magnetic field)
	  //
	  BuildBPFieldAndStepper();
	  BuildBPFieldMgr(itsStepper,itsMagField);
	  BuildRBBeampipe();

	  //
	  // build magnet (geometry + magnetic field)
	  //
	  BuildRBOuterLogicalVolume();
	  if(BDSGlobals->GetIncludeIronMagFields())
	    {
	      G4double polePos[4];
	      G4double Bfield[3];
	      
	      //coordinate in GetFieldValue
	      polePos[0]=0.;
	      polePos[1]=BDSGlobals->GetMagnetPoleRadius();
	      polePos[2]=0.;
	      polePos[3]=-999.;//flag to use polePos rather than local track
	      
	      itsMagField->GetFieldValue(polePos,Bfield);
	      G4double BFldIron=
		sqrt(Bfield[0]*Bfield[0]+Bfield[1]*Bfield[1])*
		BDSGlobals->GetMagnetPoleSize()/
		(BDSGlobals->GetComponentBoxSize()/2-
		 BDSGlobals->GetMagnetPoleRadius());
	      
	      // Magnetic flux from a pole is divided in two directions
	      BFldIron/=2.;

	      BuildOuterFieldManager(2, BFldIron,pi/2);
	    }
	  //When is SynchRescale(factor) called?
	  
	  //
	  // define sensitive volumes for hit generation
	  //
	  SetSensitiveVolume(itsBeampipeLogicalVolume);// for synchrotron
	  //SetSensitiveVolume(itsOuterLogicalVolume);// for laserwire

	  //
	  // set visualization attributes
	  //
	  itsVisAttributes=SetVisAttributes();
	  itsVisAttributes->SetForceSolid(true);
	  itsOuterLogicalVolume->SetVisAttributes(itsVisAttributes);

	  //
	  // append marker logical volume to volume map
	  //
	  (*LogVol)[itsName]=itsMarkerLogicalVolume;
	}
      else
	{
	  //
	  // use already defined marker volume
	  //
	  itsMarkerLogicalVolume=(*LogVol)[itsName];
	}      
    }
}

void BDSRBend::SynchRescale(G4double factor)
{
  // rescale B field and gradient by same factor
  itsStepper->SetBGrad(itsBGrad*factor);
  itsStepper->SetBField(-itsBField*factor);
  if(DEBUG) G4cout << "Rbend " << itsName << " has been scaled" << G4endl;
}

G4VisAttributes* BDSRBend::SetVisAttributes()
{
  itsVisAttributes = new G4VisAttributes(G4Colour(0,0,1));
  return itsVisAttributes;
}

void BDSRBend::BuildBPFieldAndStepper()
{
  // set up the magnetic field and stepper
  G4ThreeVector Bfield(0.,-itsBField,0.);
  itsMagField=new BDSSbendMagField(Bfield,GetArcLength(),itsAngle);

  itsEqRhs=new G4Mag_UsualEqRhs(itsMagField);  
  
  itsStepper = new myQuadStepper(itsEqRhs); // note the - sign...
  itsStepper->SetBField(-itsBField);
  itsStepper->SetBGrad(itsBGrad);
}

void BDSRBend::BuildRBMarkerLogicalVolume()
{
  //  if (markerSolidVolume==0) {

    G4double yHalfLen=std::max(itsOuterR,BDSGlobals->GetComponentBoxSize()/2.);
    //G4double zHalfLen=yHalfLen*fabs(cos(itsAngle/2));
    G4double zHalfLen=yHalfLen;

    // itsLength = geometrical length
    G4double xHalfLengthMinus = itsLength/2
      - zHalfLen*tan(itsAngle/2)
      + BDSGlobals->GetLengthSafety()/2;
    
    G4double xHalfLengthPlus = itsLength/2
      + zHalfLen*tan(itsAngle/2)
      + BDSGlobals->GetLengthSafety()/2;

    markerSolidVolume = new G4Trd(itsName+"_marker",
				  xHalfLengthPlus,     // x hlf lgth at +z
				  xHalfLengthMinus,    // x hlf lgth at -z
				  yHalfLen,            // y hlf lgth at +z
				  yHalfLen,            // y hlf lgth at -z
				  zHalfLen);           // z hlf len
    //  }

  G4String LocalLogicalName=itsName;
  
  itsMarkerLogicalVolume=    
    new G4LogicalVolume(markerSolidVolume,
			theMaterials->GetMaterial("Vacuum"),
			LocalLogicalName+"_marker");

  itsMarkerUserLimits = new G4UserLimits(DBL_MAX,DBL_MAX,DBL_MAX);
  itsMarkerUserLimits->SetMaxAllowedStep(GetArcLength());
  itsMarkerLogicalVolume->SetUserLimits(itsMarkerUserLimits);

  //
  // zero field in the marker volume
  //
  itsMarkerLogicalVolume->
    SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
}


// construct a beampipe for sector bend
void BDSRBend::BuildRBBeampipe()
{
  //
  // use default beampipe material
  //
  G4Material *material =  theMaterials->GetMaterial( BDSGlobals->GetPipeMaterialName());
  
  //
  // compute some geometrical parameters
  //
  G4double bpThickness = BDSGlobals->GetBeampipeThickness();
  G4double yHalfLen=std::max(itsOuterR,BDSGlobals->GetComponentBoxSize()/2.);
  //G4double zHalfLen=yHalfLen*fabs(cos(itsAngle/2));
  G4double zHalfLen=yHalfLen;

  // itsLength = geometrical length
  G4double xHalfLengthMinus = itsLength/2
    - zHalfLen*tan(itsAngle/2)
    + BDSGlobals->GetLengthSafety()/2;
    
  G4double xHalfLengthPlus = itsLength/2
    + zHalfLen*tan(itsAngle/2)
    + BDSGlobals->GetLengthSafety()/2;

  G4double tubLen = std::max(xHalfLengthPlus,xHalfLengthMinus);

  //
  // build beampipe
  //
  G4Tubs *pipeTubsEnv = new G4Tubs(itsName+"_pipe_outer_env",
				   itsBpRadius-bpThickness, // inner R
				   itsBpRadius,             // outer R
				   tubLen,                  // length
				   0,                       // starting phi
				   twopi * rad );           // delta phi
  
  G4Tubs *pipeInnerEnv = new G4Tubs(itsName+"_pipe_inner_env",
				    0,                       // inner R
				    itsBpRadius-bpThickness, // outer R
				    tubLen,                  // length
				    0,                       // starting phi
				    twopi * rad );           // delta phi

  G4IntersectionSolid *pipeTubs =
    new G4IntersectionSolid(itsName+"_pipe_outer",
			    pipeTubsEnv,
 			    markerSolidVolume,
			    RotYM90,
			    0);
  
  G4IntersectionSolid *pipeInner =
    new G4IntersectionSolid(itsName+"_pipe_inner",
			    pipeInnerEnv, 
 			    markerSolidVolume,
			    RotYM90,
			    0);

  itsBeampipeLogicalVolume=	
    new G4LogicalVolume(pipeTubs,
			material,
			itsName+"_bmp_logical");
  
  itsInnerBPLogicalVolume=	
    new G4LogicalVolume(pipeInner,
			theMaterials->GetMaterial("Vacuum"),
			itsName+"_bmp_Inner_log");

  G4VPhysicalVolume* PhysiInner;
  PhysiInner = 
    new G4PVPlacement(
		      RotY90,		       // rotation
		      0,	               // at (0,0,0)
		      itsInnerBPLogicalVolume, // its logical volume
		      itsName+"_InnerBmp",     // its name
		      itsMarkerLogicalVolume,  // its mother volume
		      false,		       // no booleanm operation
		      0);		       // copy number
  
  G4VPhysicalVolume* PhysiComp;
  PhysiComp =
    new G4PVPlacement(
		      RotY90,		        // rotation
		      0,	                // at (0,0,0)
		      itsBeampipeLogicalVolume, // its logical volume
		      itsName+"_bmp",	        // its name
		      itsMarkerLogicalVolume,   // its mother volume
		      false,		        // no boolean operation
		      0);		        // copy number
  
  //
  // set user limits for stepping, tracking and propagation in B field
  //
  itsBeampipeUserLimits =
    new G4UserLimits("beampipe cuts",DBL_MAX,DBL_MAX,DBL_MAX,
  		     BDSGlobals->GetThresholdCutCharged());
  itsBeampipeUserLimits->SetMaxAllowedStep(GetArcLength());
  itsBeampipeLogicalVolume->SetUserLimits(itsBeampipeUserLimits);
  
  itsInnerBeampipeUserLimits =
    new G4UserLimits("inner beampipe cuts",DBL_MAX,DBL_MAX,DBL_MAX,
  		     BDSGlobals->GetThresholdCutCharged());
  itsInnerBeampipeUserLimits->SetMaxAllowedStep(GetArcLength());
  itsInnerBPLogicalVolume->SetUserLimits(itsInnerBeampipeUserLimits);

  //
  // set magnetic field inside beampipe
  //
  itsBeampipeLogicalVolume->SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
  itsInnerBPLogicalVolume->SetFieldManager(itsBPFieldMgr,false);

  //
  // set visualization attributes
  //
  G4VisAttributes* VisAtt =  new G4VisAttributes(G4Colour(0., 0., 0));
  VisAtt->SetForceSolid(true);
  itsInnerBPLogicalVolume->SetVisAttributes(VisAtt);

  G4VisAttributes* VisAtt1 = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
  VisAtt1->SetForceSolid(true);
  itsBeampipeLogicalVolume->SetVisAttributes(VisAtt1);
}

void BDSRBend::BuildRBOuterLogicalVolume(G4bool OuterMaterialIsVacuum){

  G4Material* material;
  if(itsMaterial != "")
    material = theMaterials->GetMaterial(itsMaterial);
  else
    material = theMaterials->GetMaterial("Iron");

  G4double yHalfLen=std::max(itsOuterR,BDSGlobals->GetComponentBoxSize()/2.);
  //G4double zHalfLen=yHalfLen*fabs(cos(itsAngle/2));
  G4double zHalfLen=yHalfLen;

  // itsLength = geometrical length
  G4double xHalfLengthMinus = itsLength/2
    - zHalfLen*tan(itsAngle/2)
    + BDSGlobals->GetLengthSafety()/2;
    
  G4double xHalfLengthPlus = itsLength/2
    + zHalfLen*tan(itsAngle/2)
    + BDSGlobals->GetLengthSafety()/2;

  G4double tubLen = std::max(xHalfLengthPlus,xHalfLengthMinus);
  
  G4Tubs *magTubsEnv =
    new G4Tubs(itsName+"_solid_env",
	       itsInnerIronRadius, // inner R
	       itsOuterR,          // outer R
	       tubLen,             // length
	       0,                  // starting phi
	       twopi * rad );      // delta phi
  
  G4IntersectionSolid *magTubs =
    new G4IntersectionSolid(itsName+"_solid",
			    magTubsEnv,
			    markerSolidVolume,
			    RotYM90,
			    0); 

  if(OuterMaterialIsVacuum)
    {
      itsOuterLogicalVolume = 
	new G4LogicalVolume(magTubs,
			    theMaterials->GetMaterial("Vacuum"),
			    itsName+"_outer");
    }
  else
    {
      itsOuterLogicalVolume=
	new G4LogicalVolume(magTubs,
			    material,
			    itsName+"_outer");
    }

  itsPhysiComp =
    new G4PVPlacement(
                      RotY90,                 // rotation
                      0,                      // at (0,0,0)
                      itsOuterLogicalVolume,  // its logical volume
                      itsName+"_solid",       // its name
                      itsMarkerLogicalVolume, // its mother  volume
                      false,                  // no boolean operation
                      0);                     // copy number

  itsOuterUserLimits =
    new G4UserLimits("multipole cut",DBL_MAX,DBL_MAX,DBL_MAX,
                     BDSGlobals->GetThresholdCutCharged());
  itsOuterUserLimits->SetMaxAllowedStep(GetArcLength());
  itsOuterLogicalVolume->SetUserLimits(itsOuterUserLimits);
}

BDSRBend::~BDSRBend()
{
  delete itsVisAttributes;
  delete itsMarkerLogicalVolume;
  delete itsOuterLogicalVolume;
  delete itsPhysiComp;
  delete itsMagField;
  delete itsEqRhs;
  delete itsStepper;
  if (markerSolidVolume) delete markerSolidVolume;
}

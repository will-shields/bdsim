#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSSectorBend.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4IntersectionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"

#include <map>

//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;

extern G4RotationMatrix* RotY90;
extern G4RotationMatrix* RotYM90;

//============================================================

BDSSectorBend::BDSSectorBend(G4String aName, G4double aLength, 
			     G4double bpRad, G4double FeRad,
			     G4double bField, G4double angle, G4double outR,
                             std::list<G4double> blmLocZ, std::list<G4double> blmLocTheta,
			     G4double tilt, G4double bGrad, 
			     G4String aTunnelMaterial, G4String aMaterial, G4double aXAper, G4double aYAper):
  BDSMultipole(aName, aLength, bpRad, FeRad, SetVisAttributes(), blmLocZ, blmLocTheta, aTunnelMaterial, aMaterial,
	       aXAper, aYAper, angle)
{
  SetOuterRadius(outR);
  itsTilt=tilt;
  itsBField=bField;
  itsBGrad=bGrad;
  itsType="sbend";

  if (!(*LogVolCount)[itsName])
    {
      //
      // build external volume
      // 
      BuildSBMarkerLogicalVolume();
      G4VisAttributes* VisAtt1 = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
      VisAtt1->SetVisibility(true);
      VisAtt1->SetForceSolid(true);
      itsMarkerLogicalVolume->SetVisAttributes(VisAtt1);
      //
      // build tunnel
      //
//       G4double oldTunnelRad=      BDSGlobals->GetTunnelRadius();
         
      if(BDSGlobals->GetBuildTunnel()){
	BuildTunnel();  //Do not build tunnel as there is a geometry problem
      }
      
      //      BDSGlobals->SetTunnelRadius(oldTunnelRad);

      //Build IP8Gate
      //      BuildGate();
     
      //
      // build beampipe (geometry + magnetic field)
      //
      
	      BuildBPFieldAndStepper();
	      BuildBPFieldMgr(itsStepper,itsMagField);
	      BuildSBBeampipe();

      //
      // build magnet (geometry + magnetic field)
      //
      

      BuildSBOuterLogicalVolume();
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


      if(BDSGlobals->GetSensitiveBeamPipe()){
        SetMultipleSensitiveVolumes(itsBeampipeLogicalVolume);
      }
      if(BDSGlobals->GetSensitiveComponents()){
        SetMultipleSensitiveVolumes(itsOuterLogicalVolume);
      }


      //
      // set visualization attributes
      //
      itsVisAttributes=SetVisAttributes();
      
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
	  BuildSBMarkerLogicalVolume();
	  
	  //
	  // build beampipe (geometry + magnetic field)
	  //
	  BuildBPFieldAndStepper();
	  BuildBPFieldMgr(itsStepper,itsMagField);
	  BuildSBBeampipe();

	  //
	  // build magnet (geometry + magnetic field)
	  //
	  BuildSBOuterLogicalVolume();
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
	  itsVisAttributes=SetVisAttributes();
	  itsVisAttributes->SetForceSolid(true);
	}      
    }

  BuildBLMs();
}

void BDSSectorBend::SynchRescale(G4double factor)
{
  // rescale B field and gradient by same factor
  itsStepper->SetBGrad(itsBGrad*factor);
  itsStepper->SetBField(-itsBField*factor);
  // note that there are no methods to set the BDSSBendMagField as this
  // class does not do anything with the BFields.
  // not true when I will use Geant4 propagation
#ifdef DEBUG
  G4cout << "Sbend " << itsName << " has been scaled" << G4endl;
#endif
}

G4VisAttributes* BDSSectorBend::SetVisAttributes()
{
  itsVisAttributes = new G4VisAttributes(G4Colour(0,0,1)); //blue
  itsVisAttributes->SetForceSolid(true);
  itsVisAttributes->SetVisibility(true);
  return itsVisAttributes;
}

void BDSSectorBend::BuildBPFieldAndStepper()
{
  // set up the magnetic field and stepper
  G4ThreeVector Bfield(0.,-itsBField,0.);
  itsMagField=new BDSSbendMagField(Bfield,itsLength,itsAngle);

  itsEqRhs=new G4Mag_UsualEqRhs(itsMagField);  
  
  itsStepper = new myQuadStepper(itsEqRhs); // note the - sign...
  itsStepper->SetBField(-itsBField);
  itsStepper->SetBGrad(itsBGrad);
}

void BDSSectorBend::BuildSBMarkerLogicalVolume()
{
  G4double xLength, yLength;
  xLength = yLength = std::max(itsOuterR,BDSGlobals->GetComponentBoxSize()/2);
  xLength = std::max(xLength, this->GetTunnelRadius()+2*std::abs(this->GetTunnelOffsetX()) + BDSGlobals->GetTunnelThickness()+BDSGlobals->GetTunnelSoilThickness() + 4*BDSGlobals->GetLengthSafety() );   
  yLength = std::max(yLength, this->GetTunnelRadius()+2*std::abs(BDSGlobals->GetTunnelOffsetY()) + BDSGlobals->GetTunnelThickness()+BDSGlobals->GetTunnelSoilThickness()+4*BDSGlobals->GetLengthSafety() );

  G4double transverseSize=2*std::max(xLength, yLength);

#ifdef DEBUG 
  G4cout<<"marker volume : x/y="<<transverseSize/m<<
    " m, l= "<<  (itsLength)/2/m <<" m"<<G4endl;
#endif

    G4double xHalfLengthPlus, xHalfLengthMinus;
    if(fabs(itsAngle) > 1e-20){
      xHalfLengthMinus = (itsLength/itsAngle)*sin(itsAngle/2)
        - fabs(cos(itsAngle/2))*transverseSize*tan(itsAngle/2)/2;
      
      xHalfLengthPlus = (itsLength/itsAngle)*sin(itsAngle/2)
        + fabs(cos(itsAngle/2))*transverseSize*tan(itsAngle/2)/2;
    } else {
      xHalfLengthPlus=(itsLength)/2.0;
      xHalfLengthMinus=(itsLength)/2.0;
    }
    
    if((xHalfLengthPlus<0) || (xHalfLengthMinus<0)){
      G4cerr << "Bend radius in " << itsName << " too small for this tunnel/component geometry. Exiting." << G4endl;
      exit(1);
    }

    itsMarkerSolidVolume = new G4Trd(itsName+"_marker",
				  xHalfLengthPlus,     // x hlf lgth at +z
				  xHalfLengthMinus,    // x hlf lgth at -z
				  transverseSize/2,           // y hlf lgth at +z
				  transverseSize/2,           // y hlf lgth at -z
				     fabs(cos(itsAngle/2))*transverseSize/2);// z hlf lgth

  G4String LocalLogicalName=itsName;
  
  itsMarkerLogicalVolume=    
    new G4LogicalVolume(itsMarkerSolidVolume,
			theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()),
			LocalLogicalName+"_marker");

  itsMarkerUserLimits = new G4UserLimits(DBL_MAX,DBL_MAX,DBL_MAX, BDSGlobals->GetThresholdCutCharged());
G4double  maxStepFactor=1e-3;
  itsMarkerUserLimits->SetMaxAllowedStep(itsLength*maxStepFactor);
  itsMarkerLogicalVolume->SetUserLimits(itsMarkerUserLimits);
  

  //
  // zero field in the marker volume
  //
  itsMarkerLogicalVolume->
    SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
}


// construct a beampipe for sector bend
void BDSSectorBend::BuildSBBeampipe()
{
  //
  // use default beampipe material
  //
    G4Material *material =  theMaterials->GetMaterial( BDSGlobals->GetPipeMaterialName());
    //  G4Material *material =  theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()); //LD 8/11/10 
  
  //
  // compute some geometrical parameters
  //
  G4double boxSize = BDSGlobals->GetComponentBoxSize();

  G4double xHalfLengthPlus, xHalfLengthMinus, tubLen;
  if(itsAngle != 0){
    xHalfLengthMinus =
      (itsLength/itsAngle)*sin(itsAngle/2)
      - fabs(cos(itsAngle/2)) * boxSize * tan(itsAngle/2)/2;
    
    xHalfLengthPlus =
      (itsLength/itsAngle)*sin(itsAngle/2)
      + fabs(cos(itsAngle/2)) * boxSize * tan(itsAngle/2)/2;
    tubLen = std::max(xHalfLengthPlus,xHalfLengthMinus);
  } else {
    tubLen=(itsLength)/2.0;
  }

  

  //
  // build beampipe
  //
  G4VSolid *pipeTubsEnv = new G4SubtractionSolid("_pipe_outer_env",
                                               new G4EllipticalTube(itsName+"_pipe_outer_tmp_1",
                                                                    this->GetAperX()+BDSGlobals->GetBeampipeThickness(), 
                                                                    this->GetAperY()+BDSGlobals->GetBeampipeThickness(),          
                                                                    tubLen),
                                               new G4EllipticalTube(itsName+"_pipe_outer_tmp_2",
                                                                    this->GetAperX()+BDSGlobals->GetLengthSafety()/2.0, 
                                                                    this->GetAperY()+BDSGlobals->GetLengthSafety()/2.0,          
                                                                    tubLen*2)
                                               );



  
  G4VSolid *pipeInnerEnv = new G4EllipticalTube(itsName+"_pipe_outer_tmp_2",
                                                this->GetAperX(), 
                                                this->GetAperY(),          
                                                tubLen);

  G4IntersectionSolid *pipeTubs =
    new G4IntersectionSolid(itsName+"_pipe_outer",
			    pipeTubsEnv,
 			    itsMarkerSolidVolume,
			    RotYM90,
			    (G4ThreeVector)0);
  
  G4IntersectionSolid *pipeInner =
    new G4IntersectionSolid(itsName+"_pipe_inner",
			    pipeInnerEnv, 
 			    itsMarkerSolidVolume,
			    RotYM90,
			    (G4ThreeVector)0);

  itsBeampipeLogicalVolume=	
    new G4LogicalVolume(pipeTubs,
			material,
			itsName+"_bmp_logical");
  
  itsInnerBPLogicalVolume=	
    new G4LogicalVolume(pipeInner,
			theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()),
			itsName+"_bmp_Inner_log");

  G4VPhysicalVolume* PhysiInner;
  PhysiInner = 
    new G4PVPlacement(
		      RotY90,		       // rotation
		      (G4ThreeVector)0,	               // at (0,0,0)
		      itsInnerBPLogicalVolume, // its logical volume
		      itsName+"_InnerBmp",     // its name
		      itsMarkerLogicalVolume,  // its mother volume
		      false,		       // no booleanm operation
		      0, BDSGlobals->GetCheckOverlaps());		       // copy number
  
  SetMultiplePhysicalVolumes(PhysiInner);

  G4VPhysicalVolume* PhysiComp;
  PhysiComp =
    new G4PVPlacement(
		      RotY90,		        // rotation
		      (G4ThreeVector)0,	                // at (0,0,0)
		      itsBeampipeLogicalVolume, // its logical volume
		      itsName+"_bmp",	        // its name
		      itsMarkerLogicalVolume,   // its mother volume
		      false,		        // no boolean operation
		      0, BDSGlobals->GetCheckOverlaps());		        // copy number
  
  SetMultiplePhysicalVolumes(PhysiComp);

  //
  // set user limits for stepping, tracking and propagation in B field
  //
  itsBeampipeUserLimits =
    new G4UserLimits("beampipe cuts",DBL_MAX,DBL_MAX,DBL_MAX,
  		     BDSGlobals->GetThresholdCutCharged());
  G4double maxStepFactor=0.5;
  itsBeampipeUserLimits->SetMaxAllowedStep(itsLength*maxStepFactor);
  itsBeampipeLogicalVolume->SetUserLimits(itsBeampipeUserLimits);
  
  G4double maxStepFactorIn=1e-3;
  itsInnerBeampipeUserLimits =
    new G4UserLimits("inner beampipe cuts",DBL_MAX,DBL_MAX,DBL_MAX,
  		     BDSGlobals->GetThresholdCutCharged());
  itsInnerBeampipeUserLimits->SetMaxAllowedStep(itsLength*maxStepFactorIn);
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
  VisAtt1->SetVisibility(true);
  VisAtt1->SetForceSolid(true);
  itsBeampipeLogicalVolume->SetVisAttributes(VisAtt1);
}


G4double BDSSectorBend::GetArcLength()
{
  // arc length = radius*angle
  //            = (geometrical length/(2.0*sin(angle/2))*angle
  if (itsAngle == 0.0)
    return itsLength;
  else
    return (itsLength * (0.5*itsAngle) / sin(0.5*itsAngle));
}

void BDSSectorBend::BuildSBOuterLogicalVolume(G4bool OuterMaterialIsVacuum){

  G4Material* material;
  
  if(itsMaterial != "")
    material = theMaterials->GetMaterial(itsMaterial);
  else
    material = theMaterials->GetMaterial("Iron");
  
  G4double xHalfLengthPlus, xHalfLengthMinus, tubLen;
  if(itsAngle != 0){
    xHalfLengthMinus = (itsLength/itsAngle)*sin(itsAngle/2)
      - fabs(cos(itsAngle/2)) * BDSGlobals->GetComponentBoxSize() * tan(itsAngle/2)/2
      - BDSGlobals->GetLengthSafety()/2;

    xHalfLengthPlus = (itsLength/itsAngle)*sin(itsAngle/2)
    + fabs(cos(itsAngle/2)) * BDSGlobals->GetComponentBoxSize() * tan(itsAngle/2)/2
      - BDSGlobals->GetLengthSafety()/2;
    
    tubLen = std::max(xHalfLengthPlus,xHalfLengthMinus);
  } else {
    tubLen = (itsLength-BDSGlobals->GetLengthSafety())/2.0;
  }
  
  
  G4VSolid *magTubsEnv = new G4SubtractionSolid(itsName+"_solid_env",
                                                new G4Tubs(itsName+"_solid_tmp_1",
                                                           itsInnerIronRadius + BDSGlobals->GetLengthSafety()/2.0, // inner R + overlap safety
                                                           itsOuterR,          // outer R
                                                           tubLen,             // length
                                                           0,                  // starting phi
                                                           twopi * rad ),      // delta phi
                                                new G4EllipticalTube(itsName+"_pipe_outer_tmp_2",
                                                                     this->GetAperX()+BDSGlobals->GetBeampipeThickness()+BDSGlobals->GetLengthSafety()/2.0, 
                                                                     this->GetAperY()+BDSGlobals->GetBeampipeThickness()+BDSGlobals->GetLengthSafety()/2.0,          
                                                                     tubLen*2)
                                                );
  
  G4IntersectionSolid *magTubs =
    new G4IntersectionSolid(itsName+"_solid",
			    magTubsEnv,
			    itsMarkerSolidVolume,
			    RotYM90,
			    (G4ThreeVector)0); 

  if(OuterMaterialIsVacuum)
    {
      itsOuterLogicalVolume = 
	new G4LogicalVolume(magTubs,
			    theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()),
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
                      (G4ThreeVector)0,                      // at (0,0,0)
                      itsOuterLogicalVolume,  // its logical volume
                      itsName+"_solid",       // its name
                      itsMarkerLogicalVolume, // its mother  volume
                      false,                  // no boolean operation
                      0, BDSGlobals->GetCheckOverlaps());                     // copy number

SetMultiplePhysicalVolumes(itsPhysiComp);
 G4double  maxStepFactor=0.5;
  itsOuterUserLimits =
    new G4UserLimits("multipole cut",DBL_MAX,DBL_MAX,DBL_MAX,
                     BDSGlobals->GetThresholdCutCharged());
  itsOuterUserLimits->SetMaxAllowedStep(itsLength*maxStepFactor);
  itsOuterLogicalVolume->SetUserLimits(itsOuterUserLimits);
}

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

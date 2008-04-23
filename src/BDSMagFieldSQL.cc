/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 25.12.2003
   Copyright (c) 2003 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
const int DEBUG = 0;

#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSMagFieldSQL.hh"

#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistoryHandle.hh"
#include "G4TouchableHistory.hh"
#include "G4NavigationHistory.hh"
#include <string>
#include <vector>

using namespace std;

BDSMagFieldSQL::BDSMagFieldSQL(const G4String& aFieldFile,
			       G4double aMarkerLength,
			       vector<G4String> Quadvol, vector<G4double> QuadBgrad,
			       vector<G4String> Sextvol, vector<G4double> SextBgrad,
			       vector<G4String> Octvol, vector<G4double> OctBgrad,
			       vector<G4String> Fieldvol, vector<G4ThreeVector> UniformField)

  :ifs(aFieldFile.c_str()),itsMarkerLength(aMarkerLength),FieldFile(aFieldFile),
   itsQuadBgrad(QuadBgrad), itsQuadVol(Quadvol),
   itsSextBgrad(SextBgrad), itsSextVol(Sextvol),
   itsOctBgrad(OctBgrad), itsOctVol(Octvol),
   itsUniformField(UniformField), itsFieldVol(Fieldvol)
{
}

BDSMagFieldSQL::~BDSMagFieldSQL(){}

void BDSMagFieldSQL::GetFieldValue( const G4double Point[4],
		       G4double *Bfield ) const
{
  
  G4Navigator* IRNavigator=
    G4TransportationManager::GetTransportationManager()-> 
    GetNavigatorForTracking();
  // gab_dec03>>
  G4ThreeVector LocalR, GlobalR, LocalB, RLocalR, QuadB, SextB, OctB, FieldB;
  LocalR = GlobalR = LocalB = RLocalR = QuadB = SextB = OctB = FieldB= G4ThreeVector(0.,0.,0.);
  
  GlobalR.setX(Point[0]);
  GlobalR.setY(Point[1]);
  GlobalR.setZ(Point[2]);

  G4TouchableHistory* aTouchable = IRNavigator->CreateTouchableHistory();
  
  const G4NavigationHistory* itsHistory = aTouchable->GetHistory();

  const G4AffineTransform GlobalToMarker=itsHistory->GetTransform(1);
  const G4AffineTransform MarkerToGlobal=GlobalToMarker.Inverse();
  
  RLocalR=GlobalToMarker.TransformPoint(GlobalR);
  
  if( fabs(RLocalR.z()) > fabs(itsMarkerLength/2) )
    {
      // Outside of mokka region - field should be zero. This is needed
      // because sometimes RKStepper asks for overly large steps (1km)
      Bfield[0] = 0;
      Bfield[1] = 0;
      Bfield[2] = 0;
      Bfield[3] = 0;
      Bfield[4] = 0;
      Bfield[5] = 0;
      return;
      
    }

  G4AffineTransform GlobalAffine, LocalAffine;
  G4String VolName = aTouchable->GetVolume()->GetName();
  G4bool inQuad=false;
  G4bool inSext=false;
  G4bool inOct = false;
  G4bool inField=false;

  for(G4int i=0; i<(G4int)itsQuadVol.size(); i++)
    {
      if(VolName==itsQuadVol[i])
	{
	  GlobalAffine=IRNavigator->GetGlobalToLocalTransform();
	  LocalAffine=IRNavigator->GetLocalToGlobalTransform();
	  LocalR=GlobalAffine.TransformPoint(GlobalR); 
	  LocalR.setY(-LocalR.y());
	  LocalR.setX(-LocalR.x());	  // -ve signs because of Geant Co-ord System
	  QuadB.setX(LocalR.y()*itsQuadBgrad[i]);
	  QuadB.setY(LocalR.x()*itsQuadBgrad[i]);
	  QuadB.setZ(0);
	  QuadB = LocalAffine.TransformAxis(QuadB);
	  inQuad=true;
	  break;
	}
    }
  if(!inQuad) //i.e. if no quad found in previous check ^^^^^
    {
      for(G4int i=0; i<(G4int)itsSextVol.size(); i++)
	{
	  if(VolName==itsSextVol[i])
	    {
	      GlobalAffine=IRNavigator->GetGlobalToLocalTransform();
	      LocalAffine=IRNavigator->GetLocalToGlobalTransform();
	      LocalR=GlobalAffine.TransformPoint(GlobalR); 
	      LocalR.setY(-LocalR.y());
	      LocalR.setX(-LocalR.x());	  // -ve signs because of Geant Co-ord System
	      SextB.setX(LocalR.x()*LocalR.y()*itsSextBgrad[i]);
	      SextB.setY(-(LocalR.x()*LocalR.x()-LocalR.y()*LocalR.y())*itsSextBgrad[i]/2.);
	      SextB.setZ(0.0);
	      SextB = LocalAffine.TransformAxis(SextB);
	      inSext=true;
	      break;
	    }
	}
    }
  
  if(!inSext && !inQuad)
    {
      for(G4int i=0; i<(G4int)itsOctVol.size(); i++)
	{
	  if(VolName==itsOctVol[i])
	    {
	      GlobalAffine=IRNavigator->GetGlobalToLocalTransform();
	      LocalAffine=IRNavigator->GetLocalToGlobalTransform();
	      LocalR=GlobalAffine.TransformPoint(GlobalR); 
	      LocalR.setY(-LocalR.y());
	      LocalR.setX(-LocalR.x());	  // -ve signs because of Geant Co-ord System
	      OctB.setX( (3*LocalR.x()*LocalR.x()*LocalR.y() - 
			  LocalR.y()*LocalR.y()*LocalR.y())*itsOctBgrad[i]/6.);
	      OctB.setY( (LocalR.x()*LocalR.x()*LocalR.x() -
			  LocalR.x()*LocalR.y()*LocalR.y())*itsOctBgrad[i]/6.);
	      OctB.setZ(0.0);
	      OctB = LocalAffine.TransformAxis(OctB);
	      inOct=true;
		  break;
	    }
	}
    }
    
  
  for(G4int i=0; i<(G4int)itsFieldVol.size(); i++)
    {
      if(VolName==itsFieldVol[i])
	{
	  GlobalAffine=IRNavigator->GetGlobalToLocalTransform();
	  LocalAffine=IRNavigator->GetLocalToGlobalTransform();
	  LocalR=GlobalAffine.TransformPoint(GlobalR); 
	  LocalR.setY(-LocalR.y());
	  LocalR.setX(-LocalR.x());	  // -ve signs because of Geant Co-ord System
	  FieldB = itsUniformField[i];
	  FieldB = LocalAffine.TransformAxis(FieldB);
	  inField=true;
	  break;
	}
    }

  if(itsMarkerLength>0) RLocalR.setZ(RLocalR.z()+itsMarkerLength/2);
  else RLocalR.setZ( -(RLocalR.z()+fabs(itsMarkerLength)/2) + fabs(itsMarkerLength));
  G4double tempz = RLocalR.z()/cm;
  if(tempz<0)  //Mokka region resets Z to be positive at starting from one
               //Edge of the region
    {
      // This should NEVER happen. If it does, then the cause is either that
      // the mokka region length is not set properly, or that the BDSRKStepper
      // is asking for a step length greater than the Mokka marker length
      G4cout << "Z position in Mokka region less than 0 - check step lengths!!" << G4endl;
      G4Exception("Quitting BDSIM in BDSMagFieldSQL.cc");
    }
  G4double zlow = floor(tempz);
  G4int ilow = (G4int)(zlow);
  G4double zhi = zlow + 1.0;
  if (ilow > (G4int)itsBz.size() ||
      itsBz.size()==0) LocalB = G4ThreeVector(0.,0.,0.);
  else
    {
      // Calculate the field local to MarkerVolume
      // Interpolate the value of the field nearest to the point
      G4double fieldBrr_r = ( (zhi-tempz)*itsBr_over_r[ilow] +
			      (tempz-zlow)*itsBr_over_r[ilow+1]);
      // then should divide by (zhi-zlow) but this = 1
      
      G4double fieldBzz = ( (zhi-tempz)*itsBz[ilow] +
			    (tempz-zlow)*itsBz[ilow+1]);
      // then should divide by (zhi-zlow) but this = 1
      LocalB.setX(fieldBrr_r*(RLocalR.x()));
      LocalB.setY(fieldBrr_r*(RLocalR.y()));
      LocalB.setZ(fieldBzz);
      // Now rotate to give BField on Global Reference Frame
      LocalB.transform(rotation.inverse());
    }
  //LocalB=G4ThreeVector(0.,0.,0.); //turn Bfield from Solenoid off
  if(inQuad) LocalB+=QuadB;
  if(inSext) LocalB+=SextB;
  if(inOct) LocalB+=OctB;
  if(inField) LocalB+=FieldB;

  // b-field
  Bfield[0] = LocalB.x();
  Bfield[1] = LocalB.y();
  Bfield[2] = LocalB.z();
  // e-field
  Bfield[3] = 0;
  Bfield[4] = 0;
  Bfield[5] = 0;


  /*
  G4cout << "BField: " << LocalB << G4endl;
  G4cout << itsMarkerLength << G4endl;
  G4cout << RLocalR << G4endl;
  G4cout << ilow << G4endl;
  G4cout << QuadB << G4endl;
  G4cout << SextB << G4endl;
  G4cout << OctB << G4endl;
  G4cout << G4endl;
  */
  
  
  if(DEBUG) 
    {
      LocalB.rotateY(10e-3); //to track from incoming beamline perspective
      // this needs the be the crossing angle plus any marker rotation applied
      // for IR solenoid case
      G4cout << RLocalR.x()/m << " "<<RLocalR.y()/m << " "<<RLocalR.z()/m << " "<< LocalB.x()/tesla << " " << LocalB.y()/tesla << " " << LocalB.z()/tesla << G4endl;
    }
  delete aTouchable;
  aTouchable = NULL;

}

void BDSMagFieldSQL::Prepare(G4VPhysicalVolume *referenceVolume)
{
  if(FieldFile!="")
    {
      G4cout<<"BDSElement:: creating SQL field map"<<G4endl;

      if(!ifs)
	{
	  G4cerr<<"\nBDSMagFieldSQL.cc: Unable to open Field Map File: " << FieldFile << G4endl;
	  G4Exception("Aborting Program");
	}
      else
	G4cout << "Loading SQL Field Map file: " << FieldFile << G4endl;
      
      if(FieldFile.contains("inverse")) itsMarkerLength*=-1;
      double temp_z;
      double temp_Bz;
      double temp_solB;
      while(!ifs.eof()){
	if(FieldFile.contains("SiD"))
	  ifs >> temp_z >> temp_Bz >> temp_solB;
	
	if(FieldFile.contains("LD"))
	  ifs >> temp_z >> temp_Bz >> temp_solB >> temp_solB;
	
	if(FieldFile.contains("TESLA"))
	  ifs >> temp_z >> temp_Bz;
	
	itsZ.push_back(temp_z*m);
	itsBz.push_back(temp_Bz*tesla);
      }
      
      itsdz = itsZ[1] - itsZ[0];
      
      //first element:
      itsdBz_by_dz.push_back( (itsBz[1] - itsBz[0]) / itsdz );
      itsBr_over_r.push_back(0.5 * itsdBz_by_dz[0] );
      
      for(G4int j=1; j<(G4int)itsBz.size()-2; j++)
	{
	  itsdBz_by_dz.push_back( (itsBz[j+1] - itsBz[j-1]) / (2*itsdz) );
	  itsBr_over_r.push_back(0.5 * itsdBz_by_dz[j] );
	}
      
      //last element:
      itsdBz_by_dz.push_back( (itsBz[itsBz.size()-1] - itsBz[itsBz.size()-2]) / itsdz );
      itsBr_over_r.push_back(0.5 * itsdBz_by_dz[itsdBz_by_dz.size()-1] );
    }
  
  SetOriginRotation(*(referenceVolume->GetFrameRotation()));
  SetOriginTranslation(referenceVolume->GetFrameTranslation());
}

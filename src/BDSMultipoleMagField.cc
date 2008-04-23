/** BDSIM, v0.4   

Last modified 11.10.2007 by Steve Malton

**/

//==============================================================



#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSMultipoleMagField.hh"

#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

ofstream testf;


BDSMultipoleMagField::BDSMultipoleMagField(list<G4double> knl, list<G4double> ksl)
{
  //G4cout<<"Creating BDSMultipleMagField"<<G4endl;
 
  //G4cout<<"size->"<<knl.size()<<G4endl;

  //G4double P0 = BDSGlobals->GetBeamTotalEnergy();
  
  // compute magnetic rigidity brho
  // formula: B(Tesla)*rho(m) = p(GeV)/(0.299792458 * |charge(e)|)
  G4double charge = BDSGlobals->GetParticleDefinition()->GetPDGCharge();
  G4double momentum = BDSGlobals->GetBeamMomentum();
  G4double brho = ( (momentum/GeV) / (0.299792458 * fabs(charge))) * (tesla*m);

  bnl = knl;
  bsl = ksl;

  list<G4double>::iterator it;
  list<G4double>::iterator its;
  
  for(it=bnl.begin(), its=bsl.begin();it!=bnl.end();it++, its++)
    {
      //G4cout<<"was : "<<(*it)<<G4endl;
      (*it) *= (brho*tesla );
      //G4cout<<"is : "<<(*it)<<G4endl;
      (*its) *= (brho*tesla );
    }

  testf.open("field.txt");

  G4double pt[4];
  G4double b[3];

  for(G4double x=-1*cm;x<1*cm;x+=0.1*mm)
    for(G4double y=-1*cm;y<1*cm;y+=0.1*mm){
      pt[0]= x;
      pt[1]= y;
      pt[2]= pt[3]=0;
      GetFieldValue(pt,b);
      testf<<pt[0]<<" "<<pt[1]<<" "<<b[0]/tesla<<" "<<b[1]/tesla<<" "<<b[2]/tesla<<G4endl;
    }


}

BDSMultipoleMagField::~BDSMultipoleMagField(){}

void BDSMultipoleMagField::GetFieldValue( const G4double Point[4],
		       G4double *Bfield ) const
{
  G4Navigator* MulNavigator=
    G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  
  G4ThreeVector LocalR, GlobalR;
  
  GlobalR.setX(Point[0]);
  GlobalR.setY(Point[1]);
  GlobalR.setZ(Point[2]);
  //LocalR = QuadNavigator->GetCurrentLocalCoordinate();
  G4AffineTransform GlobalAffine=MulNavigator->GetGlobalToLocalTransform();
  LocalR=GlobalAffine.TransformPoint(GlobalR); 
 

  Bfield[0]=0;
  Bfield[1]=0;


  G4double br = 0;
  G4double bphi = 0;
  

  list<G4double>::const_iterator it;
  list<G4double>::const_iterator its;

  //G4cout<<"Called GetFieldValue "<<Point[0]<<" "<<Point[1]<<" "<<Point[2]<<G4endl;
  //G4cout<<"Called GetFieldValue "<<LocalR[0]<<" "<<LocalR[1]<<" "<<LocalR[2]<<//"br="G4endl;

  G4double r = sqrt(LocalR[0]*LocalR[0] + LocalR[1]*LocalR[1]);
  G4double phi;
  if(fabs(r)>1.e-11*m) phi = asin(LocalR[1]/r);
  else phi = 0; // don't care
					   

  //G4cout<<"Called GetFieldValue "<<LocalR[0]<<" "<<LocalR[1]<<" "<<LocalR[2]<<
  // " r="<<r<<" Phi="<<phi<<G4endl;

  G4int order=0;
  G4double fact = -1;


  // I want to use the strange convention of dipole coeff. with opposite sign - 
  // then it is the same sign as angle 

  for(it=bnl.begin(), its=bsl.begin();it!=bnl.end();it++, its++)
    {
      order++;
      br += (*it) * pow(r,order-1) * sin(order*phi) / fact;
      bphi += (*it) * pow(r,order-1) * cos(order*phi)/ fact;

      if(order==1) {br *= -1; bphi *=-1; }

      fact *= (order*m);
    }
  
  //G4cout<<"order="<<order<<
  //  " br="<<br<<" bphi="<<bphi<<G4endl;


  Bfield[0]=( br*cos(phi)-bphi*sin(phi) );
  Bfield[1]=( br*sin(phi)+bphi*cos(phi) );
  

  Bfield[2]=0;

  Bfield[3]=0;
  Bfield[4]=0;
  Bfield[5]=0;

  // factor of 2 is actually 2-factorial.
}




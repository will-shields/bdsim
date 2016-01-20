#include "BDSDebug.hh"
#include "BDSIntegratorOctupole.hh"
#include "BDSMagnetStrength.hh"

#include "G4AffineTransform.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ThreeVector.hh"

extern G4double BDSLocalRadiusOfCurvature;

BDSIntegratorOctupole::BDSIntegratorOctupole(const BDSMagnetStrength* strength,
					     const G4double           brho,
					     G4Mag_EqRhs* const       eqRHSIn):
  BDSIntegratorBase(eqRHSIn, 6),
  bTriplePrime(0.0),
  yInitial(0), yMidPoint(0), yFinal(0)
{
  // B''' = d^3By/dx^3 = Brho * (1/Brho d^3By/dx^3) = Brho * k3
  bTriplePrime = brho * (*strength)["k3"];
#ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << "B''' = " << bTriplePrime << G4endl;
#endif
}

void BDSIntegratorOctupole::AdvanceHelix(const G4double  yIn[],
					 G4ThreeVector /*bField*/,
					 G4double  h,
					 G4double  yOct[])
{
  const G4double *pIn = yIn+3;
  G4ThreeVector v0 = G4ThreeVector(pIn[0], pIn[1], pIn[2]);  
  G4ThreeVector InitMomDir = v0.unit();

  G4ThreeVector GlobalPosition = G4ThreeVector(yIn[0], yIn[1], yIn[2]);  
  G4double InitMag = v0.mag();
  G4double kappa   = -eqRHS->FCof()*bTriplePrime/InitMag;

  // relevant momentum scale is p_z, not P_tot:
  // check that the approximations are valid, else do a linear step:
  if(fabs(kappa)<1.e-20)
    {
      G4ThreeVector positionMove  = (h/InitMag) * v0;
      
      yOct[0]   = yIn[0] + positionMove.x(); 
      yOct[1]   = yIn[1] + positionMove.y(); 
      yOct[2]   = yIn[2] + positionMove.z(); 
				
      yOct[3] = v0.x();
      yOct[4] = v0.y();
      yOct[5] = v0.z();

      distChord=0;
    }
  else 
    {
      G4AffineTransform LocalAffine  = auxNavigator->GetLocalToGlobalTransform();
      G4AffineTransform GlobalAffine = auxNavigator->GetGlobalToLocalTransform();
      G4ThreeVector LocalR=GlobalAffine.TransformPoint(GlobalPosition); 
      G4ThreeVector LocalRp=GlobalAffine.TransformAxis(InitMomDir);
      
      G4double x0=LocalR.x(); 
      G4double y0=LocalR.y();
      G4double z0=LocalR.z();

      //G4double x02My02=(x0*x0-y0*y0);

      G4double xp=LocalRp.x();
      G4double yp=LocalRp.y();
      G4double zp=LocalRp.z();

      G4double y3fac=y0*(y0*y0-3*x0*x0);
      G4double x3fac=x0*(x0*x0-3*y0*y0);
      
      // local r'' (for curvature)
      G4ThreeVector LocalRpp;
      // extra minus signs were because x,y_machine = - x_,-y_geant_world
      // New CVS version of BDSIM uses +x, +y in geant world
      /*
      LocalRpp.setX(zp*x3fac);
      LocalRpp.setY(zp*y3fac);
      LocalRpp.setZ(- xp*x3fac - yp*y3fac);
      */
      LocalRpp.setX(-zp*x3fac);
      LocalRpp.setY(-zp*y3fac);
      LocalRpp.setZ( xp*x3fac + yp*y3fac);

      LocalRpp*=kappa/6; // 6 is actually a 3! factor.;

      // determine effective curvature
      G4double R_1 = LocalRpp.mag();
      if(R_1>0.)
	{
	  // Save for Synchrotron Radiation calculations:
	  BDSLocalRadiusOfCurvature=1/R_1;

	  // chord distance (simple quadratic approx)
          G4double h2=h*h;
	  distChord= h2*R_1/8;

	  G4double dx=LocalRp.x()*h + LocalRpp.x()*h2/2; 
	  G4double dy=LocalRp.y()*h + LocalRpp.y()*h2/2; 

	  G4double dz=sqrt(h2*(1.-h2*R_1*R_1/12)-dx*dx-dy*dy);
	  // check for precision problems
	  G4double ScaleFac=(dx*dx+dy*dy+dz*dz)/h2;
	  if(ScaleFac>1.0000001)
	    {
	      ScaleFac=sqrt(ScaleFac);
	      dx/=ScaleFac;
	      dy/=ScaleFac;
	      dz/=ScaleFac;
	    }

	  LocalR.setX(x0+dx);
	  LocalR.setY(y0+dy);
	  LocalR.setZ(z0+dz);

	  LocalRp = LocalRp+ h*LocalRpp;
	}
      else
	{LocalR += h*LocalRp;}
       
      GlobalPosition=LocalAffine.TransformPoint(LocalR); 
      G4ThreeVector GlobalTangent=LocalAffine.TransformAxis(LocalRp)*InitMag;
      
      yOct[0]   = GlobalPosition.x(); 
      yOct[1]   = GlobalPosition.y(); 
      yOct[2]   = GlobalPosition.z(); 
				
      yOct[3] = GlobalTangent.x();
      yOct[4] = GlobalTangent.y();
      yOct[5] = GlobalTangent.z();
    }
}

void BDSIntegratorOctupole::Stepper(const G4double yInput[],
				    const G4double[],
				    const G4double hstep,
				    G4double yOut[],
				    G4double yErr[])
{  
  //const G4double *pIn = yInput+3;
  //G4ThreeVector v0= G4ThreeVector( pIn[0], pIn[1], pIn[2]);  
  //G4double InitMag=v0.mag();
  //G4double kappa=  -fPtrMagEqOfMot->FCof()*bTriplePrime/InitMag;
  
  G4double yTemp[7], yIn[7];
  
  //  Saving yInput because yInput and yOut can be aliases for same array
  
  for(G4int i = 0; i < nVariables; i++)
    {yIn[i] = yInput[i];}
  
  G4double h = hstep * 0.5; 
  
  // Do two half steps
  AdvanceHelix(yIn,   (G4ThreeVector)0, h, yTemp);
  AdvanceHelix(yTemp, (G4ThreeVector)0, h, yOut); 
  
  // Do a full Step
  h = hstep ;
  AdvanceHelix(yIn, (G4ThreeVector)0, h, yTemp); 
  
  for(G4int i = 0; i < nVariables; i++)
    {
      yErr[i] = yOut[i] - yTemp[i];
      // if error small, set error to 0
      // this is done to prevent Geant4 going to smaller and smaller steps
      // ideally use some of the global constants instead of hardcoding here
      // could look at step size as well instead.
      if (std::abs(yErr[i]) < 1e-7)
	{yErr[i] = 0;}
    }
}

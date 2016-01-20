#include "BDSDebug.hh"
#include "BDSIntegratorDecapole.hh"

#include "G4AffineTransform.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ThreeVector.hh"

extern G4double BDSLocalRadiusOfCurvature;

BDSIntegratorDecapole::BDSIntegratorDecapole(const BDSMagnetStrength* strength,
					     const G4double           brho,
					     G4Mag_EqRhs* const       eqRHSIn):
  BDSIntegratorBase(eqRHSIn, 6),
  yInitial(0), yMidPoint(0), yFinal(0)
{
  // B'''' = d^4By/dx^4 = Brho * (1/Brho d^4By/dx^4) = Brho * k4
  bQuadruplePrime = brho * (*strength)["k4"];
#ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << "B'''' = " << bQuadruplePrime << G4endl;
#endif
}

void BDSIntegratorDecapole::AdvanceHelix(const G4double  yIn[],
					 G4ThreeVector,
					 G4double  h,
					 G4double  yDec[])
{
  const G4double *pIn = yIn+3;
  G4ThreeVector v0 = G4ThreeVector( pIn[0], pIn[1], pIn[2]);  
  G4ThreeVector InitMomDir = v0.unit();

  G4ThreeVector GlobalPosition = G4ThreeVector(yIn[0], yIn[1], yIn[2]);  
  G4double InitMag = v0.mag();
  G4double kappa   = -eqRHS->FCof()*bQuadruplePrime/InitMag;

  // relevant momentum scale is p_z, not P_tot:
  // check that the approximations are valid, else do a linear step:
  if(fabs(kappa)<1.e-20)
    {
      G4ThreeVector positionMove  = (h/InitMag) * v0;
      
      yDec[0]   = yIn[0] + positionMove.x(); 
      yDec[1]   = yIn[1] + positionMove.y(); 
      yDec[2]   = yIn[2] + positionMove.z(); 
				
      yDec[3] = v0.x();
      yDec[4] = v0.y();
      yDec[5] = v0.z();

      distChord=0;
    }
  else 
    {
      G4ThreeVector LocalR  = ConvertToLocal(yIn);
      G4ThreeVector LocalRp = ConvertAxisToLocal(yIn, pIn);

      G4double x0=LocalR.x(); 
      G4double y0=LocalR.y();
    
      G4double x02My02=(x0*x0-y0*y0);

      G4double xp=LocalRp.x();
      G4double yp=LocalRp.y();
      G4double zp=LocalRp.z();

      G4double Bx=4.0*x0*y0*(x02My02);
      G4double By=pow(x0,4)-6.0*x0*x0*y0*y0+pow(y0,4);

      // local r'' (for curvature)
      G4ThreeVector LocalRpp;
      // extra minus signs because x,y_machine = - x_,-y_geant_world
      LocalRpp.setX( zp*By);
      LocalRpp.setY(-zp*Bx);
      LocalRpp.setZ( xp*By - yp*Bx);
      
      LocalRpp*=kappa/24; // 24 is actually a 4! factor.;

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

	  LocalR.setX(LocalR.x()+dx);
	  LocalR.setY(LocalR.y()+dy);
	  LocalR.setZ(LocalR.z()+dz);

	  LocalRp = LocalRp+ h*LocalRpp;
	}
      else
	{LocalR += h*LocalRp;}
      
      GlobalPosition = ConvertToGlobal(LocalR);
      G4ThreeVector GlobalTangent = ConvertAxisToGlobal(LocalRp) * InitMag;
      
      yDec[0] = GlobalPosition.x(); 
      yDec[1] = GlobalPosition.y(); 
      yDec[2] = GlobalPosition.z(); 				
      yDec[3] = GlobalTangent.x();
      yDec[4] = GlobalTangent.y();
      yDec[5] = GlobalTangent.z();
    }
}

void BDSIntegratorDecapole::Stepper(const G4double yInput[],
			    const G4double[],
			    const G4double hstep,
			    G4double yOut[],
			    G4double yErr[])
{    
  for(G4int i = 0; i < nVariables; i++)
    {yErr[i]=0;} // this is clearly wrong!
  AdvanceHelix(yInput, (G4ThreeVector)0, hstep, yOut);
}
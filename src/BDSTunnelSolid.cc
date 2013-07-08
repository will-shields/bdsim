#include "BDSTunnelSolid.hh"
#include "BDSTunnelSegmentSolidFactory.hh"
#include "BDSBeamline.hh"

BDSTunnelSolid::~BDSTunnelSolid(){
}

BDSTunnelSolid::BDSTunnelSolid(){
}

G4VSolid* BDSTunnelSolid::create(){
  G4cout << "BDSTunnelSolid::create - clearing" << G4endl;
  clear();
  G4cout << "BDSTunnelSolid::create - filling" << G4endl;
  fill();  
  G4cout << "BDSTunnelSolid::create - processing" << G4endl;
  process();
  G4cout << "BDSTunnelSolid::create - returning" << G4endl;
  return buildSolid();
}

void BDSTunnelSolid::clear(){
  _vecLengthSegment.clear();
  _vecDPhiSegment.clear();
}


void BDSTunnelSolid::fill(){
  for(BDSBeamline::Instance()->first();!BDSBeamline::Instance()->isDone(); BDSBeamline::Instance()->next()){
    _vecLengthSegment.push_back(BDSBeamline::Instance()->currentItem()->GetLength());
    _vecDPhiSegment.push_back(BDSBeamline::Instance()->currentItem()->GetAngle());
    _vecR.push_back(BDSBeamline::Instance()->currentItem()->GetTunnelRadius());
    _vecOffsetX.push_back(BDSBeamline::Instance()->currentItem()->GetTunnelOffsetX());
    _vecOffsetY.push_back(BDSGlobalConstants::Instance()->GetTunnelOffsetY());
    calcWidthMax();
  }
  _numSegments=_vecLengthSegment.size();
  _r=BDSGlobalConstants::Instance()->GetTunnelRadius();
}

void BDSTunnelSolid::process(){
  G4cout << "BDSTunnelSolid::process - removing zeroes" << G4endl;
  removeZeroLengthSegs();
  G4cout << "BDSTunnelSolid::process - consolidating" << G4endl;
  consolidate();
  G4cout << "BDSTunnelSolid::process - finished" << G4endl;
}

void BDSTunnelSolid::removeZeroLengthSegs(){
  vector<G4double>::iterator lenIter = _vecLengthSegment.begin();
  vector<G4double>::iterator dPhiIter = _vecDPhiSegment.begin();
  while(lenIter!=_vecLengthSegment.end()){
    if((*lenIter)==0){
      _vecLengthSegment.erase(lenIter);
      _vecDPhiSegment.erase(dPhiIter);
    } else {
      lenIter++; 
      dPhiIter++;
    }
  }
}

void BDSTunnelSolid::consolidate(){
  if(_vecLengthSegment.size()<=1) return;  //We only need to consolidate if there is more than one segment.
  //The radius of curvature swept by the torus 
  G4double rTor=0;
  G4double rTorNext=0;
  //Setting up the iterators
  vector<G4double>::iterator lenIter = _vecLengthSegment.begin();
  vector<G4double>::iterator dPhiIter = _vecDPhiSegment.begin();
  vector<G4double>::iterator lenIterNext = _vecLengthSegment.begin();
  lenIterNext++;
  vector<G4double>::iterator dPhiIterNext = _vecDPhiSegment.begin();
  dPhiIterNext++;
    
  //Looping over the segments
  while(lenIterNext!=_vecLengthSegment.end()){
    if(((*dPhiIter)==0)&&((*dPhiIterNext)==0)){
      //This and the previous segments are straight sections. Simply add the lengths of the two tubes....
      (*lenIter)+=(*lenIterNext);
      _vecLengthSegment.erase(lenIterNext);
      _vecDPhiSegment.erase(dPhiIterNext);
    } else {
      rTor = (*lenIter)/(*dPhiIter);
      rTorNext = (*lenIterNext)/(*dPhiIterNext);
      //...otherwise, if curved and same radius of curvature as previous segment
      if(rTor == rTorNext){
	(*dPhiIter)+=(*dPhiIterNext);
	(*lenIter)+=(*lenIterNext);
	_vecLengthSegment.erase(lenIterNext);
	_vecDPhiSegment.erase(dPhiIterNext);
      } else {
	//...otherwise, proceed to the next segment.
	lenIter++;
	lenIterNext++;
	dPhiIter++;
	dPhiIterNext++;
      }
    }
  }
}

G4VSolid* buildSolid(){
  buildSolidList();
  G4Transform3D totalTrans3D;
  G4Transform3D tempTransform3D;
  G4Transform3D internalTransform3D;
  G4Transform3D halfTransform3D;
  G4VSolid* tempSolid;
  totalTrans3D.setIdentity();
  tempTransform3D.setIdentity();
  list<G4VSolid*>::const_iterator solidIter = _solidList.begin();
  while(solidIter!=_solidList.end()){
    //Get the transform3d and compound with total...
    internalTransform3D=(*_solidList).transform3D();
    localTransform3D=tempTransform3D*totalTransform3D;
    //Create the UnionSolid...
  }
}

list<BDSPipeSection*> BDSTunnelSolid::buildPipeSectionList(){
  vector<G4double>::const_iterator lenIter = _vecLengthSegment.begin();
  vector<G4double>::const_iterator dPhiIter = _vecDPhiSegment.begin();
  G4String name = "tunnelCavitySolid";
  G4String nameSeg = "segmentSolid";
  
  while(lenIter!=_vecLengthSegment.end()){
    if(lenIter == _vecLengthSegment.begin()){
      _pipeSectionList.push_back(new BDSPipeSection(nameSeg,_r,*dPhiIter,*lenIter));
    }
    G4cout << "Added tunnel segment. " << G4endl;
    G4cout << "Volume of segment = " << _pipeSectionList.back()->solid()->GetCubicVolume()/m3 << " m^3" << G4endl;
    dPhiIter++; lenIter++;
  }
  //  G4VSolid* result = _pipeSectionList.back()->Clone();
  //  delete _pipeSectionList;
  return result;
}
 
void BDSTunnelSolid::calcWidthMax(){
  _widthMax.setX(std::max(_widthMax.x(),(2*(BDSBeamline::Instance()->currentItem()->GetTunnelRadius() + std::abs(BDSBeamline::Instance()->currentItem()->GetTunnelOffsetX() + BDSBeamline::Instance()->positionEnd()->x())))));
  _widthMax.setY(std::max(_widthMax.y(),2*(BDSBeamline::Instance()->currentItem()->GetTunnelRadius() + std::abs(BDSGlobalConstants::Instance()->GetTunnelOffsetY() + BDSBeamline::Instance()->positionEnd()->y()))));
  _widthMax.setZ(std::max(_widthMax.z(),BDSBeamline::Instance()->positionEnd()->z()));
}
 
G4ThreeVector BDSTunnelSolid::widthMax(){
  G4cout << "BDSTunnelCavity::widthmax() ... " << G4endl;
  G4cout << "BDSTunnelCavity::widthmax() x= " << _widthMax.x()/m << " m" << G4endl;
  G4cout << "BDSTunnelCavity::widthmax() y= " << _widthMax.y()/m << " m" << G4endl;
  G4cout << "BDSTunnelCavity::widthmax() z= " << _widthMax.z()/m << " m" << G4endl;
  return _widthMax;
}

G4double BDSTunnelSolid::maxAbs(vector<G4double> var){
  G4double lower = *std::min_element(var.begin(), var.end());
  G4double upper = *std::max_element(var.begin(), var.end());
  return std::max(std::abs(lower),std::abs(upper));
}

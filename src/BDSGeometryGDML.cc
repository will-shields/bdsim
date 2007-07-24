#ifdef USE_XML
#include "BDSGlobalConstants.hh" // must be first in include list
#include "BDSGeometryGDML.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Torus.hh"
#include "G4Polycone.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"
#include "BDSMySQLWrapper.hh"
#include "BDSMaterials.hh"
#include "BDSSamplerSD.hh"
#include "G4SDManager.hh"
#include "BDSSamplerSD.hh"
#include "BDSOutput.hh"
#include <vector>
#include <map>
#include <cstdlib>
#include "G4ClassicalRK4.hh"
#include <cstring>

using namespace std;

extern BDSSamplerSD* BDSSamplerSensDet;

extern BDSMaterials* theMaterials;
extern G4RotationMatrix* RotY90;
extern BDSOutput bdsOutput;
extern BDSGlobalConstants* BDSGlobals;

BDSGeometryGDML::BDSGeometryGDML(G4String GDMLfile)
{
  itsGDMLfile = GDMLfile;
  visRed = visGreen = visBlue = 0.0;

  // define some standard constants
  CONST_REF aconst;
  aconst.name="pi";
  aconst.value=pi;
  CONST_LIST.push_back(aconst);

  aconst.name="TWOPI";
  aconst.value=twopi;
  CONST_LIST.push_back(aconst);

  
  aconst.name="HALFPI";
  aconst.value=halfpi;
  CONST_LIST.push_back(aconst);

  // define units using mm and radians as =1
  
  
  aconst.name="radian";
  aconst.value=radian;
  CONST_LIST.push_back(aconst);

  aconst.name="degree";
  aconst.value=degree;
  CONST_LIST.push_back(aconst);

  
  aconst.name="mm";
  aconst.value=mm;
  CONST_LIST.push_back(aconst);

  
  aconst.name="cm";
  aconst.value=cm;
  CONST_LIST.push_back(aconst);

  
  aconst.name="m";
  aconst.value=m;
  CONST_LIST.push_back(aconst);

}

BDSGeometryGDML::~BDSGeometryGDML()
{;}

void BDSGeometryGDML::Construct(G4LogicalVolume *marker)
{
  itsMarkerVol = marker;
  parseDoc();

}

void BDSGeometryGDML::parseDoc()
{
  const char* docname = itsGDMLfile.c_str();
  xmlDocPtr doc;
  xmlNodePtr cur;

  doc = xmlParseFile(docname);

  if (doc == NULL )
  {
  G4Exception("Document not parsed successfully");
  }

  cur = xmlDocGetRootElement(doc); //GO the first node

  if (cur == NULL)
  {
   xmlFreeDoc(doc);
   G4Exception("empty document");
  }

//test the xml format, must be start by lcdd
   if (xmlStrcmp(cur->name, (const xmlChar *) "lcdd"))
   {
   xmlFreeDoc(doc);
   G4Exception("XML document of the wrong type, root node != lcdd\nCheck your XML file\n");
   }

   cur = cur->xmlChildrenNode;
   
   while (cur != NULL)
     {
       if ((!xmlStrcmp(cur->name, (const xmlChar *)"header")))
	 {
	   //Note coded yet
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"iddict")))
	 {
	   //Note coded yet
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"sensitive_detectors")))
	 {
	   //Note coded yet
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"limits")))
	 {
	   //Note coded yet
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"regions")))
	 {
	   //Note coded yet
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"display")))
	 {
	   parseDISPLAY(cur);
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"gdml")))
	 {
	   parseGDML(cur);
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"fields")))
	 {
	   //Note coded yet
	 }
       cur = cur->next;
     }

   xmlFreeDoc(doc);
   return;
}

void BDSGeometryGDML::parseDISPLAY(xmlNodePtr cur)
{
  cur = cur->xmlChildrenNode;

  while (cur != NULL)
     {
       if ((!xmlStrcmp(cur->name, (const xmlChar *)"vis")))
	 {
	   parseVIS(cur);
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"VisType")))
	 {
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"DisplayType")))
	 {
	   //Note coded yet
	 }
       cur = cur->next;
     }
  return;
}

void BDSGeometryGDML::parseVIS(xmlNodePtr cur)
{
  G4String name = parseStrChar(xmlGetProp(cur,(const xmlChar*)"name"));
  G4bool visible = parseBoolChar(xmlGetProp(cur,(const xmlChar*)"visible"));
  G4bool show_daughters = parseBoolChar(xmlGetProp(cur,(const xmlChar*)"show_daughters"));
  G4String line_style = parseStrChar(xmlGetProp(cur,(const xmlChar*)"line_style"));
  G4String drawing_style = parseStrChar(xmlGetProp(cur,(const xmlChar*)"drawing_style"));

  G4double R = 1.0, G = 1.0, B = 1.0, alpha = 1.0;

  cur = cur->xmlChildrenNode;
  
  while(cur!=NULL)
    {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"color")))
	{
	  R = parseDblChar(xmlGetProp(cur,(const xmlChar*)"R"));
	  G = parseDblChar(xmlGetProp(cur,(const xmlChar*)"G"));
	  B = parseDblChar(xmlGetProp(cur,(const xmlChar*)"R"));
	  alpha = parseDblChar(xmlGetProp(cur,(const xmlChar*)"alpha"));

	}
      else if ((!xmlStrcmp(cur->name, (const xmlChar *)"color")))
	{
	  
	}
      cur = cur->next;
    }

  G4VisAttributes* VisAtt = 
    new G4VisAttributes(G4Colour(R,G,B,alpha));

  if(drawing_style=="solid" )VisAtt->SetForceSolid(true);
  else if(drawing_style=="wireframe") VisAtt->SetForceWireframe(true);

  if(line_style=="unbroken" )VisAtt->SetLineStyle(G4VisAttributes::unbroken);
  else if(line_style=="dashed") VisAtt->SetLineStyle(G4VisAttributes::dashed);
  else if(line_style=="dotted") VisAtt->SetLineStyle(G4VisAttributes::dotted);

  VisAtt->SetVisibility(visible);
  if(show_daughters) VisAtt->SetDaughtersInvisible(false);
  else VisAtt->SetDaughtersInvisible(true);

  VIS_REF avis;
  avis.name = name;
  avis.value = VisAtt;
  VIS_LIST.push_back(avis);
  return;
}
void BDSGeometryGDML::parseGDML(xmlNodePtr cur)
{
  cur = cur->xmlChildrenNode;

  while (cur != NULL)
     {
       if ((!xmlStrcmp(cur->name, (const xmlChar *)"define")))
	 {
	   G4cout << "Importing GDML Definitions" << G4endl;
	   parseDEFINE(cur);
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"materials")))
	 {
	   G4cout << "Importing GDML Materials" << G4endl;
	   parseMATERIALS(cur);
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"solids")))
	 {
	   G4cout << "Importing GDML Solids" << G4endl;
	   parseSOLID(cur);
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"structure")))
	 {
	   G4cout << "Importing GDML Structures" << G4endl;
	   parseSTRUCTURE(cur);
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"setup")))
	 {
	   G4cout << "Importing GDML Setup" << G4endl;
	   G4String name = parseStrChar(xmlGetProp(cur,(const xmlChar*)"name"));
	   G4String version = parseStrChar(xmlGetProp(cur,(const xmlChar*)"version"));
	   xmlNodePtr tempcur = cur->xmlChildrenNode;
	   G4String worldref;

	   while(tempcur!=NULL)
	     {
	       if ((!xmlStrcmp(tempcur->name, (const xmlChar *)"world")))
		 worldref = parseStrChar(xmlGetProp(tempcur,(const xmlChar*)"ref"));
	       tempcur = tempcur->next;
	     }

	   G4LogicalVolume* topvol = GetLogVolByName(worldref);
	   new G4PVPlacement(NULL,
			     G4ThreeVector(0.,0.,0.),
			     topvol,
			     topvol->GetName()+"_PhysiComp",
			     itsMarkerVol,
			     false,
			     0);


	 }
       cur = cur->next;
     }
  return;
}

void BDSGeometryGDML::parseSTRUCTURE(xmlNodePtr cur)
{
  cur = cur->xmlChildrenNode;

  while (cur != NULL)
     {
       
       if ((!xmlStrcmp(cur->name, (const xmlChar *)"volume")))
	 parseVOLUME(cur);
       cur = cur->next;
     }
  
  return;
}


void BDSGeometryGDML::parseDEFINE(xmlNodePtr cur)
{
  cur = cur->xmlChildrenNode;

  while (cur != NULL)
     {
       
       if ((!xmlStrcmp(cur->name, (const xmlChar *)"rotation")))
	 {	
	   ROT_REF arot;
	   arot.name=(parseStrChar(xmlGetProp(cur,(const xmlChar*)"name")));
	   arot.value=G4ThreeVector(parseDblChar(xmlGetProp(cur,(const xmlChar*)"x")),
				    parseDblChar(xmlGetProp(cur,(const xmlChar*)"y")),
				    parseDblChar(xmlGetProp(cur,(const xmlChar*)"z")));

	   ROT_LIST.push_back(arot);
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"position")))
	 {
	   POS_REF apos;
	   apos.name=(parseStrChar(xmlGetProp(cur,(const xmlChar*)"name")));
	   apos.value=G4ThreeVector(parseDblChar(xmlGetProp(cur,(const xmlChar*)"x")),
				    parseDblChar(xmlGetProp(cur,(const xmlChar*)"y")),
				    parseDblChar(xmlGetProp(cur,(const xmlChar*)"z")));

	   POS_LIST.push_back(apos);
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"constant")))
	 {
	   CONST_REF aconst;
	   aconst.name=(parseStrChar(xmlGetProp(cur,(const xmlChar*)"name")));
	   aconst.value=parseDblChar(xmlGetProp(cur,(const xmlChar*)"value"));
	   CONST_LIST.push_back(aconst);
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"expression")))
	 {
	   //Not coded yet
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"quantity")))
	 {
	   //Not coded yet
	 }
       cur = cur->next;
     }
  
  return;
}

void BDSGeometryGDML::parseMATERIALS(xmlNodePtr cur)
{
  cur = cur->xmlChildrenNode;

  while (cur != NULL)
     {
       
       if ((!xmlStrcmp(cur->name, (const xmlChar *)"define")))
	 {
	   //Not coded yet
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"isotope")))
	 {
	   //Not coded yet
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"element")))
	 {
	   //Not coded yet
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"material")))
	 {
	   //Not coded yet
	 }
       cur = cur->next;
     }
  
  return;

}

void BDSGeometryGDML::parseSOLID(xmlNodePtr cur)
{
  cur = cur->xmlChildrenNode;

  while (cur != NULL)
     {
       
       if ((!xmlStrcmp(cur->name, (const xmlChar *)"tube")))
	 BuildTube(cur);

       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"box")))
	 BuildBox(cur);

       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"cone")))
	 {
	   G4Exception("GDML cone not currently implemented in BDSIM");
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"eltube")))
	 {
	   G4Exception("GDML eltube not currently implemented in BDSIM");
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"hype")))
	 {
	   G4Exception("GDML hype not currently implemented in BDSIM");
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"intersection")))
	 {
	   G4Exception("GDML intersection not currently implemented in BDSIM");
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"orb")))
	 {
	   G4Exception("GDML orb not currently implemented in BDSIM");
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"polyhedra")))
	 {
	   G4Exception("GDML polyhedra not currently implemented in BDSIM");
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"sphere")))
	 {
	   G4Exception("GDML sphere not currently implemented in BDSIM");
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"trap")))
	 {
	   G4Exception("GDML trap not currently implemented in BDSIM");
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"subtraction")))
	 BuildSubtraction(cur);

       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"torus")))
	 {
	   G4Exception("GDML torus not currently implemented in BDSIM");
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"trd")))
	 {
	   G4Exception("GDML trd not currently implemented in BDSIM");
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"union")))
	 {
	   G4Exception("GDML union not currently implemented in BDSIM");
	 }

       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"polycone")))
	 BuildPolycone(cur);
	   
       cur = cur->next;
     }
  
  return;
}

void BDSGeometryGDML::parseVOLUME(xmlNodePtr cur)
{
  G4String volume_name = parseStrChar(xmlGetProp(cur,(const xmlChar*)"name"));
  cur = cur->xmlChildrenNode;

  G4String materialref;
  G4String solidref;
  G4String visref;
  xmlNodePtr origcur = cur;
  while (cur != NULL)
     {
       if ((!xmlStrcmp(cur->name, (const xmlChar *)"materialref")))
	 {
	   materialref = parseStrChar(xmlGetProp(cur,(const xmlChar*)"ref"));
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"solidref")))
	 {
	   solidref = parseStrChar(xmlGetProp(cur,(const xmlChar*)"ref"));  
	 }
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"visref")))
	 {
	   visref = parseStrChar(xmlGetProp(cur,(const xmlChar*)"ref"));  
	 }
       
       cur = cur->next;
     }

  if(materialref!="" && solidref!="")
    {
      // TEMP 
      materialref="iron";
      G4LogicalVolume* alogvol = new G4LogicalVolume(GetSolidByName(solidref),
						     theMaterials->GetMaterial(materialref),
						     volume_name);
      
      LOGVOL_LIST.push_back(alogvol);

      alogvol->SetVisAttributes(GetVisByName(visref));
      
      while(origcur!=NULL)
	{
	  if ((!xmlStrcmp(origcur->name, (const xmlChar *)"physvol")))
	    {
	      parsePHYSVOL(origcur, volume_name);
	    }
	  origcur = origcur->next;
	}
    }
  
  else
    {
      G4cout << "Can't build" << volume_name << " : " << solidref << G4endl;
      G4Exception();
    }
  

  
  return;
}

void BDSGeometryGDML::parsePHYSVOL(xmlNodePtr cur, G4String volume_name)
{
  cur = cur->xmlChildrenNode;

  G4String volumeref;

  G4RotationMatrix* componentRotation = NULL;
  
  G4ThreeVector PlacementPoint;


  while (cur != NULL)
     {
       if ((!xmlStrcmp(cur->name, (const xmlChar *)"volumeref")))
	 volumeref = parseStrChar(xmlGetProp(cur,(const xmlChar*)"ref"));

       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"positionref")))
	 PlacementPoint = GetPosition(parseStrChar(xmlGetProp(cur,(const xmlChar*)"ref")));
       
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"position")))
	 PlacementPoint = GetPosition(cur);
       
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"rotationref")))
	 componentRotation = GetRotation(parseStrChar(xmlGetProp(cur,(const xmlChar*)"ref")));
       
       else if ((!xmlStrcmp(cur->name, (const xmlChar *)"rotation")))
	 componentRotation = GetRotation(cur);

       cur = cur->next;
     }

  G4LogicalVolume* currentVol = GetLogVolByName(volumeref);
  G4LogicalVolume* parentVol = GetLogVolByName(volume_name);

  new G4PVPlacement(componentRotation,
		    PlacementPoint,
		    currentVol,
		    currentVol->GetName()+"_PhysiComp",
		    parentVol,
		    false,
		    0);
  

  return;
}

 G4RotationMatrix* BDSGeometryGDML::RotateComponent(G4ThreeVector rotvalues)
{
  G4RotationMatrix *rotateComponent = new G4RotationMatrix;
  
  //Assuming order is rotate x, then y, then z;

  rotateComponent->rotateX(rotvalues.x());
  rotateComponent->rotateY(rotvalues.y());
  rotateComponent->rotateZ(rotvalues.z());
  
  
  return rotateComponent;
}

#endif

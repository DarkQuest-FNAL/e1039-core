#include "PHFieldUtility.h"

#include "PHFieldConfig_v1.h"

#include "PHField2D.h"
#include "PHField3DCartesian.h"
#include "PHField3DCylindrical.h"
#include "PHFieldUniform.h"
#include "PHFieldRegionalConst.h"
#include "PHFieldSeaQuest.h"

// PHENIX includes
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHTypedNodeIterator.h>
#include <phool/getClass.h>
#include <phool/recoConsts.h>

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <sys/types.h>
#include <unistd.h>  // for generate unique local file
using namespace std;

PHFieldUtility::PHFieldUtility()
{
}

PHFieldUtility::~PHFieldUtility()
{
}

PHField *
PHFieldUtility::BuildFieldMap(const PHFieldConfig *field_config, const int verbosity)
{
  assert(field_config);

  if (verbosity)
  {
#ifdef _DEBUG_ON
    cout << "PHFieldUtility::BuildFieldMap - construction field with configuration: ";
    field_config->identify();
#endif
  }

  PHField *field(nullptr);

  switch (field_config->get_field_config())
  {
  case PHFieldConfig::kFieldUniform:
    //    return "Constant field";

    field = new PHFieldUniform(
        field_config->get_field_mag_x(),
        field_config->get_field_mag_y(),
        field_config->get_field_mag_z());

    break;
  case PHFieldConfig::kField2D:
    //    return "2D field map expressed in cylindrical coordinates";
    field = new PHField2D(
        field_config->get_filename(),
        verbosity,
        field_config->get_magfield_rescale());

    break;
  case PHFieldConfig::kField3DCylindrical:
    //    return "3D field map expressed in cylindrical coordinates";
    field = new PHField3DCylindrical(
        field_config->get_filename(),
        verbosity,
        field_config->get_magfield_rescale());

    break;
  case PHFieldConfig::kField3DCartesian:
    //    return "3D field map expressed in Cartesian coordinates";
    field = new PHField3DCartesian(
        field_config->get_filename(),
        field_config->get_magfield_rescale());
		break;
  case PHFieldConfig::RegionalConst:
    //    return "3D field map expressed in Cartesian coordinates";
    field = new PHFieldRegionalConst(
    		5.0,
        1.0);
		break;
  case PHFieldConfig::kFieldSeaQuest:
  	field = new PHFieldSeaQuest(
        field_config->get_filename1(),
				field_config->get_filename2(),
				field_config->get_magfield_rescale1(),
				field_config->get_magfield_rescale2(),
				field_config->get_taregetmag_y()
  			);
    break;
  default:
    cout << "PHFieldUtility::BuildFieldMap - Invalid Field Configuration" << endl;
    //    return nullptr;
    //    return "Invalid Field";
  }
  assert(field);  // Check for Invalid Field
  return field;
}

//! Make a default PHFieldConfig
//! Field map = /phenix/upgrades/decadal/fieldmaps/sPHENIX.2d.root
//! Field Scale to 1.4/1.5
//! \output default field configuration object. Caller assumes ownership
PHFieldConfig *
PHFieldUtility::
    DefaultFieldConfig()
{
  return new PHFieldConfig_v1(PHFieldConfig_v1::kField2D,
                              "/phenix/upgrades/decadal/fieldmaps/sPHENIX.2d.root",
                              1.4 / 1.5);
}

//! Get transient PHField from DST nodes. If not found, make a new one based on default_config
PHField *
PHFieldUtility::GetFieldMapNode(const PHFieldConfig *default_config, PHCompositeNode *topNode, const int verbosity)
{
  if (topNode == nullptr) topNode = Fun4AllServer::instance()->topNode();
  assert(topNode);
  PHNodeIterator iter(topNode);

  // Looking for the RUN node
  PHCompositeNode *parNode = static_cast<PHCompositeNode *>(iter.findFirst(
      "PHCompositeNode", "PAR"));
  if (!parNode)
  {
    stringstream serr;
    serr << __PRETTY_FUNCTION__ << ": PAR Node missing, request aborting.";
    cout << serr.str() << endl;

    throw runtime_error(serr.str());

    return NULL;
  }

  PHField *field = findNode::getClass<PHField>(parNode, GetDSTFieldMapNodeName());
  if (!field)
  {
    PHFieldConfig *field_config =
        GetFieldConfigNode(default_config, topNode, verbosity);
    assert(field_config);

    field = BuildFieldMap(field_config, verbosity>0?verbosity-1:verbosity);
    assert(field);

    parNode->addNode(new PHDataNode<PHObject>(field, GetDSTFieldMapNodeName(), "PHObject"));
  }

  return field;
}

//! Get persistent PHGeomIOTGeo from DST nodes. If not found, make a new one
PHFieldConfig *
PHFieldUtility::GetFieldConfigNode(const PHFieldConfig *default_config, PHCompositeNode *topNode, const int verbosity)
{
  if (topNode == nullptr) topNode = Fun4AllServer::instance()->topNode();
  assert(topNode);

  PHNodeIterator iter(topNode);

  // Looking for the RUN node
  PHCompositeNode *runNode = static_cast<PHCompositeNode *>(iter.findFirst(
      "PHCompositeNode", "RUN"));
  if (!runNode)
  {
    stringstream serr;
    serr << __PRETTY_FUNCTION__ << ": RUN Node missing, request aborting.";
    cout << serr.str() << endl;

    throw runtime_error(serr.str());

    return NULL;
  }

  PHFieldConfig *field = findNode::getClass<PHFieldConfig>(runNode,
                                               GetDSTConfigNodeName());
  if (!field)
  {
    if (!default_config)
    {
      field = DefaultFieldConfig();
      if (verbosity)
          {
          cout <<"PHFieldUtility::GetFieldConfigNode - field map with configuration from build-in default: ";
          field->identify();
        }
    }
    else
    {
      field = static_cast<PHFieldConfig *>(default_config->Clone());
      if (verbosity)
          {
          cout <<"PHFieldUtility::GetFieldConfigNode - field map with configuration from input default: ";
          field->identify();
        }
    }

    assert(field);
    runNode->addNode(new PHIODataNode<PHObject>(field,
                                              GetDSTConfigNodeName(), "PHObject"));
  }
  else
  {
    if (verbosity)
    {
    cout <<"PHFieldUtility::GetFieldConfigNode - field map with configuration from DST/RUN: ";
    field->identify();
  }}

  return field;
}

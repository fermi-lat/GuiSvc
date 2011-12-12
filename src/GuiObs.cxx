/** @file GuiObs.cxx 
   @brief Implementation file for GuiSvc

gets adresses 
 of CLHEP random number engines used in Gleam shared libraries
 and sets seeds for them based on run and particle sequence
 number obtained from the MCHeader

 $Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/GuiSvc/src/Attic/GuiObs.cxx,v 1.1.4.1 2011/01/14 03:44:03 heather Exp $

 Author: Toby Burnett, Karl Young
*/

#include "GuiSvc/GuiObs.h"
#include "facilities/Util.h"

#include <iterator>
#include <fstream>
#include "GaudiKernel/Incident.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/SvcFactory.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/DataStoreItem.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/SmartDataPtr.h"


GuiObs::GuiObs():IToolSvc::Observer(),m_guiMgr(0)
{
   
}


void GuiObs::onCreate(IAlgTool& tool) {


    IGuiTool* gtool;
    StatusCode status =tool.queryInterface( IGuiTool::interfaceID(), (void**)&gtool);
    if( status.isSuccess() ){
        gtool->initialize(m_guiMgr);
    }


}



GuiObs::~GuiObs () 
{
    // Purpose and Method: finish up, delete the list of engines
    //
    // Inputs:  none
    // Outputs: none
    // Dependencies: None
    // Restrictions and Caveats:  None

}



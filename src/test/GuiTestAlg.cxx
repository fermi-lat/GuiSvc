// $Header: /nfs/slac/g/glast/ground/cvs/GuiSvc/src/test/GuiTestAlg.cxx,v 1.1.1.1 2001/01/04 01:03:32 burnett Exp $

// Include files
#include "GuiTestAlg.h"
#include "GuiSvc/IGuiSvc.h"

#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"

#include "src/test/Demo.h"
#include "gui/GuiMgr.h"
 
static const AlgFactory<GuiTestAlg>  Factory;
const IAlgFactory& GuiTestAlgFactory = Factory;


//------------------------------------------------------------------------------
//
GuiTestAlg::GuiTestAlg(const std::string& name, ISvcLocator* pSvcLocator) :
Algorithm(name, pSvcLocator), m_guiMgr(0), m_demo(0) {
 
}


//------------------------------------------------------------------------------
/*! */
StatusCode GuiTestAlg::initialize() {
    
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "initialize" << endreq;
    
    // Use the Job options service to set the Algorithm's parameters
    setProperties();

    // get the pointer to the GUI Service 
    IGuiSvc* gsvc=0;

    // get the service
    StatusCode sc = service("GuiSvc", gsvc);
    
    if( sc.isFailure()) {
        log << MSG::ERROR << "Could not find GuiSvc" << endreq;
        return sc;
    }
    // now get the gui manager and set up the demo from gui.
    m_guiMgr = gsvc->guiMgr();

    m_demo = new Demo(m_guiMgr->display());

    return sc;
}


//------------------------------------------------------------------------------
StatusCode GuiTestAlg::execute() {
    
    StatusCode  sc = StatusCode::SUCCESS;
    MsgStream   log( msgSvc(), name() );

    // have the demo object do its thing.
    m_demo->process();
    return sc;
}


//------------------------------------------------------------------------------
StatusCode GuiTestAlg::finalize() {
    
    delete m_demo;
    return StatusCode::SUCCESS;
}







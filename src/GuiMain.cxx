// $Header: /nfs/slac/g/glast/ground/cvs/GuiSvc/src/GuiMain.cxx,v 1.1 2001/03/22 21:07:39 burnett Exp $

// Include files
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IAppMgrUI.h"
#include "GaudiKernel/IProperty.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/System.h"

//------------------------------------------------------------------------------
//
//  Package    : GuiSvc
//
//  Description: Graphics Main Program
//
//------------------------------------------------------------------------------
/**
  Standard Glast-Gaudi main.
  Specify the job options file location by:
  1) specification, local or global, on command line.
  2) content of env var JOBOPTIONS
  3) src/jobOPtions

  */
int main( int argn, char** argc) {

    std::string joboptions_file;

    // is ther a job options env var defined?
    const char * job = ::getenv("JOBOPTIONS");

    if( argn>1 ) {job = argc[1];}
    else if( job ) { joboptions_file = job; }
    else {
        // no args, JOBOPTIONS var: assume this.
        joboptions_file ="src/jobOptions.txt";
    }
    // Create an instance of an application manager
    IInterface* iface = Gaudi::createApplicationMgr();
    
    SmartIF<IProperty>     propMgr ( IID_IProperty, iface );
    SmartIF<IAppMgrUI>     appMgr  ( IID_IAppMgrUI, iface );
    
    // Set properties of algorithms and services
    StatusCode status = StatusCode::SUCCESS;
    if ( iface && propMgr == iface )    {
        status = propMgr->setProperty( "JobOptionsPath", joboptions_file );
    }
    else  {
        exit(-1);
    }

    // Run the application manager and process events
    if ( appMgr )   {
        status = appMgr->run();
    }
    else  {
        return 0;
    }
    // All done - exit
    return 0;
    
}

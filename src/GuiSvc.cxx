// $Header: /nfs/slac/g/glast/ground/cvs/GuiSvc/src/GuiSvc.cxx,v 1.3 2001/03/01 23:48:03 igable Exp $
// 
//  Original author: Toby Burnett tburnett@u.washington.edu
//

#include "GuiSvc/GuiSvc.h"

#include "GaudiKernel/SvcFactory.h"
#include "GaudiKernel/MsgStream.h"

#include "GaudiKernel/Incident.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Property.h"

// includes for implementing GuiMgr stuff
#include "gui/GuiMgr.h"
#include "gui/Menu.h"
#include "gui/SubMenu.h"
#include "gui/SimpleCommand.h"

// declare the service factories for the GuiSvc
static SvcFactory<GuiSvc> a_factory;
const ISvcFactory& GuiSvcFactory = a_factory;


// ------------------------------------------------
// Implementation of the GuiSvc class
// ------------------------------------------------
/// Standard Constructor
GuiSvc::GuiSvc(const std::string& name,ISvcLocator* svc)
: Service(name,svc)
{
    
    // declare the properties and set defaults

    declareProperty ("size", m_size=-300);
    declareProperty( "pause_interval", m_pause_interval=0);
    declareProperty( "paused", m_paused=true);

}






// initialize
StatusCode GuiSvc::initialize () 
{
    using namespace gui;
    StatusCode  status =  Service::initialize ();

        
    // bind all of the properties for this service
    setProperties ();
    
    // open the message log
    MsgStream log( msgSvc(), name() );
 
    // create and start the gui manager
    m_guiMgr = new gui::GuiMgr(m_size, 2, m_pause_interval); 

    // use the incident service to register begin, end events
    IIncidentSvc* incsvc = 0;
    StatusCode sc = serviceLocator()->getService ("IncidentSvc",
        IID_IIncidentSvc, reinterpret_cast<IInterface*&>( incsvc ));
    
    if( status.isFailure() ) return status;

    incsvc->addListener(this, "BeginEvent", 100);
    incsvc->addListener(this, "EndEvent", 0);

    // get the menu for additional buttons.
    gui::SubMenu& sub_menu= m_guiMgr->subMenu();

    sub_menu.addButton("set max event...",
        new SimpleCommand<GuiSvc>(this, &GuiSvc::queryEvtMax));

    sub_menu.addButton("Quit Loop",
        new SimpleCommand<GuiSvc>(this, &GuiSvc::quit));

    m_guiMgr->menu().add(new MenuClient<GuiSvc>(this)); // schedule callback if exit button pressed

    //TODO: make sure it will get to the first event
    return status;
}


void GuiSvc::queryPause()
{
    m_guiMgr->menu().query("Enter new pause interval in ms (0: no pause, -1 infinite)", 
        &m_pause_interval);	
}
void GuiSvc::queryEvtMax()
{
    IProperty* glastPropMgr=0;
    StatusCode status = 
        serviceLocator()->getService("EventSelector", IID_IProperty,
                         reinterpret_cast<IInterface*&>( glastPropMgr ));
    if( status.isFailure() ) return;
      
    IntegerProperty evtMax("EvtMax",0);
    status = glastPropMgr->getProperty( &evtMax );
    if (status.isFailure()) return;

    int max_event = evtMax.value();
    m_guiMgr->menu().query("Enter new max_event",& max_event);

    evtMax = max_event; 
    status = glastPropMgr->setProperty( evtMax );
    glastPropMgr->release();

}


void GuiSvc::quit() {
    IProperty* glastPropMgr=0;
    StatusCode status = 
        serviceLocator()->getService("EventSelector", IID_IProperty,
                         reinterpret_cast<IInterface*&>( glastPropMgr ));
    if( status.isFailure() ) return;
      
    IntegerProperty evtMax("EvtMax",0);
    status = glastPropMgr->getProperty( &evtMax );
    if (status.isFailure()) return;

    evtMax = -1; 
    status = glastPropMgr->setProperty( evtMax );
    glastPropMgr->release();
    m_guiMgr->resume();
}

// handle "incidents"
void GuiSvc::handle(const Incident &inc)
{
    if( inc.type()=="BeginEvent")beginEvent();
    else if(inc.type()=="EndEvent")endEvent();

}


void GuiSvc::beginEvent() // should be called at the beginning of an event
{ 
    using namespace gui;
    static bool first=true;
    if( first) { first=false;

        // start of first event: startup the GUI
        m_guiMgr->setState( m_paused?  GuiMgr::PAUSED : GuiMgr::RUNNING);
        MsgStream log( msgSvc(), name() );

        log << MSG::INFO << "starting GuiSvc GUI in " 
            << (m_guiMgr->state()==GuiMgr::PAUSED? "PAUSED": "RUNNING" ) << " state" << endreq;
        m_guiMgr->menu().run();
    }
    m_guiMgr->begin_event();
}

void GuiSvc::endEvent()  // must be called at the end of an event to update, allow pause
{        
    m_guiMgr->end_event();
}


// finalize
StatusCode GuiSvc::finalize ()
{
    StatusCode  status = StatusCode::SUCCESS;
       
    delete m_guiMgr;
    return status;
}
/// Query interface
StatusCode GuiSvc::queryInterface(const IID& riid, void** ppvInterface)  {
  if ( IID_IGuiSvc.versionMatch(riid) )  {
    *ppvInterface = (IGuiSvc*)this;
  }
  else  {
    return Service::queryInterface(riid, ppvInterface);
  }
  addRef();
  return SUCCESS;
}


/** 
* @file GuiSvc.cxx
* @brief definition of the class GuiSvc
*
*  $Header: /nfs/slac/g/glast/ground/cvs/GuiSvc/src/GuiSvc.cxx,v 1.23 2011/12/12 22:48:56 heather Exp $
*/

#include "GuiSvc/GuiSvc.h"

#include "GaudiKernel/SvcFactory.h"
#include "GaudiKernel/MsgStream.h"

#include "GaudiKernel/Incident.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/IAppMgrUI.h"
#include "GaudiKernel/SmartIF.h"
//#include "GaudiKernel/IObjManager.h"
//#include "GaudiKernel/IToolFactory.h"
#include "GaudiKernel/IAlgManager.h"
#include "GaudiKernel/Algorithm.h"


#include "GuiSvc/IGuiTool.h"

// includes for implementing GuiMgr stuff
#include "gui/GuiMgr.h"
#include "gui/Menu.h"
#include "gui/SubMenu.h"
#include "gui/SimpleCommand.h"

// declare the service factories for the GuiSvc
//static SvcFactory<GuiSvc> a_factory;
//const ISvcFactory& GuiSvcFactory = a_factory;
DECLARE_SERVICE_FACTORY(GuiSvc);

// ------------------------------------------------
// Implementation of the GuiSvc class
// ------------------------------------------------
/// Standard Constructor
GuiSvc::GuiSvc(const std::string& name,ISvcLocator* svc)
: Service(name,svc), m_guiMgr(0)
{

    // declare the properties and set defaults

    declareProperty ("size", m_size=-3000);
    declareProperty( "pause_interval", m_pause_interval=0);
    declareProperty( "paused", m_paused=true);

    declareProperty("EvtMax", m_evtMax=0xFEEDBABE);


}

// ___________________________________________________________

void GuiSvc::pause()
{
    m_guiMgr->pause();
}


// initialize
// ___________________________________________________________

StatusCode GuiSvc::initialize () 
{
    using namespace gui;
    //if(state()==INITIALIZED) return StatusCode::SUCCESS;
    if(FSMState()==Gaudi::StateMachine::INITIALIZED) return StatusCode::SUCCESS;
    StatusCode  status =  Service::initialize ();


    // bind all of the properties for this service
    setProperties ();

    // open the message log
    MsgStream log( msgSvc(), name() );

    // create and start the gui manager
    m_guiMgr = new gui::GuiMgr(m_size, 2, m_pause_interval); 

    // use the incident service to register begin, end events
    IIncidentSvc* incsvc = 0;
    StatusCode sc = service ("IncidentSvc", incsvc, true);

    if( status.isFailure() ) return status;

    incsvc->addListener(this, "BeginEvent", 100);
    incsvc->addListener(this, "EndEvent", 0);

    // get the menu for additional buttons.
    gui::SubMenu& sub_menu= m_guiMgr->subMenu();

    sub_menu.addButton("set max event...",
        new SimpleCommand<GuiSvc>(this, &GuiSvc::queryEvtMax));
#if 0 // doesn't work TODO to fix
    m_guiMgr->menu().file_menu().addButton("set OutputLevel...",
        new SimpleCommand<GuiSvc>(this, &GuiSvc::queryOutputLevel));
#endif
#if 0 // seems to crash on X TODO: figure out why, if we need this.
    sub_menu.addButton("Quit Loop",
        new SimpleCommand<GuiSvc>(this, &GuiSvc::quit));
#endif
#if 0 // already implemented somewhere else
    sub_menu.addButton("Set Pause interval...", 
        new SimpleCommand<GuiMgr>(m_guiMgr, &GuiMgr::queryPause));
#endif
    m_guiMgr->menu().add(new MenuClient<GuiSvc>(this)); // schedule callback if exit button pressed



    sc = serviceLocator()->queryInterface(IAppMgrUI::interfaceID(), (void**)&m_appMgrUI);

    // get property from application manager
    if ( m_evtMax == (int)0xFEEDBABE )   {
        IProperty* propMgr=0;
        status = serviceLocator()->service("ApplicationMgr", propMgr );
        if( status.isFailure()) {
            log << MSG::ERROR << "Unable to locate PropertyManager Service" << endreq;
            return status;
        }

        IntegerProperty evtMax("EvtMax",0);
        status = propMgr->getProperty( &evtMax );
        if (status.isFailure()) return status;

        setProperty(evtMax);
    }

  // get a pointer to the tool service
  status = service( "ToolSvc", m_toolSvc, true );
  if (!status.isSuccess()) {
    log << MSG::ERROR << "Unable to get a handle to the tool service" << endmsg;
    return status;
  } else {
    log << MSG::DEBUG << "Got pointer to ToolSvc " << endmsg;
  }


    m_guiObs = new GuiObs();
    m_guiObs->setGuiMgr(m_guiMgr);
    m_toolSvc->registerObserver(m_guiObs);



    //----------------------------------------------------------------
    // most of  the following cribbed from ToolSvc and ObjManager

    // look for a factory of an AlgTool that implements the IGuiTool interface:
    // if found, make one and call the special method 

    // Manager of the AlgTool Objects
    /*IObjManager* objManager=0;             

    // locate Object Manager to locate later the tools 
    status = serviceLocator()->service("ApplicationMgr", objManager );
    if( status.isFailure()) {
        log << MSG::ERROR << "Unable to locate ObjectManager Service" << endreq;
        return status;
    }

    IToolFactory* toolfactory = 0;

    // search through all object factories for tool factories
    for(IObjManager::ObjIterator it = objManager->objBegin(); it !=objManager->objEnd(); ++ it){

        std::string tooltype= (*it)->ident();
        // is it a tool factory?
        const IFactory* factory = objManager->objFactory( tooltype );
        IFactory* fact = const_cast<IFactory*>(factory);
        status = fact->queryInterface( IID_IToolFactory, (void**)&toolfactory );
        if( status.isSuccess() ) {

            // found a tool factory: have it create a tool, and check its interface
            std::string fullname = this->name()+"."+tooltype;
            IAlgTool* itool = toolfactory->instantiate(fullname, this );
            IGuiTool* gtool;
            status =itool->queryInterface( IGuiTool::interfaceID(), (void**)&gtool);
            if( status.isSuccess() ){
                log << MSG::DEBUG << "Initializing gui stuff in " << tooltype << endreq;
                gtool->initialize(m_guiMgr);
            }
            itool->release();
        }
    }
*/

    return StatusCode::SUCCESS;
}

// ___________________________________________________________

void GuiSvc::queryEvtMax()
{

    m_guiMgr->menu().query("Enter new max event",& m_evtMax);
}

// ___________________________________________________________

void GuiSvc::queryOutputLevel()
{
    IProperty* glastPropMgr=0;
    StatusCode status = service("MessageSvc", glastPropMgr, true);
    if( status.isFailure() ) return;

    IntegerProperty levelProp("OutputLevel",0);
    status = glastPropMgr->getProperty( &levelProp );
    if (status.isFailure()) return;

    int level = levelProp.value();
    m_guiMgr->menu().query("Enter new outputlevel(2=DEBUG, 3=INFO, 4=WARNING, 5=ERROR, 6=FATAL )",& level);

    levelProp =level;
    status = glastPropMgr->setProperty( levelProp );
    glastPropMgr->release();

}

// ___________________________________________________________

void GuiSvc::quit() {
    m_evtMax=0;
    m_guiMgr->resume();
}


// handle "incidents"
// ___________________________________________________________

void GuiSvc::handle(const Incident &inc)
{
#if 0 // don't need now that implementing Runable interface
    if( inc.type()=="BeginEvent")beginEvent();
    else if(inc.type()=="EndEvent")endEvent();
#endif
}

// ___________________________________________________________

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

// ___________________________________________________________

void GuiSvc::endEvent()  // must be called at the end of an event to update, allow pause
{        
    m_guiMgr->end_event();
}

// ___________________________________________________________

StatusCode GuiSvc::run(){
    StatusCode status = StatusCode::FAILURE;
    MsgStream log( msgSvc(), name() );

    if ( 0 == m_appMgrUI ) return status;
    // now find the top alg so we can monitor its event count
    //
    IAlgManager* theAlgMgr;
    status = serviceLocator( )->getService( "ApplicationMgr",
        IAlgManager::interfaceID(),
        (IInterface*&)theAlgMgr );
    IAlgorithm* theIAlg;
    Algorithm*  theAlgorithm=0;
    IntegerProperty errorProperty("ErrorCount",0);

    status = theAlgMgr->getAlgorithm( "Top", theIAlg );
    if ( status.isSuccess( ) ) {
        try{
            theAlgorithm = dynamic_cast<Algorithm*>(theIAlg);
        } catch(...){
            status = StatusCode::FAILURE;
        }
    }
    if ( status.isFailure( ) ) {
        log << MSG::WARNING << "Could not find algorithm 'Top'; will not monitor errors" << endreq;
    }


    // loop over the events
    int event= 0;
    while(event++ < m_evtMax) {
        beginEvent();
        status =  m_appMgrUI->nextEvent(1); // currently, always success

        // the single event may have created a failure. Check the ErrorCount propery of the Top alg.
        if( theAlgorithm !=0) theAlgorithm->getProperty(&errorProperty);
        if( status.isFailure() || errorProperty.value() > 0){
            status = StatusCode::FAILURE;
            m_guiMgr->gui().inform("Event failed: terminating job after current display");
        }

        endEvent();
        if( status.isFailure()){
            log << MSG::ERROR << "Terminating Gui loop due to error" << endreq;
            break;
        }
    }
    return status;

}

// finalize
// ___________________________________________________________

StatusCode GuiSvc::finalize ()
{
    StatusCode  status = StatusCode::SUCCESS;

    delete m_guiMgr;
    return status;
}

// Query interface
// ___________________________________________________________

StatusCode GuiSvc::queryInterface(const InterfaceID& riid, void** ppvInterface)  {
    if ( IID_IGuiSvc.versionMatch(riid) )  {
        *ppvInterface = (IGuiSvc*)this;
    }
    else if (IRunable::interfaceID() == riid ) {
        *ppvInterface = (IRunable*)this;
    }
    else  {
        return Service::queryInterface(riid, ppvInterface);
    }
    addRef();
    return SUCCESS;
}

// $Header: /nfs/slac/g/glast/ground/cvs/GuiSvc/src/test/GuiTestAlg.h,v 1.1.1.1 2001/01/04 01:03:32 burnett Exp $
#ifndef GuiTestAlg_H
#define GuiTestAlg_H

// Include files
#include "GaudiKernel/Algorithm.h"

// forward declarations
namespace gui{ class GuiMgr; }
class Demo;

/*! \class GuiTestAlg
\brief 

  */

class GuiTestAlg : public Algorithm {

public:
  //! Constructor of this form must be provided
  GuiTestAlg(const std::string& name, ISvcLocator* pSvcLocator); 

  StatusCode initialize();
  StatusCode execute();
  StatusCode finalize();

private:
    gui::GuiMgr* m_guiMgr;
    Demo*        m_demo;
};


#endif // Graphics_BeginEvent_H

/** 
* @file GuiSvc_load.cxx
* @brief This is needed for forcing the linker to load all components
* of the library.
*
*  $Header: /nfs/slac/g/glast/ground/cvs/GuiSvc/src/GuiSvc_load.cxx,v 1.4 2002/10/14 15:15:57 burnett Exp $
*/

#include "GaudiKernel/DeclareFactoryEntries.h"

DECLARE_FACTORY_ENTRIES(GuiSvc) {
    DECLARE_SERVICE( GuiSvc);
    DECLARE_ALGORITHM(GuiPause);
} 

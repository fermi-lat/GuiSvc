/** 
* @file GuiSvc_load.cxx
* @brief This is needed for forcing the linker to load all components
* of the library.
*
*  $Header: /nfs/slac/g/glast/ground/cvs/GuiSvc/src/GuiSvc_load.cxx,v 1.3 2002/03/15 18:55:20 heather Exp $
*/

#include "GaudiKernel/DeclareFactoryEntries.h"

DECLARE_FACTORY_ENTRIES(GuiSvc) {
    DECLARE_SERVICE( GuiSvc);
} 

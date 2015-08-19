#ifndef __HNODE_H__
#define __HNODE_H__

#include <string>
#include <list>
#include <vector>
#include <map>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "hnode-pktsrc.h"
#include "hnode-uid.h"
#include "hnode-provider.h"

class HNode
{
    private:

        GHNodeProvider *provider;

        std::string  id;


    public:
        HNode();
       ~HNode();

        void setProvider( GHNodeProvider *provider );
        GHNodeProvider *getProvider();

        std::string getID();
        std::string getAddressStr();
        bool getVersion( guint32 &major, guint32 &minor, guint32 &micro );
        std::string getVersionStr();

        guint32 getEndpointCount();
        std::string getEndpointCountStr();

        bool getEndpointAddressStr( guint32 Index, std::string &addrStr );
        bool getEndpointVersion( guint32 Index, guint32 &major, guint32 &minor, guint32 &micro );
        bool getEndpointVersionStr( guint32 Index, std::string &verStr );
        bool getEndpointAssociateIndex( guint32 Index, guint32 &assocIndex );
        bool getEndpointTypeStr( guint32 Index, std::string &typeStr );
};

#endif // __HNODE_H__


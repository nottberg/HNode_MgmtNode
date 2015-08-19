#ifndef __HNODEMANAGER_H__
#define __HNODEMANAGER_H__

#include <string>
#include <list>
#include <vector>
#include <map>

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "hnode-pktsrc.h"
#include "hnode-provider.h"
#include "hnode-srvobj.h"

#include "HNode.hpp"

class HNodeManager
{
    private:

        GHNodeServer *srvObj; 

        std::string cfgPath;

        std::vector<HNode *> hnodeList;

        bool getAttribute( xmlNode *elem, std::string attrName, std::string &result );

    public:
        HNodeManager();
       ~HNodeManager();

        void setServerObject( GHNodeServer *Server );

        void setConfigurationPath( std::string cfgPath );
        bool loadConfiguration();

        void addProvider( GHNodeProvider *provider );
        void removeProvider( GHNodeProvider *provider );

        int getHNodeCount();
        HNode *getHNodeByIndex( int index );

        HNode *getHNodeByID( std::string HNodeID );

        bool start();
};

#endif // __HNODEMANAGER_H__


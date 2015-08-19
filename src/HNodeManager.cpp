#include <string>
#include <list>
#include <vector>
#include <map>
#include <string.h>

#include "HNode.hpp"
#include "HNodeManager.hpp"

HNodeManager::HNodeManager()
{
    cfgPath = "/etc/hnode";
}

HNodeManager::~HNodeManager()
{
}

void 
HNodeManager::setServerObject( GHNodeServer *Server )
{
    srvObj = Server;
}

void 
HNodeManager::setConfigurationPath( std::string cfgPathStr )
{
    cfgPath = cfgPathStr;
}

bool
HNodeManager::getAttribute( xmlNode *elem, std::string attrName, std::string &result )
{
    xmlChar *attrValue;
    
    // Start with a clear result
    result.clear();

    // Get the type attribute
    attrValue = xmlGetProp( elem, (xmlChar *)attrName.c_str() );

    if( attrValue == NULL )
    {
        return true;
    }    

    result = (char*)attrValue;

    xmlFree( attrValue );

    return false;
}

bool 
HNodeManager::loadConfiguration()
{
    xmlDocPtr   doc;
    xmlNode    *rootElem;
    xmlNode    *devListElem;
    xmlNode    *devElem;
    xmlNode    *gpioElem;
    xmlNode    *curElem;

    std::string filePath;
#if 0
    filePath = cfgPath + "/switch/hnode_switch_config.xml";

    doc = xmlReadFile( filePath.c_str(), NULL, 0 );
    if (doc == NULL) 
    {
        fprintf( stderr, "Failed to parse %s\n", filePath.c_str() );
	    return true;
    }

    // Get the root element for the document
    rootElem = xmlDocGetRootElement( doc );

    // Make sure it has the expected root tag
    if( strcmp( (char *)rootElem->name, "hnode-switch-cfg" ) != 0 )
    {
        fprintf( stderr, "Root tag didn't match expected: %s\n", rootElem->name );
	    return true;
    } 

    // Find the device list element
    devListElem = NULL;
    for( curElem = rootElem->children; curElem; curElem = curElem->next ) 
    {
        if( (curElem->type == XML_ELEMENT_NODE) && (strcmp( (char *)curElem->name, "switch-device-list" ) == 0) ) 
        {
            printf("devListNode found\n");
            devListElem = curElem;
            break;
        }
    }

    if( devListElem == NULL )
    {
        fprintf( stderr, "Could not find devlist element\n" );
	    return true;
    } 

    // Parse the elements under the device list
    for( curElem = devListElem->children; curElem; curElem = curElem->next )
    {
        // We are only interested in the elements at this level
        if( curElem->type != XML_ELEMENT_NODE )
            continue;

        // Check for an i2c expander
        if( strcmp( (char *)curElem->name, "i2c-expander" ) == 0 )
        {
            addI2CExpander( doc, curElem );
        }
    }


    // Free the config document
    xmlFreeDoc(doc);
#endif
    return false;
}

bool 
HNodeManager::start()
{
#if 0
    HNode *hnode1 = new HNode();
    HNode *hnode2 = new HNode();

    hnode1->setID("01:12:13:14:15:16:17");
    hnode2->setID("02:12:13:14:15:16:17");

    hnodeList.push_back( hnode1 );
    hnodeList.push_back( hnode2 );
#endif

    // Done with startup
    return false;
}

void 
HNodeManager::addProvider( GHNodeProvider *provider )
{
    HNode *hnode = new HNode();

    hnode->setProvider( provider );

    hnodeList.push_back( hnode );
}

void
HNodeManager::removeProvider( GHNodeProvider *provider )
{

}

int 
HNodeManager::getHNodeCount()
{
    return hnodeList.size(); //g_hnode_server_get_provider_count( srvObj );
}

HNode *
HNodeManager::getHNodeByIndex( int index )
{
    return hnodeList[ index ];
}

HNode *
HNodeManager::getHNodeByID( std::string hnodeID )
{
    HNode *hnodeObj = NULL;

    // Search through the switch list for the right ID
    for( std::vector<HNode *>::iterator it = hnodeList.begin() ; it != hnodeList.end(); ++it)
    {
        if( hnodeID == (*it)->getID() )
        {
            hnodeObj = *it;
        }
    }

    return hnodeObj;
}



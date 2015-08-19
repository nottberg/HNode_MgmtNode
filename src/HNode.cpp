#include <sys/time.h>

#include "HNode.hpp"

HNode::HNode()
{
}

HNode::~HNode()
{

}

void 
HNode::setProvider( GHNodeProvider *newProvider )
{
    provider = newProvider;
}

GHNodeProvider *
HNode::getProvider()
{
    return provider;
}

std::string 
HNode::getID()
{
    GHNodeUID *uidObj;
    gchar uidStr[256];
    std::string resultStr;

    uidObj = g_hnode_provider_get_uid_objptr( provider );

    if( g_hnode_uid_get_uid_as_string( uidObj, uidStr ) )
    {
        resultStr = uidStr;
    }

    return resultStr;
}

std::string
HNode::getAddressStr()
{
    GHNodeAddress *EPAddr;
    gchar   *aStr;
    guint32  aLen;
    std::string addrStr;

    addrStr.clear();

    if( g_hnode_provider_get_address( provider, &EPAddr ) )
    {
        // Error
        return addrStr;
    }

    g_hnode_address_get_str_ptr( EPAddr, &aStr, &aLen );

    addrStr.assign( aStr, aLen );

    // Success
    return addrStr;
}

bool
HNode::getVersion( guint32 &major, guint32 &minor, guint32 &micro )
{
    major = 0;
    minor = 0;
    micro = 0;

    return g_hnode_provider_get_version( provider, &major, &minor, &micro );
}

std::string
HNode::getVersionStr()
{
    gchar vStr[64];
    guint32 major, minor, micro;
    std::string verStr;

    getVersion( major, minor, micro );

    verStr.clear();

    sprintf( vStr, "%d.%d.%d", major, minor, micro );

    verStr = vStr;

    // Success
    return verStr;
}

guint32 
HNode::getEndpointCount()
{
   guint32 epCount;

   if( g_hnode_provider_get_endpoint_count( provider, &epCount ) )
   {
       return 0;
   }  
   else
   {
       return epCount;
   }
}

std::string 
HNode::getEndpointCountStr()
{
   gchar tmpStr[32];
   guint32 epCount;
   std::string result;

   if( g_hnode_provider_get_endpoint_count( provider, &epCount ) )
   {
       result = "0";       
   }  
   else
   {
       sprintf(tmpStr, "%d", epCount);
       result = tmpStr;
   }

   return result;
}

bool
HNode::getEndpointAddressStr( guint32 Index, std::string &addrStr )
{
    GHNodeAddress *EPAddr;
    gchar   *aStr;
    guint32  aLen;

    addrStr.clear();

    if( g_hnode_provider_endpoint_get_address( provider, Index, &EPAddr ) )
    {
        // Error
        return true;
    }

    g_hnode_address_get_str_ptr( EPAddr, &aStr, &aLen );

    addrStr.assign( aStr, aLen );

    // Success
    return false;
}

bool
HNode::getEndpointVersion( guint32 Index, guint32 &major, guint32 &minor, guint32 &micro )
{
    major = 0;
    minor = 0;
    micro = 0;

    return g_hnode_provider_endpoint_get_version( provider, Index, &major, &minor, &micro );
}

bool
HNode::getEndpointVersionStr( guint32 Index, std::string &verStr )
{
    gchar vStr[64];
    guint32 major, minor, micro;

    getEndpointVersion( Index, major, minor, micro );

    verStr.clear();

    sprintf( vStr, "%d.%d.%d", major, minor, micro );

    verStr = vStr;

    // Success
    return false;
}

bool
HNode::getEndpointAssociateIndex( guint32 Index, guint32 &assocIndex )
{
    return g_hnode_provider_endpoint_get_associate_index( provider, Index, &assocIndex );
}

bool
HNode::getEndpointTypeStr( guint32 Index, std::string &typeStr )
{
    gchar *tStr;

    typeStr.clear();

    if( g_hnode_provider_endpoint_get_type_str( provider, Index, &tStr ) )
    {
        // Error
        return true;
    }

    typeStr = tStr;

    // Success
    return false;
}




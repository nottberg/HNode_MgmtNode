#include <string>
#include <iostream>
#include <fstream>
#include <curl/curl.h>
using namespace std;

#include "hnode-rest.hpp"
#include "HNodeManager.hpp"
#include "HNodeResource.hpp"

HNodeListResource::HNodeListResource()
{
    setURLPattern( "/hnode", (REST_RMETHOD_T)(REST_RMETHOD_GET) );
}

HNodeListResource::~HNodeListResource()
{

}

void
HNodeListResource::setHNodeManager( HNodeManager *hnMgr )
{
    hnodeManager = hnMgr;
}

void 
HNodeListResource::restGet( RESTRequest *request )
{
    std::string rspData;

    std::cout << "HNodeListResource::restGet" << std::endl;

    rspData = "<hnode-list>";

    printf( "HNodeListResource: swManager - 0x%x\n", hnodeManager );

    if( hnodeManager )
    {
        for( int index = 0; index < hnodeManager->getHNodeCount(); index++ )
        {
            HNode *hnodeObj = hnodeManager->getHNodeByIndex( index );
            printf( "Output HNode: %d\n", index );
            rspData += "<hnodeid>" + hnodeObj->getID() + "</hnodeid>";
        }
    }

    rspData += "</hnode-list>"; 

    RESTRepresentation *rspRep = request->getOutboundRepresentation();
    rspRep->setSimpleContent( "application/xml", (unsigned char *) rspData.c_str(), rspData.size() );

    request->setResponseCode( REST_HTTP_RCODE_OK );
    request->sendResponse();
}

HNodeResource::HNodeResource()
{
    setURLPattern( "/hnode/{hnodeid}", (REST_RMETHOD_T)(REST_RMETHOD_GET) ); 
}

HNodeResource::~HNodeResource()
{

}

void
HNodeResource::setHNodeManager( HNodeManager *hnodeMgr )
{
    hnodeManager = hnodeMgr;
}

void 
HNodeResource::restGet( RESTRequest *request )
{
    std::string hnodeID;
    std::string rspData;
    gchar tmpStr[256];
    std::string tmpData;

    HNode *hnodeObj;
    guint32 epCnt;
  
    if( request->getURIParameter( "hnodeid", hnodeID ) )
    {
        printf("Failed to look up hnodeid parameter\n");
        request->setResponseCode( REST_HTTP_RCODE_BAD_REQUEST );
        request->sendResponse();
        return;
    }

    printf( "URL HNodeID: %s\n", hnodeID.c_str() );

    hnodeObj = hnodeManager->getHNodeByID( hnodeID );

    if( hnodeObj == NULL )
    {
        request->setResponseCode( REST_HTTP_RCODE_NOT_FOUND );
        request->sendResponse();
        return;
    }

    rspData = "<hnode id=\"";
    rspData += hnodeID;
    rspData += "\">";

    rspData += "<address>" + hnodeObj->getAddressStr() + "</address>";
    rspData += "<version>" + hnodeObj->getVersionStr() + "</version>";

    rspData += "<endpoint-count>" + hnodeObj->getEndpointCountStr() + "</endpoint-count>";

    rspData += "<endpoint-list>";
    epCnt = hnodeObj->getEndpointCount();
    for( guint32 index = 0; index < epCnt; index++ )
    {
        sprintf(tmpStr, "%d", index);
        rspData += "<endpoint index=\"";
        rspData += tmpStr;
        rspData += "\">";

        if( hnodeObj->getEndpointAddressStr( index, tmpData ) == false )
        {
            rspData += "<address>" + tmpData + "</address>";
        }

        if( hnodeObj->getEndpointVersionStr( index, tmpData ) == false )
        {
            rspData += "<version>" + tmpData + "</version>";
        }

        //if( hnodeObj->getEndpointAssociateIndex( index, guint32 &assocIndex ) == false )
        //{
        //    rspData = "<address>" + tmpData + "</address>";
        //}

        if( hnodeObj->getEndpointTypeStr( index, tmpData ) == false )
        {
            rspData += "<type>" + tmpData + "</type>";
        }

        rspData += "</endpoint>";  
    }

    rspData += "</endpoint-list>";

#if 0
    rspData += "<name>";
    rspData += swObj->getName();
    rspData += "</name>";

    rspData += "<desc>";
    rspData += swObj->getDescription();
    rspData += "</desc>";

    rspData += "<state>";
    if( swObj->isStateOn() )
        rspData += "on";
    else
        rspData += "off";
    rspData += "</state>";
#endif
    rspData += "</hnode>";

    RESTRepresentation *rspRep = request->getOutboundRepresentation();
    rspRep->setSimpleContent( "application/xml", (unsigned char *) rspData.c_str(), rspData.size() );

    request->setResponseCode( REST_HTTP_RCODE_OK );
    request->sendResponse();
}

HNodeUIResource::HNodeUIResource()
{
    setURLPattern( "/hnode/{hnodeid}/ui/*", (REST_RMETHOD_T)(REST_RMETHOD_GET) ); 
}

HNodeUIResource::~HNodeUIResource()
{

}

void
HNodeUIResource::setHNodeManager( HNodeManager *hnodeMgr )
{
    hnodeManager = hnodeMgr;
}

void 
HNodeUIResource::restGet( RESTRequest *request )
{
    std::string hnodeID;
    std::string rspData;
    gchar tmpStr[256];
    std::string tmpData;
    std::string htmlRoot = "/var/hmnode/html/";
    std::string filePath;
    streampos size;
    char * memblock;

    HNode *hnodeObj;
    guint32 epCnt;
  
    if( request->getURIParameter( "hnodeid", hnodeID ) )
    {
        printf("Failed to look up hnodeid parameter\n");
        request->setResponseCode( REST_HTTP_RCODE_BAD_REQUEST );
        request->sendResponse();
        return;
    }

    printf( "URL HNodeID: %s\n", hnodeID.c_str() );

    hnodeObj = hnodeManager->getHNodeByID( hnodeID );

    if( hnodeObj == NULL )
    {
        request->setResponseCode( REST_HTTP_RCODE_NOT_FOUND );
        request->sendResponse();
        return;
    }

    std::string extraURL = request->getWildcardURL();

    if( extraURL.empty() )
    {
        filePath = htmlRoot + "irrigation_ui.html"; 
    }
    else
    {
        filePath = htmlRoot + extraURL;
    }

    cout << "HNode UI file open: " << filePath << std::endl;

    ifstream file( filePath.c_str(), ios::in|ios::ate );
    if (file.is_open() == false )
    {
        cout << "Unable to open file";
        request->setResponseCode( REST_HTTP_RCODE_NOT_FOUND );
        request->sendResponse();
        return;
    }

    size = file.tellg();
    memblock = new char [size];
    file.seekg (0, ios::beg);
    file.read (memblock, size);
    file.close();

    cout << "the entire file content is in memory" << std::endl;

    RESTRepresentation *rspRep = request->getOutboundRepresentation();
    rspRep->setSimpleContent( "text/html", (unsigned char *)memblock, size );

    request->setResponseCode( REST_HTTP_RCODE_OK );
    request->sendResponse();

    delete[] memblock;

    return;


}

HNodeProxyResource::HNodeProxyResource()
{
    setURLPattern( "/hnode/{hnodeid}/ui/proxy/*", (REST_RMETHOD_T)(REST_RMETHOD_GET | REST_RMETHOD_POST | REST_RMETHOD_PUT | REST_RMETHOD_DELETE) ); 
}

HNodeProxyResource::~HNodeProxyResource()
{

}

void
HNodeProxyResource::setHNodeManager( HNodeManager *hnodeMgr )
{
    hnodeManager = hnodeMgr;
}

#if 0
size_t
HNodeProxyResource::recvCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  RESTRequest *request = (RESTRequest *)userp;
  size_t realsize = size * nmemb;
 
  RESTRepresentation *rspRep = request->getOutboundRepresentation();
  rspRep->appendSimpleContent( (unsigned char *)contents, realsize );
 
  return realsize;
}
#endif

void 
HNodeProxyResource::restGet( RESTRequest *request )
{
    std::string hnodeID;
    std::string rspData;
    gchar tmpStr[256];
    std::string tmpData;

    HNode *hnodeObj;
    guint32 epCnt;
    CURL *curl;
    CURLcode res;
  
    if( request->getURIParameter( "hnodeid", hnodeID ) )
    {
        printf("Failed to look up hnodeid parameter\n");
        request->setResponseCode( REST_HTTP_RCODE_BAD_REQUEST );
        request->sendResponse();
        return;
    }

    printf( "URL HNodeID: %s\n", hnodeID.c_str() );

    hnodeObj = hnodeManager->getHNodeByID( hnodeID );

    if( hnodeObj == NULL )
    {
        request->setResponseCode( REST_HTTP_RCODE_NOT_FOUND );
        request->sendResponse();
        return;
    }

    std::string hnodeAddr; 
    hnodeObj->getEndpointAddressStr( 1, hnodeAddr );

    RESTHttpClient client;
    client.setHost( hnodeAddr.substr(8, std::string::npos) );
    client.setRequest( RHC_REQTYPE_GET, request->getWildcardURL() );

    client.getOutboundRepresentation().setFromRepresentation( request->getInboundRepresentation() );

    client.makeRequest();

    request->getOutboundRepresentation()->setFromRepresentation( &client.getInboundRepresentation() ); 

    request->setResponseCode( REST_HTTP_RCODE_OK );
    request->sendResponse();

#if 0
    std::string proxyURL  = "http://" + hnodeAddr.substr(8, std::string::npos) + request->getWildcardURL(); 

    if( request->getParameterCount() )
    {
        proxyURL = proxyURL + "?" + request->getParametersAsQueryStr();
    }

    printf( "Proxy GET request to: %s\n", proxyURL.c_str() );
 
    curl = curl_easy_init();
    if( curl == NULL ) 
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        request->setResponseCode( REST_HTTP_RCODE_BAD_REQUEST );
        request->sendResponse();
        return;
    }
    
    curl_easy_setopt( curl, CURLOPT_URL, proxyURL.c_str() );
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HNodeProxyResource::recvCallback );
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)request );

    res = curl_easy_perform(curl);

    if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        request->setResponseCode( REST_HTTP_RCODE_BAD_REQUEST );
        request->sendResponse();
        curl_easy_cleanup(curl);

        return;
    }

    curl_easy_cleanup(curl);
#endif

}

void 
HNodeProxyResource::restPost( RESTRequest *request )
{
    std::string hnodeID;
    HNode *hnodeObj;
  
    if( request->getURIParameter( "hnodeid", hnodeID ) )
    {
        printf("Failed to look up hnodeid parameter\n");
        request->setResponseCode( REST_HTTP_RCODE_BAD_REQUEST );
        request->sendResponse();
        return;
    }

    printf( "URL HNodeID: %s\n", hnodeID.c_str() );

    hnodeObj = hnodeManager->getHNodeByID( hnodeID );

    if( hnodeObj == NULL )
    {
        request->setResponseCode( REST_HTTP_RCODE_NOT_FOUND );
        request->sendResponse();
        return;
    }

    std::string hnodeAddr; 
    hnodeObj->getEndpointAddressStr( 1, hnodeAddr );

    RESTHttpClient client;
    client.setHost( hnodeAddr.substr(8, std::string::npos) );
    client.setRequest( RHC_REQTYPE_POST, request->getWildcardURL() );

    client.getOutboundRepresentation().setFromRepresentation( request->getInboundRepresentation() );

    client.makeRequest();

    request->getOutboundRepresentation()->setFromRepresentation( &client.getInboundRepresentation() ); 

    request->setResponseCode( REST_HTTP_RCODE_OK );
    request->sendResponse();
}

void 
HNodeProxyResource::restPut( RESTRequest *request )
{
    std::string hnodeID;
    std::string rspData;
    gchar tmpStr[256];
    std::string tmpData;

    HNode *hnodeObj;
    guint32 epCnt;
    CURL *curl;
    CURLcode res;
  
    if( request->getURIParameter( "hnodeid", hnodeID ) )
    {
        printf("Failed to look up hnodeid parameter\n");
        request->setResponseCode( REST_HTTP_RCODE_BAD_REQUEST );
        request->sendResponse();
        return;
    }

    printf( "URL HNodeID: %s\n", hnodeID.c_str() );

    hnodeObj = hnodeManager->getHNodeByID( hnodeID );

    if( hnodeObj == NULL )
    {
        request->setResponseCode( REST_HTTP_RCODE_NOT_FOUND );
        request->sendResponse();
        return;
    }

    std::string hnodeAddr; 
    hnodeObj->getEndpointAddressStr( 1, hnodeAddr );

    RESTHttpClient client;
    client.setHost( hnodeAddr.substr(8, std::string::npos) );
    client.setRequest( RHC_REQTYPE_PUT, request->getWildcardURL() );

    client.getOutboundRepresentation().setFromRepresentation( request->getInboundRepresentation() );
    //client.getOutboundRepresentation().addHTTPHeader( "Accept", "*/*" );

    client.makeRequest();

    request->getOutboundRepresentation()->setFromRepresentation( &client.getInboundRepresentation() ); 

    request->setResponseCode( REST_HTTP_RCODE_OK );
    request->sendResponse();

#if 0
    std::string proxyURL  = "http://" + hnodeAddr.substr(8, std::string::npos) + request->getWildcardURL(); 

#if 0
    if( request->getParameterCount() )
    {
        proxyURL = proxyURL + "?" + request->getParametersAsQueryStr();
    }
#endif

    printf( "Proxy PUT request to: %s\n", proxyURL.c_str() );
 
    curl = curl_easy_init();
    if( curl == NULL ) 
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        request->setResponseCode( REST_HTTP_RCODE_BAD_REQUEST );
        request->sendResponse();
        return;
    }

    struct curl_slist *headers=NULL;
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, "Content-Length: 0");
    headers = curl_slist_append(headers, "Content-Type: application/xml");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt( curl, CURLOPT_URL, proxyURL.c_str() );
    curl_easy_setopt( curl, CURLOPT_PUT, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, 0);

    //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HNodeProxyResource::recvCallback );
    //curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)request );

    res = curl_easy_perform(curl);

    if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        request->setResponseCode( REST_HTTP_RCODE_BAD_REQUEST );
        request->sendResponse();
        curl_easy_cleanup(curl);

        return;
    }

    curl_slist_free_all(headers); /* free the header list */

    curl_easy_cleanup(curl);

    printf( "Proxy PUT complete\n" );

    request->setResponseCode( REST_HTTP_RCODE_OK );
    request->sendResponse();
#endif
}

void 
HNodeProxyResource::restDelete( RESTRequest *request )
{
    std::string hnodeID;
    HNode *hnodeObj;
  
    if( request->getURIParameter( "hnodeid", hnodeID ) )
    {
        printf("Failed to look up hnodeid parameter\n");
        request->setResponseCode( REST_HTTP_RCODE_BAD_REQUEST );
        request->sendResponse();
        return;
    }

    printf( "URL HNodeID: %s\n", hnodeID.c_str() );

    hnodeObj = hnodeManager->getHNodeByID( hnodeID );

    if( hnodeObj == NULL )
    {
        request->setResponseCode( REST_HTTP_RCODE_NOT_FOUND );
        request->sendResponse();
        return;
    }

    std::string hnodeAddr; 
    hnodeObj->getEndpointAddressStr( 1, hnodeAddr );

    RESTHttpClient client;
    client.setHost( hnodeAddr.substr(8, std::string::npos) );
    client.setRequest( RHC_REQTYPE_DELETE, request->getWildcardURL() );

    client.getOutboundRepresentation().setFromRepresentation( request->getInboundRepresentation() );

    client.makeRequest();

    request->getOutboundRepresentation()->setFromRepresentation( &client.getInboundRepresentation() ); 

    request->setResponseCode( REST_HTTP_RCODE_OK );
    request->sendResponse();
}


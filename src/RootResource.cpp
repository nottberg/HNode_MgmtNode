#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "hnode-rest.hpp"
#include "RootResource.hpp"

RootResource::RootResource()
{
    setURLPattern( "/*", (REST_RMETHOD_T)(REST_RMETHOD_GET) ); 
}

RootResource::~RootResource()
{

}

void 
RootResource::restGet( RESTRequest *request )
{

    //std::string swID;
    //std::string rspData;
    std::string htmlRoot = "/var/hmnode/html/";
    std::string filePath;
    streampos size;
    char * memblock;

    cout << "RootResource::restGet -- start" << std::endl;

    // Get the wildcard part of the URL
    std::string wildcardURL = request->getWildcardURL();
    cout << "wildcard URL: " << wildcardURL << std::endl;

    if( wildcardURL.empty() )
    {
        filePath = htmlRoot + "index.html"; 
    }
    else
    {
        filePath = htmlRoot + wildcardURL;
    }

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

#if 0
    Switch *swObj;

    if( request->getParameter( "switchid", swID ) )
    {
        printf("Failed to look up switchid parameter\n");
        request->setResponseCode( REST_HTTP_RCODE_BAD_REQUEST );
        request->sendResponse();
        return;
    }

    printf( "URL SwitchID: %s\n", swID.c_str() );

    swObj = swManager->getSwitchByID( swID );

    if( swObj == NULL )
    {
        request->setResponseCode( REST_HTTP_RCODE_NOT_FOUND );
        request->sendResponse();
        return;
    }
#endif
    //rspData = "<html><body>";
    //rspData += "hi!!!!";
    //rspData += "</body></html>";
}


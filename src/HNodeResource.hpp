#ifndef __HNODERESOURCE_H__
#define __HNODERESOURCE_H__

#include "hnode-rest.hpp"
#include "HNode.hpp"
#include "HNodeManager.hpp"

class HNodeListResource : public RESTResource
{
    private:
        HNodeManager *hnodeManager;

    public:
        HNodeListResource();
       ~HNodeListResource();

        void setHNodeManager( HNodeManager *hnodeMgr );

        virtual void restGet( RESTRequest *request );
};

class HNodeResource : public RESTResource
{
    private:
        HNodeManager *hnodeManager;

    public:
        HNodeResource();
       ~HNodeResource();

        void setHNodeManager( HNodeManager *hnodeMgr );

        virtual void restGet( RESTRequest *request );
};

class HNodeUIResource : public RESTResource
{
    private:
        HNodeManager *hnodeManager;

    public:
        HNodeUIResource();
       ~HNodeUIResource();

        void setHNodeManager( HNodeManager *hnodeMgr );

        virtual void restGet( RESTRequest *request );
};

class HNodeProxyResource : public RESTResource
{
    private:
        HNodeManager *hnodeManager;

        static size_t recvCallback(void *contents, size_t size, size_t nmemb, void *userp);

    public:
        HNodeProxyResource();
       ~HNodeProxyResource();

        void setHNodeManager( HNodeManager *hnodeMgr );

        virtual void restGet( RESTRequest *request );
        virtual void restPost( RESTRequest *request );
        virtual void restPut( RESTRequest *request );
        virtual void restDelete( RESTRequest *request );
};


#endif //__HNODERESOURCE_H__

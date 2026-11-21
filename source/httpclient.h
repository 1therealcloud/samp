#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <winsock.h>

#include <static_assert.h>
#pragma pack(push, 1)
//----------------------------------------------------
//
//	HTTP/1.0 Client Procedures Header.
//	(c) 2002-2005 Kye Bitossi
//
//  Version: $Id: httpclient.h,v 1.3 2006/03/20 17:44:19 kyeman Exp $
//
//----------------------------------------------------

#define HTTP_GET 1
#define HTTP_POST 2
#define HTTP_HEAD 3

//----------------------------------------------------

#define MAX_ENTITY_LENGTH 500000

//----------------------------------------------------

#define HTTP_SUCCESS 0
#define HTTP_ERROR_BAD_HOST 1
#define HTTP_ERROR_NO_SOCKET 2
#define HTTP_ERROR_CANT_CONNECT 3
#define HTTP_ERROR_CANT_WRITE 4
#define HTTP_ERROR_CONTENT_TOO_BIG 5
#define HTTP_ERROR_MALFORMED_RESPONSE 6

//----------------------------------------------------

#define CONTENT_TYPE_UNKNOWN 0
#define CONTENT_TYPE_TEXT 1
#define CONTENT_TYPE_HTML 2

//----------------------------------------------------

#define USER_AGENT "SAMP/0.2.0"
#define GET_FORMAT "GET %s HTTP/1.0\r\nAccept: */*\r\nUser-Agent: %s\r\nReferer: http://%s\r\nHost: %s\r\n\r\n"
#define POST_FORMAT                                                                                                                                                                \
    "POST %s HTTP/1.0\r\nAccept: */*\r\nUser-Agent: %s\r\nReferer: http://%s\r\nHost: %s\r\nContent-type: application/x-www-form-urlencoded\r\nContent-length: %u\r\n\r\n%s"
#define HEAD_FORMAT "HEAD %s HTTP/1.0\r\nAccept: */*\r\nUser-Agent: %s\r\nReferer: http://%s\r\nHost: %s\r\n\r\n"

//----------------------------------------------------

typedef struct
{
    unsigned short port; /* remote port */
    int rtype;           /* request type */
    char* host;          /* hostname */
    char* file;          /* GET/POST request file */
    char* data;          /* POST data (if rtype HTTP_POST) */
    char* referer;       /* http referer. */
} HTTP_REQUEST;

//----------------------------------------------------

typedef struct
{
    char* header;
    char* response;
    unsigned int header_len;
    unsigned long response_len;
    unsigned int response_code;
    unsigned int content_type;
} HTTP_RESPONSE;

//----------------------------------------------------

class CHttpClient
{
private:
    int m_iSocket;
    HTTP_REQUEST m_Request;
    HTTP_RESPONSE m_Response;
    int m_iError;

    // 0x000029a0
    bool Connect(char* szHost, int iPort);
    // 0x00002440
    void CloseConnection();
    // 0x00002450
    bool Send(char* szData);
    // 0x00002490
    int Recv(char* szBuffer, int iBufferSize);

    // 0x000024b0
    void InitRequest(int iType, char* szURL, char* szPostData, char* szReferer);
    // 0x00002680
    void HandleEntity();

    // 0x00002a60
    void Process();

public:
    // 0x00002c40
    int ProcessURL(int iType, char* szURL, char* szData, char* szReferer);

    // 0x000023b0
    bool GetHeaderValue(char* szHeaderName, char* szReturnBuffer, int iBufSize);
    // inline
    int GetResponseCode()
    {
        return m_Response.response_code;
    };
    // inline
    int GetContentType()
    {
        return m_Response.content_type;
    };
    // inline
    char* GetResponseHeaders()
    {
        return m_Response.header;
    };
    // inline
    char* GetDocument()
    {
        return m_Response.response;
    };
    // inline
    int GetDocumentLength()
    {
        return m_Response.response_len;
    };

    // 0x000022e0
    CHttpClient();
    // 0x00002340
    ~CHttpClient();
};

//----------------------------------------------------
#pragma pack(pop)
STATIC_ASSERT(sizeof(HTTP_REQUEST) == 22);
STATIC_ASSERT(sizeof(HTTP_RESPONSE) == 24);
STATIC_ASSERT(sizeof(CHttpClient) == 54);

#endif // HTTPCLIENT_H

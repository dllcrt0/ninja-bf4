
#pragma once

#include <xtl.h>
#include <string>
#include <vector>
#include <xhttp.h>
#include <fstream>

#define RECEIVE_BUFFER_SIZE 16384
#define MAX_HOSTNAME_LENGTH 128 
#define MAX_HEADER_LENGTH   16384 
#define MAX_URL_LENGTH      1024 

enum HttpState
{
	HTTP_STATE_IDLE,
	HTTP_STATE_REQUEST_SENT,
	HTTP_STATE_RECEIVE_RESPONSE,
	HTTP_STATE_WAITING_FOR_RESPONSE,
	HTTP_STATE_RESPONSE_RECEIVED,
	HTTP_STATE_WAITING_FOR_DATA,
	HTTP_STATE_READ_COMPLETE,
	HTTP_STATE_RECEIVE_DATA,
	HTTP_STATE_RECEIVED_DATA,
	HTTP_STATE_COMPLETED,
	HTTP_STATE_ERROR,
};

struct AsyncHttpRequest
{
	char ServerName[XHTTP_REQUEST_MAX_HOST];
	char Path[XHTTP_REQUEST_MAX_URL];
	BYTE *Buffer;
	BOOL Https;
	DWORD Size;
	DWORD Port;
};

class HttpRequest
{
public:
	HttpRequest();
	BOOL DownloadFile(CHAR *ServerName, string Path, string SavePath, DWORD dataLength, BOOL https, INTERNET_PORT port = 80);
	BYTE *SendRequest(CHAR *ServerName, string Path, BOOL https, BOOL *success, INTERNET_PORT port = 80, DWORD dataLength = RECEIVE_BUFFER_SIZE);

	void MoveToState(DWORD state);

	AsyncHttpRequest AsyncRequest;
	DWORD dwBytesRead;
	
private:
	static VOID CALLBACK StatusCallback(HINTERNET hInternet,
		DWORD_PTR dwpContext,
		DWORD dwInternetStatus,
		LPVOID lpvStatusInformation,
		DWORD dwStatusInformationSize);
	DWORD GetHTTPStatusCode();
	HRESULT GetResponseStatusCode(
		HINTERNET hRequest,
		DWORD * pdwStatusCode
	);
	HRESULT HttpRequest::GetResponseETagHeader(
		HINTERNET hRequest
	);
	HRESULT GetResponseContentLength(
		HINTERNET hRequest,
		DWORD * pdwContentLength
	);
	HRESULT GetResponseHeader(
		HINTERNET hRequest,
		DWORD dwInfoLevel,
		CONST CHAR * pszHeader,
		VOID * pvBuffer,
		DWORD * pdwBufferLength
	);

	VOID ResetRequest();

	PBYTE receiveBuffer;
	DWORD dwState;
	BOOL isRequestBusy;
	HINTERNET hSession, hConnect, hRequest;
	DWORD dwContentLength;
	DWORD dwBufferLength;
	BOOL hasContentLengthHeader;
	DWORD dwHttpStatusCode;
	DWORD dwContentLengthRemaining;
	DWORD dwLastBytesReceived;
	BOOL bWritePending;
};

vector<PCHAR> GetWebDirectoryListing(PCHAR pServer, string pPath);
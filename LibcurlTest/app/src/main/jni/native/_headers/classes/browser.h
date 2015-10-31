#ifndef BYTE
typedef const char BYTE;
#endif

#ifndef BOOL
typedef short BOOL;
#define TRUE 1
#define FALSE 0
#endif

typedef struct _CURL_DOWNLOAD_OBJECT {
	long size;
	char* data;
} CURL_DOWNLOAD_OBJECT, *LPCURL_DOWNLOAD_OBJECT;

BOOL downloadUrl(CURL* curl, const char* url,
		LPCURL_DOWNLOAD_OBJECT downloadObject);

#ifdef ANDROID
#include <android/log.h>
#include <jni.h>
#ifdef __LP64__
#define SIZE_T_TYPE "%lu"
#else
#define SIZE_T_TYPE "%u"
#endif
#endif
BOOL downloadUrl(string, CURL*, LPCURL_DOWNLOAD_OBJECT, string);
size_t curlCallback(char *data, size_t size, size_t count, void* userdata);
class connection {
private:
	CURL* curl;

	CURLcode res;

	int status;

	int getStatus();

public:
	string response;

	// functions
	connection();
	void endConnection ();
	bool browser(JNIEnv*,jobject, string, string, int);
	void browser_init();
};
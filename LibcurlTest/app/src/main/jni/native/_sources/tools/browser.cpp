#include <com_hazem_apps_libcurltest_Libcurl.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <curl/curl.h>
using std::string;

#include <classes/browser.h>

string tostring(JNIEnv* env, jstring &jstr) {
	if (!jstr) {
		return "";
	}
	const char *tempstring = env->GetStringUTFChars(jstr, NULL);
	string temp = tempstring;
	env->ReleaseStringUTFChars(jstr, tempstring);
	env->DeleteLocalRef(jstr);
	return temp;
}
jstring tojstring(JNIEnv* env, string str) {
	jstring temp = env->NewStringUTF(str.c_str());
	return temp;
}
CURLcode downloadUrl(string url, CURL* curl, int &status,
		LPCURL_DOWNLOAD_OBJECT downloadObject, string param, JNIEnv* env, jobject object ) {
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, param.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, downloadObject);


	CURLcode res = curl_easy_perform(curl);
	return res;
}

size_t curlCallback(char *data, size_t size, size_t count, void* userdata) {

	LPCURL_DOWNLOAD_OBJECT downloadObject = (LPCURL_DOWNLOAD_OBJECT) userdata;
	long newSize = 0;
	long offset = 0;
	char* dataPtr;

	if (downloadObject->data == NULL) {
		newSize = size * count * sizeof(BYTE);
		dataPtr = (char*) malloc(newSize);
	} else {
		newSize = downloadObject->size + (size * count * sizeof(BYTE));
		dataPtr = (char*) realloc(downloadObject->data, newSize);
		offset = downloadObject->size;
	}

	if (dataPtr == NULL) {
		if (downloadObject->data != NULL) {
			free(downloadObject->data);
			downloadObject->data = NULL;
			downloadObject->size = 0;
		}
		return 0;
	}
	downloadObject->data = dataPtr;
	downloadObject->size = newSize;

	memcpy(downloadObject->data + offset, data, size * count * sizeof(BYTE));
	return size * count;
}
connection::connection() {
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlCallback);
}

void connection::endConnection (){
    curl_easy_cleanup(curl);
}

bool connection::browser(JNIEnv* env, jobject object, string url, string param, int expectedStatus) {
	CURL_DOWNLOAD_OBJECT* downloadObject = new CURL_DOWNLOAD_OBJECT;
	downloadObject->data = NULL;
	downloadObject->size = 0;
    res = downloadUrl(url, curl, status, downloadObject, param, env, object);
	if (res == CURLE_OK) {
		if (downloadObject->size == 0){
			response = "err: no respose available or request rejected.";
			return 0;
		}


//    	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
//        if (status != expectedStatus){
//        	return 0;
//        }

        response = downloadObject->data;

		free(downloadObject->data);
		delete downloadObject;
		return 1;
	}
	else if (res == CURLE_COULDNT_CONNECT || res == CURLE_COULDNT_RESOLVE_HOST || res == CURLE_COULDNT_RESOLVE_PROXY)
		response = "err: no internet connection or host is unreachable!";
    else
    	response = "err: bad parameters or requested server is down.";
	return 0;
}

JNIEXPORT jstring JNICALL Java_com_hazem_apps_libcurltest_Libcurl_requestPage
  (JNIEnv * env, jobject object, jstring location, jstring params){

	connection * con = new connection();
	con->browser(env, object, tostring(env, location), tostring(env, params), 0);
	con->endConnection();
	return tojstring(env, con->response);
  }

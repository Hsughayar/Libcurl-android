package com.hazem.apps.libcurltest;

public class Libcurl {

    static {
        System.loadLibrary("native");
    }
    public native String requestPage(String URL, String params);
}

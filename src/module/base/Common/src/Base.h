#ifndef BASE_H
#define BASE_H

#ifdef WIN32
#define RTSP_EXPORT __declspec(dllexport)
#else
#define RTSP_EXPORT
#endif

#endif

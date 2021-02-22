#ifndef RTSP_COMMON_H
#define RTSP_COMMON_H
#include <string>
#include "liveMedia.hh"

#define NEW_SMS(description) do {\
char const* descStr = description\
    ", streamed by the LIVE555 Media Server";\
    std::string stream = createStreamName(mFileName);\
sms = ServerMediaSession::createNew(*mEnvironment, stream.c_str(), stream.c_str(), descStr);\
} while(0)


// Special code for handling Matroska files:
struct MatroskaDemuxCreationState
{
  MatroskaFileServerDemux* demux;
  char watchVariable;
};

// Special code for handling Ogg files:
struct OggDemuxCreationState
{
  OggFileServerDemux* demux;
  char watchVariable;
};

void onMatroskaDemuxCreation(MatroskaFileServerDemux* newDemux, void* clientData);

void onOggDemuxCreation(OggFileServerDemux* newDemux, void* clientData);

// 获取流的名称
std::string createStreamName(std::string url);

#endif

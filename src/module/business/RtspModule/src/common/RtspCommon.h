#ifndef RTSP_COMMON_H
#define RTSP_COMMON_H
#include "liveMedia.hh"

#define NEW_SMS(description) do {\
char const* descStr = description\
    ", streamed by the LIVE555 Media Server";\
sms = ServerMediaSession::createNew(env, fileName, fileName, descStr);\
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

#endif

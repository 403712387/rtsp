#include "RtspCommon.h"

void onMatroskaDemuxCreation(MatroskaFileServerDemux* newDemux, void* clientData)
{
  MatroskaDemuxCreationState* creationState = (MatroskaDemuxCreationState*)clientData;
  creationState->demux = newDemux;
  creationState->watchVariable = 1;
}

void onOggDemuxCreation(OggFileServerDemux* newDemux, void* clientData)
{
  OggDemuxCreationState* creationState = (OggDemuxCreationState*)clientData;
  creationState->demux = newDemux;
  creationState->watchVariable = 1;
}

// 获取流的名称
std::string createStreamName()
{
    static long long index = 0;
    return std::to_string(++index);
}

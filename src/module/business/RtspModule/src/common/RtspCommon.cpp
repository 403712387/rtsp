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

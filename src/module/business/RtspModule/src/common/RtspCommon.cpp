#include "RtspCommon.h"

extern "C"
{
#include "ffmpeg/libavutil/adler32.h""
}

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
std::string createStreamName(std::string url)
{
    long long result = av_adler32_update(0, (const uint8_t *)url.data(), url.size());
    return "LanZongNiubility/" + std::to_string(result);
}

#include "BasicUsageEnvironment.hh"
#include "liveMedia.hh"
#include "ffmpeg_demux.h"
#include "ffmpeg_server_demux.h"
#include "ffmpeg_demuxed_elementary_stream.h"
#include "ffmpeg_h264_server_media_subsession.h"

FFmpegH264ServerMediaSubsession *FFmpegH264ServerMediaSubsession::CreateNew(FFmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_source)
{
    return new FFmpegH264ServerMediaSubsession(demux, stream_id, reuse_source);
}


FFmpegH264ServerMediaSubsession::~FFmpegH264ServerMediaSubsession()
{
    // TODO Auto-generated destructor stub
}

FFmpegH264ServerMediaSubsession::FFmpegH264ServerMediaSubsession(FFmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_source):
    H264VideoFileServerMediaSubsession(demux.envir(), NULL, reuse_source), ffmpeg_demux_(demux),stream_id_(stream_id)
{
    // TODO Auto-generated constructor stub

}

FramedSource *FFmpegH264ServerMediaSubsession::createNewStreamSource(unsigned  clientSessionId, unsigned& estBitrate)
{
    estBitrate = 500;
    FramedSource* es = ffmpeg_demux_.NewElementaryStream(clientSessionId, stream_id_);
    if (es == NULL)
    {
        return NULL;
    }
    return H264VideoStreamFramer::createNew(envir(), es);
}



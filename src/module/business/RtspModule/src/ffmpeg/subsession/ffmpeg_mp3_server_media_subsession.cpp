#include "BasicUsageEnvironment.hh"
#include "liveMedia.hh"
#include "ffmpeg_demux.h"
#include "ffmpeg_server_demux.h"
#include "ffmpeg_demuxed_elementary_stream.h"
#include "ffmpeg_mp3_server_media_subsession.h"


FFmpegMp3ServerMediaSubsession *FFmpegMp3ServerMediaSubsession::CreateNew(FFmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_source)
{
    return new FFmpegMp3ServerMediaSubsession(demux, stream_id, reuse_source);
}

FFmpegMp3ServerMediaSubsession::FFmpegMp3ServerMediaSubsession(FFmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_source)
    : MP3AudioFileServerMediaSubsession(demux.envir() , NULL, reuse_source, False, NULL),ffmpeg_demux_(demux), stream_id_(stream_id)
{

}

FFmpegMp3ServerMediaSubsession::~FFmpegMp3ServerMediaSubsession()
{
    // TODO Auto-generated destructor stub
}

FramedSource *FFmpegMp3ServerMediaSubsession::createNewStreamSource( unsigned  clientSessionId, unsigned  & estBitrate)
{
    FramedSource* source = ffmpeg_demux_.NewElementaryStream(clientSessionId, stream_id_);
    if (source == NULL)
    {
        return NULL;
    }

    return createNewStreamSourceCommon(source, 0/*file size, temporary*/, estBitrate);  //TODO:
}

//we must override seekStreamSource function
void FFmpegMp3ServerMediaSubsession::seekStreamSource(FramedSource* inputSource, double& seekNPT,  double streamDuration, u_int64_t& numBytes)
{
    //TODO: now do nothing
}

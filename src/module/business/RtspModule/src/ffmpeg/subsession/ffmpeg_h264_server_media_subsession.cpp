/*
 * ffmpeg_H264_server_media_subsession.cpp
 *
 *  Created on: 2011-12-8
 *      Author: Liang Guangwei
 */
#include "BasicUsageEnvironment.hh"
#include "liveMedia.hh"
#include "ffmpeg_demux.h"
#include "ffmpeg_server_demux.h"
#include "ffmpeg_demuxed_elementary_stream.h"
#include "ffmpeg_h264_server_media_subsession.h"

FfmpegH264ServerMediaSubsession *FfmpegH264ServerMediaSubsession::CreateNew(
        FfmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_source)
{
    return new FfmpegH264ServerMediaSubsession(demux, stream_id, reuse_source);
}


FfmpegH264ServerMediaSubsession::~FfmpegH264ServerMediaSubsession() {
    // TODO Auto-generated destructor stub
}

FfmpegH264ServerMediaSubsession::FfmpegH264ServerMediaSubsession(
        FfmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_source):
        H264VideoFileServerMediaSubsession(demux.envir(), NULL, reuse_source),
        ffmpeg_demux_(demux),
        stream_id_(stream_id)
{
    // TODO Auto-generated constructor stub

}

FramedSource *FfmpegH264ServerMediaSubsession::createNewStreamSource(
        unsigned  clientSessionId, unsigned& estBitrate)
{
    estBitrate = 500; //kbps��estimate

    FramedSource* es = ffmpeg_demux_.NewElementaryStream(clientSessionId, stream_id_);
    if (es == NULL)  return NULL;

    //return H264VideoStreamDiscreteFramer::createNew(envir(), es);
    return H264VideoStreamFramer::createNew(envir(), es);
}



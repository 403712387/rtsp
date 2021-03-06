﻿#include "BasicUsageEnvironment.hh"
#include "liveMedia.hh"
#include "ffmpeg_demux.h"
#include "ffmpeg_server_demux.h"
#include "ffmpeg_demuxed_elementary_stream.h"
#include "ffmpeg_aac_server_media_subsession.h"

FFmpegAACServerMediaSubession *FFmpegAACServerMediaSubession::CreateNew(
        FFmpegServerDemux & demux, u_int8_t stream_id, Boolean reuse_source)
{
    return new FFmpegAACServerMediaSubession(demux, stream_id, reuse_source);
}

FFmpegAACServerMediaSubession::FFmpegAACServerMediaSubession(
        FFmpegServerDemux & demux, u_int8_t stream_id, Boolean reuse_source):
        FileServerMediaSubsession(demux.envir(), NULL, reuse_source ),
        ffmpeg_demux_(demux),
        stream_id_(stream_id) {
    const StreamInfo* stream = ffmpeg_demux_.GetStreamInfo(stream_id);
    config_str_ = new char[stream->extra_data_size * 2 + 1];
    for(int i = 0; i < stream->extra_data_size; ++i)
    {
        sprintf(&config_str_[2*i], "%02X", stream->extra_data[i]);
    }
}

FFmpegAACServerMediaSubession::~FFmpegAACServerMediaSubession() {
    // TODO Auto-generated destructor stub
}

FramedSource *FFmpegAACServerMediaSubession::createNewStreamSource(
        unsigned clientSessionId, unsigned & estBitrate) {
    estBitrate = 96; // kbps, estimate

    return ffmpeg_demux_.NewElementaryStream(clientSessionId, stream_id_);
}

RTPSink* FFmpegAACServerMediaSubession
::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* /*inputSource*/) {
//  MatroskaTrack* track = fOurDemux.lookup(fTrackNumber);
//  return MPEG4GenericRTPSink::createNew(envir(), rtpGroupsock,
//                                        rtpPayloadTypeIfDynamic,
//                                        track->samplingFrequency,
//                                        "audio", "AAC-hbr", fConfigStr,
//                                        track->numChannels);
    const StreamInfo* stream = ffmpeg_demux_.GetStreamInfo(stream_id_);
    return MPEG4GenericRTPSink::createNew(envir(), rtpGroupsock,
                                            rtpPayloadTypeIfDynamic,
                                            stream->sample_rate,
                                            "audio", "AAC-hbr", config_str_,
                                            stream->channels );
    //TODO:
    return NULL;
}

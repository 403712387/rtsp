#include "liveMedia.hh"
#include <GroupsockHelper.hh> 
#include "ffmpeg_demux.h"
#include "ffmpeg_demuxed_elementary_stream.h"

FFmpegDemuxedElementaryStream *FFmpegDemuxedElementaryStream::CreateNew(UsageEnvironment & env,u_int8_t stream_id, FFmpegDemux& demux,char const* mine_type,unsigned duration)
{
    return new FFmpegDemuxedElementaryStream(env, stream_id, demux, mine_type, duration);
}

FFmpegDemuxedElementaryStream::~FFmpegDemuxedElementaryStream()
{
    ffmpeg_demux_.NoteElementaryStreamDeletion();
}

FFmpegDemuxedElementaryStream::FFmpegDemuxedElementaryStream(UsageEnvironment& env, u_int8_t stream_id,FFmpegDemux& demux, char const* mine_type, unsigned duration)
    :FramedSource(env), ffmpeg_demux_(demux), stream_id_(stream_id), duration_(duration)
{
    mine_type_ = strdup(mine_type);

    // Use the current wallclock time as the base 'presentation time':
    gettimeofday(&fPresentationTime, NULL);  
}

void FFmpegDemuxedElementaryStream::doGetNextFrame()
{
    ffmpeg_demux_.GetNextFrame(stream_id_, fTo, fMaxSize, AfterGettingFrame, this, handleClosure, this);
}

void FFmpegDemuxedElementaryStream::AfterGettingFrame1(unsigned  frame_size,
        unsigned  num_truncated_bytes, struct timeval /*presentation_time*/,
        unsigned  /*duration_in_microseconds*/)
{
    fFrameSize = frame_size;
    fNumTruncatedBytes = num_truncated_bytes;

    //calculate the time
    fPresentationTime.tv_usec += duration_;
    fPresentationTime.tv_sec += fPresentationTime.tv_usec/1000000;
    fPresentationTime.tv_usec %= 1000000;
    fDurationInMicroseconds = duration_;

    FramedSource::afterGetting(this);
}

void FFmpegDemuxedElementaryStream::AfterGettingFrame(void *client_data, unsigned  frame_size, unsigned  num_truncated_bytes, struct timeval presentation_time, unsigned  duration_in_microseconds)
{
    FFmpegDemuxedElementaryStream* stream = (FFmpegDemuxedElementaryStream*)client_data;

    stream->AfterGettingFrame1(frame_size, num_truncated_bytes, presentation_time, duration_in_microseconds);
}

void FFmpegDemuxedElementaryStream::doStopGettingFrames()
{
    //TODO
}

char const* FFmpegDemuxedElementaryStream::MIMEtype() const
{
    return mine_type_;
}





﻿#ifndef FFMPEGDEMUXEDELEMENTARYSTREAM_H_
#define FFMPEGDEMUXEDELEMENTARYSTREAM_H_
class FFmpegDemux;
class FFmpegDemuxedElementaryStream: public FramedSource {
public:
   static FFmpegDemuxedElementaryStream* CreateNew(
            UsageEnvironment& env,
            u_int8_t stream_id,
            FFmpegDemux& demux,
            char const* mine_type,
            unsigned duration);
private:
    virtual ~FFmpegDemuxedElementaryStream();
    FFmpegDemuxedElementaryStream(
            UsageEnvironment& env,
            u_int8_t stream_id,
            FFmpegDemux& demux,
            char const* mine_type,
            unsigned duration);

protected:  //redefined virtual functions
    virtual void doGetNextFrame();
    virtual void doStopGettingFrames();
    virtual char const* MIMEtype() const;

private:
    static void AfterGettingFrame(void* client_data,
                  unsigned frame_size, unsigned num_truncated_bytes,
                  struct timeval presentation_time,
                  unsigned duration_in_microseconds);

    void AfterGettingFrame1(unsigned frame_size, unsigned num_truncated_bytes,
                struct timeval presentation_time,
                unsigned duration_in_microseconds);
private:
    FFmpegDemux &ffmpeg_demux_;
    u_int8_t stream_id_;
    char const* mine_type_;
    unsigned duration_;
};

#endif /* FFMPEGDEMUXEDELEMENTARYSTREAM_H_ */

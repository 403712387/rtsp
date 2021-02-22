#ifndef FFMPEGDEMUX_H_
#define FFMPEGDEMUX_H_
#define MAX_STREAM_NUM 5

class FFmpegDemuxedElementaryStream;
class FFmpegDemux;

struct StreamInfo {
    char const* mine_type;
    unsigned int duration;
    unsigned char* extra_data;
    int extra_data_size;
    unsigned int codec_id;

    //audio parameter
    unsigned int sample_rate;
    unsigned int channels;
};

class FFmpegServerDemux: public Medium{
public:
    static FFmpegServerDemux* CreateNew(UsageEnvironment& env, char const* filename, Boolean reuse_source);

private:
    virtual ~FFmpegServerDemux();
    FFmpegServerDemux(UsageEnvironment& env, char const* filename, Boolean reuse_source);

public:
    FFmpegDemuxedElementaryStream* NewElementaryStream(unsigned client_session_id, u_int8_t stream_id);

    //
    //the follow tow functions called in class FFmpegDemux
    //
    FFmpegDemuxedElementaryStream* NewAudioStream();
    FFmpegDemuxedElementaryStream* NewVideoStream();

    ServerMediaSubsession* NewAudioServerMediaSubsession();
    ServerMediaSubsession* NewVideoServerMediaSubsession();

    char const* MIMEtype(int stream_id);
    const StreamInfo* GetStreamInfo(int stream_id);
private:
    ServerMediaSubsession* NewServerMediaSubsession(unsigned int type);
    //detect stream tags from file
    Boolean DetectedStream();

private:
    FramedSource *input_source_;
    Boolean reuse_source_;
    char const* filename_;

    FFmpegDemux *session0_demux_;
    FFmpegDemux *last_created_demux_;
    unsigned last_client_session_id_;

    int video_stream_id_;
    int audio_stream_id_;
    
    StreamInfo stream_[MAX_STREAM_NUM];
};

#endif /* FFMPEGDEMUX_H_ */

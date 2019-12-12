#ifndef FFMPEG_AAC_SERVER_MEDIA_SUBESSION_H_
#define FFMPEG_AAC_SERVER_MEDIA_SUBESSION_H_

class FfmpegAACServerMediaSubession: public FileServerMediaSubsession{
public:
    static FfmpegAACServerMediaSubession* CreateNew(
            FfmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_source);
private:
    FfmpegAACServerMediaSubession(FfmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_source);
    virtual ~FfmpegAACServerMediaSubession();

protected:  //redefined virtual functions
    virtual FramedSource* createNewStreamSource(unsigned clientSessionId,
            unsigned& estBitrate);
    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic,
            FramedSource* inputSource);

private:
    FfmpegServerDemux& ffmpeg_demux_;
    u_int8_t stream_id_;
    char* config_str_;
};

#endif /* FFMPEG_AAC_SERVER_MEDIA_SUBESSION_H_ */

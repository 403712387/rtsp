#ifndef FFMPEG_AAC_SERVER_MEDIA_SUBESSION_H_
#define FFMPEG_AAC_SERVER_MEDIA_SUBESSION_H_

class FFmpegAACServerMediaSubession: public FileServerMediaSubsession{
public:
    static FFmpegAACServerMediaSubession* CreateNew(
            FFmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_source);
private:
    FFmpegAACServerMediaSubession(FFmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_source);
    virtual ~FFmpegAACServerMediaSubession();

protected:  //redefined virtual functions
    virtual FramedSource* createNewStreamSource(unsigned clientSessionId,
            unsigned& estBitrate);
    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic,
            FramedSource* inputSource);

private:
    FFmpegServerDemux& ffmpeg_demux_;
    u_int8_t stream_id_;
    char* config_str_;
};

#endif /* FFMPEG_AAC_SERVER_MEDIA_SUBESSION_H_ */

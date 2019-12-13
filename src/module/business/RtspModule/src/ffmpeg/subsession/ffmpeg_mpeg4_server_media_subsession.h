#ifndef FFMPEG_MPEG4_SERVER_MEDIA_SUBSESSION_H_
#define FFMPEG_MPEG4_SERVER_MEDIA_SUBSESSION_H_

class FFmpegMPEG4ServerMediaSubsession: public MPEG4VideoFileServerMediaSubsession {
public:
    static FFmpegMPEG4ServerMediaSubsession* CreateNew(
            FFmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_source);

private:
    virtual ~FFmpegMPEG4ServerMediaSubsession();
    FFmpegMPEG4ServerMediaSubsession(
            FFmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_source);

protected:  //redefined virtual functions
    virtual FramedSource* createNewStreamSource(unsigned clientSessionId,
            unsigned& estBitrate);

private:
    FFmpegServerDemux& ffmpeg_demux_;
    u_int8_t stream_id_;
};

#endif /* FFMPEG_MPEG4_SERVER_MEDIA_SUBSESSION_H_ */

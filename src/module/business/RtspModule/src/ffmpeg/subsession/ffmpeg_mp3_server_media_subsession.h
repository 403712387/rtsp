#ifndef FFMPEG_MP3_SERVER_MEDIA_SUBSESSION_H_
#define FFMPEG_MP3_SERVER_MEDIA_SUBSESSION_H_

class FFmpegMp3ServerMediaSubsession: public MP3AudioFileServerMediaSubsession{
public:
    static FFmpegMp3ServerMediaSubsession* CreateNew(
            FFmpegServerDemux& demux,
            u_int8_t stream_id,
            Boolean reuse_source);

private:
    FFmpegMp3ServerMediaSubsession(
            FFmpegServerDemux& demux,
            u_int8_t stream_id,
            Boolean reuse_source);
    virtual ~FFmpegMp3ServerMediaSubsession();

protected:  //redefined virtual functions
    virtual FramedSource* createNewStreamSource(unsigned clientSessionId,
                            unsigned& estBitrate);
    virtual void seekStreamSource(FramedSource* inputSource, double& seekNPT, double /*streamDuration*/, u_int64_t& /*numBytes*/);

private:
    FFmpegServerDemux& ffmpeg_demux_;
    u_int8_t stream_id_;
};

#endif /* FFMPEG_MP3_SERVER_MEDIA_SUBSESSION_H_ */

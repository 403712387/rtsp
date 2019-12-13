#ifndef FFMPEG_H265_SERVER_MEDIA_SUBSESSION_H_
#define FFMPEG_H265_SERVER_MEDIA_SUBSESSION_H_

//though inherit from H264VideoFileServerMediaSubsession, we don't use the function
//about dealing file.
class FfmpegServerDemux;
class FfmpegH265ServerMediaSubsession: public H265VideoFileServerMediaSubsession
{
public:
    static FfmpegH265ServerMediaSubsession* CreateNew(FfmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_sourc);

private:
    virtual ~FfmpegH265ServerMediaSubsession();
    FfmpegH265ServerMediaSubsession(FfmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_sourc);

protected:  //redefined virtual functions
    //
    virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate);
private:
    FfmpegServerDemux& ffmpeg_demux_;
    u_int8_t stream_id_;

};

#endif /* FFMPEG_H264_VIDEO_SERVER_MEDIA_SUBSESSION_H_ */

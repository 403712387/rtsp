#ifndef FFMPEG_H265_SERVER_MEDIA_SUBSESSION_H_
#define FFMPEG_H265_SERVER_MEDIA_SUBSESSION_H_

//though inherit from H264VideoFileServerMediaSubsession, we don't use the function
//about dealing file.
class FFmpegServerDemux;
class FFmpegH265ServerMediaSubsession: public H265VideoFileServerMediaSubsession
{
public:
    static FFmpegH265ServerMediaSubsession* CreateNew(FFmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_sourc);

private:
    virtual ~FFmpegH265ServerMediaSubsession();
    FFmpegH265ServerMediaSubsession(FFmpegServerDemux& demux, u_int8_t stream_id, Boolean reuse_sourc);

protected:  //redefined virtual functions
    //
    virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate);
private:
    FFmpegServerDemux& ffmpeg_demux_;
    u_int8_t stream_id_;

};

#endif /* FFMPEG_H264_VIDEO_SERVER_MEDIA_SUBSESSION_H_ */

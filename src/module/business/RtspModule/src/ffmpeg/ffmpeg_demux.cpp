/*
 * ffmpeg_demux.cpp
 *
 *  Created on: 2011-12-9
 *      Author: Liang Guangwei
 */
#include "BasicUsageEnvironment.hh"
#include "liveMedia.hh"
#include "ffmpeg_demuxed_elementary_stream.h"
#include "ffmpeg_demux.h"
#include <iostream>
#include <cstring>
extern "C"
{
#include "libavutil/avutil.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}


class SavedData {
public:
    SavedData(unsigned char* buf, unsigned size)
        :next_(NULL), data_(buf), data_size_(size), num_bytes_used_(0)
    {
    }
    virtual ~SavedData()
    {
        delete[] data_;
        delete next_;
    }

    SavedData* next_;
    unsigned char* data_;
    unsigned data_size_, num_bytes_used_;
};

////////// FfmpegDemux implementation //////////

FfmpegDemux *FfmpegDemux::CreateNew(UsageEnvironment & env, char const *filename, Boolean reclaim_last_es_dies)
{
    FfmpegDemux* demux = new FfmpegDemux(env, filename, reclaim_last_es_dies);
    if (!demux->InitFfmpeg())
    {
        Medium::close(demux);
        return NULL;
    }
    return demux;
}

FfmpegDemux::FfmpegDemux(UsageEnvironment & env, char const *filename,Boolean reclaim_last_es_dies)
    : Medium(env), num_pending_reads_(0), have_undelivered_data_(False),format_ctx_(NULL)
{
    filename_ = strdup(filename);

    for (unsigned i = 0; i < 1024; ++i)
    {
        output_[i].saved_data_head = output_[i].saved_data_tail = NULL;
        output_[i].is_potentially_readable = False;
        output_[i].is_currently_active = False;
        output_[i].is_currently_awaiting_data = False;
        output_[i].presentation_time.tv_sec = 0;
        output_[i].presentation_time.tv_usec = 0;

        output_[i].data_counts = 0; //this should be delete
    }

    reclaim_last_es_dies_ = reclaim_last_es_dies;
    num_out_es_ = 0;
    h264bsfc =  NULL;
}

FfmpegDemux::~FfmpegDemux()
{
    delete[] filename_;
    ReinitFfmpeg();
    for (unsigned i = 0; i < 1024; ++i)
    {
        delete output_[i].saved_data_head;
    }
}

Boolean FfmpegDemux::InitFfmpeg()
{
    av_register_all();
    if (avformat_open_input(&format_ctx_, filename_, NULL, NULL) != 0)
    {
        return False;
    }

    if (avformat_find_stream_info(format_ctx_,NULL) < 0)
    {
        return False;
    }
    return True;
}

Boolean FfmpegDemux::ReinitFfmpeg()
{
	avformat_close_input(&format_ctx_);
    return True;
}

FfmpegDemuxedElementaryStream *FfmpegDemux::NewElementaryStream(u_int8_t stream_id, char const* mine_type, unsigned duration)
{
    ++num_out_es_;
    output_[stream_id].is_potentially_readable = True;
    return FfmpegDemuxedElementaryStream::CreateNew(envir(), stream_id, *this, mine_type, duration);
}

void FfmpegDemux::GetNextFrame(u_int8_t stream_id, unsigned char* to, unsigned max_size, FramedSource::afterGettingFunc* AfterGettingFunc, void* after_getting_client_data, FramedSource::onCloseFunc* OnCloseFunc, void* on_close_client_data)
{
    // First, check whether we have saved data for this stream id:
    if (UseSavedData(stream_id, to, max_size, AfterGettingFunc, after_getting_client_data)) 
    {
        return;
    }

    // Then save the parameters of the specified stream id:
    RegisterReadInterest(stream_id, to, max_size, AfterGettingFunc, after_getting_client_data, OnCloseFunc, on_close_client_data);

    // Next, if we're the only currently pending read, continue looking for data:
    if (num_pending_reads_ == 1 || have_undelivered_data_)
    {
        have_undelivered_data_ = 0;
        ContinueReadProcessing();
    } // otherwise the continued read processing has already been taken care of
    //TODO:
}

void FfmpegDemux::StopGettingFrames(u_int8_t stream_id_tag)
{
    struct OutputDescriptor& out = output_[stream_id_tag];
    if (out.is_currently_awaiting_data && num_pending_reads_ > 0)
    {
        --num_pending_reads_;
    }

    out.is_currently_active = out.is_currently_awaiting_data = False;
}

Boolean FfmpegDemux::UseSavedData(u_int8_t stream_id_tag, unsigned char *to,unsigned max_size, FramedSource::afterGettingFunc *AfterGettingFunc, void *after_getting_client_data)
{

    struct OutputDescriptor& out = output_[stream_id_tag];
    if (out.saved_data_head == NULL)
    {
        return False;
    }

    unsigned totNumBytesCopied = 0;
    do
    {
        SavedData& savedData = *(out.saved_data_head);
        unsigned char* from = &savedData.data_[savedData.num_bytes_used_];
        unsigned numBytesToCopy = savedData.data_size_ - savedData.num_bytes_used_;

        if (numBytesToCopy > max_size)
        {
            numBytesToCopy = max_size;
        }

        std::memmove(to, from, numBytesToCopy);

        to += numBytesToCopy;
        max_size -= numBytesToCopy;
        out.saved_data_total_size -= numBytesToCopy;
        totNumBytesCopied += numBytesToCopy;
        savedData.num_bytes_used_ += numBytesToCopy;
        if (savedData.num_bytes_used_ == savedData.data_size_)
        {
            out.saved_data_head = savedData.next_;
            if (out.saved_data_head == NULL)
            {
                out.saved_data_tail = NULL;
            }
            savedData.next_ = NULL;
            delete &savedData;

            out.data_counts--;
        }
    } while (0);

    out.is_currently_active = True;
    if (AfterGettingFunc != NULL)
    {
        struct timeval presentationTime;
        presentationTime.tv_sec = 0;
        presentationTime.tv_usec = 0; // should fix #####
        (*AfterGettingFunc)(after_getting_client_data, totNumBytesCopied,
                0 /* numTruncatedBytes */, presentationTime, 0 /* durationInMicroseconds ?????#####*/);
    }
    return True;
}

void FfmpegDemux::ContinueReadProcessing()
{
    while (num_pending_reads_ > 0)
    {
        int acquiredStreamIdTag = Parse();

        if (acquiredStreamIdTag >= 0)
        {
            // We were able to acquire a frame from the input.
            struct OutputDescriptor& newOut = output_[acquiredStreamIdTag];
            newOut.is_currently_awaiting_data = False;
            // indicates that we can be read again
            // (This needs to be set before the 'after getting' call below,
            //  in case it tries to read another frame)

            // Call our own 'after getting' function.  Because we're not a 'leaf'
            // source, we can call this directly, without risking infinite recursion.

            if (newOut.AfterGettingFunc != NULL) {
                (*newOut.AfterGettingFunc)(newOut.after_getting_client_data,
                        newOut.frame_size, 0 /* numTruncatedBytes */,
                        newOut.presentation_time, 0 /* durationInMicroseconds ?????#####*/);

                --num_pending_reads_;
            }
        } else {
            // We were unable to parse a complete frame from the input, because:
            // - we had to read more data from the source stream, or
            // - we found a frame for a stream that was being read, but whose
            //   reader is not ready to get the frame right now, or
            // - the source stream has ended.
            break;
        }
    }
}

void FfmpegDemux::RegisterReadInterest(u_int8_t stream_id_tag,
        unsigned char *to, unsigned max_size,
        FramedSource::afterGettingFunc *AfterGettingFunc,
        void *after_getting_client_data,
        FramedSource::onCloseFunc *OnCloseFunc, void *on_close_client_data) {
    struct OutputDescriptor& out = output_[stream_id_tag];

    // Make sure this stream is not already being read:
    if (out.is_currently_awaiting_data) {
        envir()
                << "MPEG1or2Demux::registerReadInterest(): attempt to read stream id "
                << stream_id_tag << " more than once!\n";
        envir().internalError();
    }

    out.to = to;
    out.max_size = max_size;
    out.AfterGettingFunc = AfterGettingFunc;
    out.after_getting_client_data = after_getting_client_data;
    out.OnCloseFunc = OnCloseFunc;
    out.on_close_client_data = on_close_client_data;
    out.is_currently_active = True;
    out.is_currently_awaiting_data = True;
    // out.frameSize and out.presentationTime will be set when a frame's read

    ++num_pending_reads_;
}

void FfmpegDemux::FlushInput() {
    //TODO
}

void FfmpegDemux::ContinueReadProcessing(void *client_data, unsigned char *ptr,
        unsigned size, struct timeval presentation_time) {
    //TODO
}

void FfmpegDemux::HandleClosure(void *client_data) {
    //TODO
    FfmpegDemux* demux = (FfmpegDemux*) client_data;

    demux->num_pending_reads_ = 0;

    // Tell all pending readers that our source has closed.
    // Note that we need to make a copy of our readers' close functions
    // (etc.) before we start calling any of them, in case one of them
    // ends up deleting this.
    struct {
        FramedSource::onCloseFunc* OnCloseFunc;
        void* on_close_client_data;
    } savedPending[1024];
    unsigned i, numPending = 0;
    for (i = 0; i < 1024; ++i) {
        struct OutputDescriptor& out = demux->output_[i];
        if (out.is_currently_awaiting_data) {
            if (out.OnCloseFunc != NULL) {
                savedPending[numPending].OnCloseFunc = out.OnCloseFunc;
                savedPending[numPending].on_close_client_data
                        = out.on_close_client_data;
                ++numPending;
            }
        }
        delete out.saved_data_head;
        out.saved_data_head = out.saved_data_tail = NULL;
        out.saved_data_total_size = 0;
        out.is_potentially_readable = out.is_currently_active
                = out.is_currently_awaiting_data = False;
    }
    for (i = 0; i < numPending; ++i) {
        (*savedPending[i].OnCloseFunc)(savedPending[i].on_close_client_data);
    }
}

int FfmpegDemux::CopyData(void* dst, int dst_max_size, const void* src,
        int src_size) {
    int num_bytes_to_cpy = src_size;
    if (src_size > dst_max_size) {
        envir() << "FfmpegDemux::DataCopy() error: source buffer size ("
                << src_size
                << ") exceeds max destination buffer size asked for ("
                << dst_max_size << ")\n";
        num_bytes_to_cpy = dst_max_size;
    }
    std::memcpy(dst, src, num_bytes_to_cpy);
    return num_bytes_to_cpy;
}

u_int8_t FfmpegDemux::Parse() {
    AVPacket packet;
    int acquired_stream_id = -1;
    int stream_id = -1;

    do {
        Boolean has_extra_data = False;

        //        if (av_read_frame(format_ctx_, &packet) == 0) {  //read one frame frome file
        if (ReadOneFrame(&packet, has_extra_data) == 0) {
            stream_id = packet.stream_index;
            OutputDescriptor_t& out = output_[stream_id];
            AVCodecContext *codec = format_ctx_->streams[stream_id]->codec;

            if (out.is_currently_awaiting_data) {
                //normal case
                int num_bytes_copy = 0;
                int truncation = 0;

                out.frame_size = 0;

                if (has_extra_data) {
                    num_bytes_copy = CopyData(out.to, out.max_size,
                            codec->extradata, codec->extradata_size);
                    out.frame_size += num_bytes_copy;
                    truncation = codec->extradata_size - num_bytes_copy;

                    if (truncation > 0) {
                        SaveData(stream_id, codec->extradata + num_bytes_copy,
                                truncation);
                    }
                }

                num_bytes_copy = CopyData(out.to + num_bytes_copy, out.max_size
                        - num_bytes_copy, packet.data, packet.size);
                truncation = packet.size - num_bytes_copy;
                if (truncation > 0) {
                    SaveData(stream_id, packet.data + num_bytes_copy,
                            truncation);
                }

                // set out.presentationTime later
                out.frame_size += num_bytes_copy;
                acquired_stream_id = stream_id;

            } else if (out.is_potentially_readable) {
                int extra_data_size = 0;
                int total_size = 0;
                unsigned char* buf = NULL;

                if (has_extra_data) {
                    extra_data_size = codec->extradata_size;
                }
                total_size = extra_data_size + packet.size;
                buf = new unsigned char[total_size];

                if (has_extra_data) {
                    CopyData(buf, extra_data_size, codec->extradata,
                            extra_data_size);
                }
                CopyData(buf + extra_data_size, packet.size, packet.data,
                        packet.size);

                SaveData(stream_id, buf, total_size, True);
            } else if (out.is_currently_active) {
                have_undelivered_data_ = True;
                break;
            }
        }

        av_free_packet(&packet); //must free the packet

    } while (acquired_stream_id == -1);

    return acquired_stream_id;

}

#ifdef NOT_USE_BITSTREAM_FILER
//parse pps and sps from extradata.
//note: this function is temporary, you should use the bitstream filter "h264_mp4toannexb"
int static Convert(unsigned char** extradata, int &extra_size) {
    int size = 0;
    int totalsize = 0;
    unsigned char* tmp = NULL;
    unsigned char* pos = *extradata;
    unsigned char nalu_header[4] = { 0, 0, 0, 1 };
    pos += 6;
    size = (pos[0] << 8) + pos[1];
    totalsize = size + 4;

    tmp = (unsigned char*) realloc(tmp, totalsize);
    memcpy(tmp, nalu_header, 4);
    memcpy(tmp + 4, pos + 2, size);

    //PPS
    pos += 2 + size;
    pos += 1;
    size = (pos[0] << 8) + pos[1];
    totalsize += size + 4;

    tmp = (unsigned char*) realloc(tmp, totalsize);
    memcpy(tmp + totalsize - size - 4, nalu_header, 4);
    memcpy(tmp + totalsize - size, pos + 2, size);

    av_free(*extradata);
    extra_size = totalsize;
    *extradata = tmp;

    return size;
}
#endif

int FfmpegDemux::ParseH264ExtraDataInMp4(int stream_id) {
#ifdef NOT_USE_BITSTREAM_FILER
    Convert(&(format_ctx_->streams[stream_id]->codec->extradata),
            format_ctx_->streams[stream_id]->codec->extradata_size);
#else

    uint8_t *dummy = NULL;
    int dummy_size;
    AVBitStreamFilterContext* bsfc = av_bitstream_filter_init(
            "h264_mp4toannexb");
    AVCodecContext* codec = format_ctx_->streams[stream_id]->codec;
    if(codec == NULL || codec->extradata == NULL)
    {
    	return 0;
    }
    if (bsfc == NULL) {
        envir() << "cannot open the h264_mp4toannexb\n";
        return -1;
    }
    av_bitstream_filter_filter (bsfc, format_ctx_-> streams [stream_id] -> codec, NULL, & dummy, & dummy_size, NULL, 0, 0);
//    av_bitstream_filter_filter(bsfc, codec,NULL, &codec->extradata, &codec->extradata_size, codec->extradata, codec->extradata_size, 0);
    av_bitstream_filter_close(bsfc);
#endif
    return 0;
}

int FfmpegDemux::ReadOneFrame(AVPacket* packet, boolean &has_extra_data) {
    int stream_id = -1;
    char const* extension = strrchr(format_ctx_->filename, '.');

    if (av_read_frame(format_ctx_, packet) != 0) {
        return -1;
    }
    stream_id = packet->stream_index;

    //for mp4, some encoded parameters is stored in extradata
    if (strcmp(extension, ".mp4") == 0) {
        AVCodecContext *codec = NULL;
        codec = format_ctx_->streams[stream_id]->codec;

        if (codec->codec_id == AV_CODEC_ID_H264) {
            //pps and sps
//            const char start_code[4] = { 0, 0, 0, 1 };
#if 0
            _264stream = stream_id;
#endif
            if(h264bsfc  == NULL)
            {
            	h264bsfc =  av_bitstream_filter_init("h264_mp4toannexb");
            }
            av_bitstream_filter_filter(h264bsfc, codec, NULL, &packet->data, &packet->size, packet->data, packet->size, 0);

//            std::memcpy(packet->data, start_code, 4);
//
//            if ((codec->extradata != NULL) && (codec->extradata[0] != 0) && (ParseH264ExtraDataInMp4(
//                    stream_id) == 0)) {
//                has_extra_data = True;
//            }
        } else if (codec->codec_id == AV_CODEC_ID_MPEG4) {
            static boolean first = True;
            if (first) {
                has_extra_data = True;
                first = False;
            }
        }
    }
    return 0;
}

int FfmpegDemux::SaveData(int stream_id, unsigned char* srcbuf, int size,
        boolean reuse_buf /* = False*/) {
    if (size > 0) {
        OutputDescriptor_t& out = output_[stream_id];
        unsigned char* buf = srcbuf;

        if (!reuse_buf) {
            buf = new unsigned char[size];
            std::memcpy(buf, srcbuf, size);
        }

        SavedData* saved_data = new SavedData(buf, size);
        if (out.saved_data_head == NULL) {
            out.saved_data_head = out.saved_data_tail = saved_data;
        } else {
            out.saved_data_tail->next_ = saved_data;
            out.saved_data_tail = saved_data;
        }
        out.data_counts++; //TODO:should be delete
    }
    return 0;
}

void FfmpegDemux::NoteElementaryStreamDeletion() {
    if (--num_out_es_ == 0 && reclaim_last_es_dies_) {
        Medium::close(this);
    }
}


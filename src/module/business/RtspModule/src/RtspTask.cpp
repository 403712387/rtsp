#include <QTime>
#include "RtspTask.h"
#include "RtspCommon.h"

RtspTask::RtspTask(RTSPServer *server, UsageEnvironment *environment)
{
    mRtspServer = server;
    mEnvironment = environment;
}

// 开始任务
bool RtspTask::startTask(std::string file)
{
    mFileName = file;

    // 创建视频流
    mMediaSession = createSession();
    mRtspServer->addServerMediaSession(mMediaSession);
    return NULL != mMediaSession;
}

// 获取流的名称
std::string RtspTask::getStreamName()
{
    std::string result;
    if (NULL != mMediaSession)
    {
        result = mRtspServer->rtspURL(mMediaSession);
    }
    return result;
}

// 结束任务
bool RtspTask::stopTask()
{
    if (NULL == mMediaSession)
    {
        return true;
    }
    mMediaSession->deleteAllSubsessions();
    return true;
}

// 获取文件名称
std::string RtspTask::getFileName()
{
    return mFileName;
}

// 是否发送完毕
bool RtspTask::isFinish()
{
    bool result = true;
    if (NULL != mMediaSession)
    {
        result = (mMediaSession->numSubsessions() == 0);
    }
    return result;
}

Json::Value RtspTask::toJson()
{
    Json::Value result;
    result["file name"] = mFileName;
    result["stream name"] = getStreamName();
    result["birtyday"] = mBirthday.toString("yyyy-MM-dd HH:mm:ss.zzz").toStdString();
    result["current"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz").toStdString();
    result["duration"] = QTime::fromMSecsSinceStartOfDay(mMediaSession->duration() * 1000).toString("HH:mm:ss.zzz").toStdString();
    return result;
}

// 创建session
ServerMediaSession* RtspTask::createSession()
{
  char const* extension = strrchr(mFileName.c_str(), '.');
  if (extension == NULL)
  {
      return NULL;
  }

  ServerMediaSession* sms = NULL;
  Boolean const reuseSource = False;
  if (strcmp(extension, ".aac") == 0)
  {
    // Assumed to be an AAC Audio (ADTS format) file:
    NEW_SMS("AAC Audio");
    sms->addSubsession(ADTSAudioFileServerMediaSubsession::createNew(*mEnvironment, mFileName.c_str(), reuseSource));
  }
  else if (strcmp(extension, ".amr") == 0)
  {
    // Assumed to be an AMR Audio file:
    NEW_SMS("AMR Audio");
    sms->addSubsession(AMRAudioFileServerMediaSubsession::createNew(*mEnvironment, mFileName.c_str(), reuseSource));
  }
  else if (strcmp(extension, ".ac3") == 0)
  {
    // Assumed to be an AC-3 Audio file:
    NEW_SMS("AC-3 Audio");
    sms->addSubsession(AC3AudioFileServerMediaSubsession::createNew(*mEnvironment, mFileName.c_str(), reuseSource));
  }
  else if (strcmp(extension, ".m4e") == 0)
  {
    // Assumed to be a MPEG-4 Video Elementary Stream file:
    NEW_SMS("MPEG-4 Video");
    sms->addSubsession(MPEG4VideoFileServerMediaSubsession::createNew(*mEnvironment, mFileName.c_str(), reuseSource));
  }
  else if (strcmp(extension, ".264") == 0)
  {
    // Assumed to be a H.264 Video Elementary Stream file:
    NEW_SMS("H.264 Video");
    OutPacketBuffer::maxSize = 100000; // allow for some possibly large H.264 frames
    sms->addSubsession(H264VideoFileServerMediaSubsession::createNew(*mEnvironment, mFileName.c_str(), reuseSource));
  }
  else if (strcmp(extension, ".265") == 0)
  {
    // Assumed to be a H.265 Video Elementary Stream file:
    NEW_SMS("H.265 Video");
    OutPacketBuffer::maxSize = 100000; // allow for some possibly large H.265 frames
    sms->addSubsession(H265VideoFileServerMediaSubsession::createNew(*mEnvironment, mFileName.c_str(), reuseSource));
  }
  else if (strcmp(extension, ".mp3") == 0)
  {
    // Assumed to be a MPEG-1 or 2 Audio file:
    NEW_SMS("MPEG-1 or 2 Audio");
    Boolean useADUs = False;
    Interleaving* interleaving = NULL;
    sms->addSubsession(MP3AudioFileServerMediaSubsession::createNew(*mEnvironment, mFileName.c_str(), reuseSource, useADUs, interleaving));
  }
  else if (strcmp(extension, ".mpg") == 0)
  {
    // Assumed to be a MPEG-1 or 2 Program Stream (audio+video) file:
    NEW_SMS("MPEG-1 or 2 Program Stream");
    MPEG1or2FileServerDemux* demux = MPEG1or2FileServerDemux::createNew(*mEnvironment, mFileName.c_str(), reuseSource);
    sms->addSubsession(demux->newVideoServerMediaSubsession());
    sms->addSubsession(demux->newAudioServerMediaSubsession());
  }
  else if (strcmp(extension, ".vob") == 0)
  {
    // Assumed to be a VOB (MPEG-2 Program Stream, with AC-3 audio) file:
    NEW_SMS("VOB (MPEG-2 video with AC-3 audio)");
    MPEG1or2FileServerDemux* demux = MPEG1or2FileServerDemux::createNew(*mEnvironment, mFileName.c_str(), reuseSource);
    sms->addSubsession(demux->newVideoServerMediaSubsession());
    sms->addSubsession(demux->newAC3AudioServerMediaSubsession());
  }
  else if (strcmp(extension, ".ts") == 0)
  {
    // Assumed to be a MPEG Transport Stream file:
    // Use an index file name that's the same as the TS file name, except with ".tsx":
    unsigned indexFileNameLen = strlen(mFileName.c_str()) + 2; // allow for trailing "x\0"
    char* indexFileName = new char[indexFileNameLen];
    sprintf(indexFileName, "%sx", mFileName.c_str());
    NEW_SMS("MPEG Transport Stream");
    sms->addSubsession(MPEG2TransportFileServerMediaSubsession::createNew(*mEnvironment, mFileName.c_str(), indexFileName, reuseSource));
    delete[] indexFileName;
  }
  else if (strcmp(extension, ".wav") == 0)
  {
    // Assumed to be a WAV Audio file:
    NEW_SMS("WAV Audio Stream");
    // To convert 16-bit PCM data to 8-bit u-law, prior to streaming,
    // change the following to True:
    Boolean convertToULaw = False;
    sms->addSubsession(WAVAudioFileServerMediaSubsession::createNew(*mEnvironment, mFileName.c_str(), reuseSource, convertToULaw));
  }
  else if (strcmp(extension, ".dv") == 0)
  {
    // Assumed to be a DV Video file
    // First, make sure that the RTPSinks' buffers will be large enough to handle the huge size of DV frames (as big as 288000).
    OutPacketBuffer::maxSize = 300000;
    NEW_SMS("DV Video");
    sms->addSubsession(DVVideoFileServerMediaSubsession::createNew(*mEnvironment, mFileName.c_str(), reuseSource));
  }
  else if (strcmp(extension, ".mkv") == 0 || strcmp(extension, ".webm") == 0)
  {
    // Assumed to be a Matroska file (note that WebM ('.webm') files are also Matroska files)
    OutPacketBuffer::maxSize = 100000; // allow for some possibly large VP8 or VP9 frames
    NEW_SMS("Matroska video+audio+(optional)subtitles");

    MatroskaDemuxCreationState creationState;
    creationState.watchVariable = 0;
    MatroskaFileServerDemux::createNew(*mEnvironment, mFileName.c_str(), onMatroskaDemuxCreation, &creationState);
    mEnvironment->taskScheduler().doEventLoop(&creationState.watchVariable);

    ServerMediaSubsession* smss;
    while ((smss = creationState.demux->newServerMediaSubsession()) != NULL)
    {
      sms->addSubsession(smss);
    }
  }
  else if (strcmp(extension, ".ogg") == 0 || strcmp(extension, ".ogv") == 0 || strcmp(extension, ".opus") == 0)
  {
    // Assumed to be an Ogg file
    NEW_SMS("Ogg video and/or audio");
    OggDemuxCreationState creationState;
    creationState.watchVariable = 0;
    OggFileServerDemux::createNew(*mEnvironment, mFileName.c_str(), onOggDemuxCreation, &creationState);
    mEnvironment->taskScheduler().doEventLoop(&creationState.watchVariable);

    ServerMediaSubsession* smss;
    while ((smss = creationState.demux->newServerMediaSubsession()) != NULL)
    {
      sms->addSubsession(smss);
    }
  }

  return sms;
}

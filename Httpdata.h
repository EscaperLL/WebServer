#pragma once

#include"Channel.h"
#include"TimeNode.h"

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

enum ProcessState
{
    State_PARSE_URI=1,
    state_PARSE_HEADERS,
    STATE_RECV_BODY,
    STATE_ANALYSIS,
    STATE_FINISH
};
enum URIState {
  PARSE_URI_AGAIN = 1,
  PARSE_URI_ERROR,
  PARSE_URI_SUCCESS,
};

enum HeaderState {
  PARSE_HEADER_SUCCESS = 1,
  PARSE_HEADER_AGAIN,
  PARSE_HEADER_ERROR
};

enum AnalysisState { ANALYSIS_SUCCESS = 1, ANALYSIS_ERROR };

enum ParseState {
  H_START = 0,
  H_KEY,
  H_COLON,
  H_SPACES_AFTER_COLON,
  H_VALUE,
  H_CR,
  H_LF,
  H_END_CR,
  H_END_LF
};

enum ConnectionState { H_CONNECTED = 0, H_DISCONNECTING, H_DISCONNECTED };

enum HttpMethod { METHOD_POST = 1, METHOD_GET, METHOD_HEAD };

enum HttpVersion { HTTP_10 = 1, HTTP_11 };

class MimeType
{
private:
    MimeType(/* args */);
    static void init();
    static std::unordered_map<std::string,std::string> m_mapMime;
public:
   static std::string getMime(const std::string &suffix);

   private:
   static pthread_once_t once_control;
};


class HttpData:public std::enable_shared_from_this<HttpData>
{
  public:
    HttpData(EventLoop* loop,int fd);
    ~HttpData();
    void reset();
    void linkTimer(std::shared_ptr<TimerNode> &timerNode)
    {
      timer_ = timerNode;
    }
  sp_channel getChannel(){
    return channel_;
  }
  EventLoop* getLoop()
  {
    return loop_;
  }
  void newConnection();
  void handleClose();
  private:
    void handleRead();
    void handleWrite();
    void handleConn();
    void handleErr(int fd,int err_num,std::string short_msg);
    URIState parseURI();
    HeaderState parseHeaders();
    AnalysisState analyzeRequest();
  private:
    int fd_;
    std::map<std::string, std::string> headers_;
    bool keepAlive_;
    std::string fileName_;
    HttpVersion httpVersion_;
    ParseState hState_;
    std::string readBuffer_;
    std::string outBuffer_;
    HttpMethod method_;
    EventLoop* loop_;
    sp_channel channel_;
    std::weak_ptr<TimerNode> timer_;
};
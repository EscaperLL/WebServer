#include"Httpdata.h"
#include"EventLoop.h"
#include <sys/stat.h>
const __uint32_t DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
const int DEFAULT_EXPIRED_TIME = 2000;              // ms
const int DEFAULT_KEEP_ALIVE_TIME = 5 * 60 * 1000;  // ms
pthread_once_t MimeType::once_control = PTHREAD_ONCE_INIT;
std::unordered_map<std::string, std::string> MimeType::m_mapMime;
using namespace std;

char favicon[555] = {
    '\x89', 'P',    'N',    'G',    '\xD',  '\xA',  '\x1A', '\xA',  '\x0',
    '\x0',  '\x0',  '\xD',  'I',    'H',    'D',    'R',    '\x0',  '\x0',
    '\x0',  '\x10', '\x0',  '\x0',  '\x0',  '\x10', '\x8',  '\x6',  '\x0',
    '\x0',  '\x0',  '\x1F', '\xF3', '\xFF', 'a',    '\x0',  '\x0',  '\x0',
    '\x19', 't',    'E',    'X',    't',    'S',    'o',    'f',    't',
    'w',    'a',    'r',    'e',    '\x0',  'A',    'd',    'o',    'b',
    'e',    '\x20', 'I',    'm',    'a',    'g',    'e',    'R',    'e',
    'a',    'd',    'y',    'q',    '\xC9', 'e',    '\x3C', '\x0',  '\x0',
    '\x1',  '\xCD', 'I',    'D',    'A',    'T',    'x',    '\xDA', '\x94',
    '\x93', '9',    'H',    '\x3',  'A',    '\x14', '\x86', '\xFF', '\x5D',
    'b',    '\xA7', '\x4',  'R',    '\xC4', 'm',    '\x22', '\x1E', '\xA0',
    'F',    '\x24', '\x8',  '\x16', '\x16', 'v',    '\xA',  '6',    '\xBA',
    'J',    '\x9A', '\x80', '\x8',  'A',    '\xB4', 'q',    '\x85', 'X',
    '\x89', 'G',    '\xB0', 'I',    '\xA9', 'Q',    '\x24', '\xCD', '\xA6',
    '\x8',  '\xA4', 'H',    'c',    '\x91', 'B',    '\xB',  '\xAF', 'V',
    '\xC1', 'F',    '\xB4', '\x15', '\xCF', '\x22', 'X',    '\x98', '\xB',
    'T',    'H',    '\x8A', 'd',    '\x93', '\x8D', '\xFB', 'F',    'g',
    '\xC9', '\x1A', '\x14', '\x7D', '\xF0', 'f',    'v',    'f',    '\xDF',
    '\x7C', '\xEF', '\xE7', 'g',    'F',    '\xA8', '\xD5', 'j',    'H',
    '\x24', '\x12', '\x2A', '\x0',  '\x5',  '\xBF', 'G',    '\xD4', '\xEF',
    '\xF7', '\x2F', '6',    '\xEC', '\x12', '\x20', '\x1E', '\x8F', '\xD7',
    '\xAA', '\xD5', '\xEA', '\xAF', 'I',    '5',    'F',    '\xAA', 'T',
    '\x5F', '\x9F', '\x22', 'A',    '\x2A', '\x95', '\xA',  '\x83', '\xE5',
    'r',    '9',    'd',    '\xB3', 'Y',    '\x96', '\x99', 'L',    '\x6',
    '\xE9', 't',    '\x9A', '\x25', '\x85', '\x2C', '\xCB', 'T',    '\xA7',
    '\xC4', 'b',    '1',    '\xB5', '\x5E', '\x0',  '\x3',  'h',    '\x9A',
    '\xC6', '\x16', '\x82', '\x20', 'X',    'R',    '\x14', 'E',    '6',
    'S',    '\x94', '\xCB', 'e',    'x',    '\xBD', '\x5E', '\xAA', 'U',
    'T',    '\x23', 'L',    '\xC0', '\xE0', '\xE2', '\xC1', '\x8F', '\x0',
    '\x9E', '\xBC', '\x9',  'A',    '\x7C', '\x3E', '\x1F', '\x83', 'D',
    '\x22', '\x11', '\xD5', 'T',    '\x40', '\x3F', '8',    '\x80', 'w',
    '\xE5', '3',    '\x7',  '\xB8', '\x5C', '\x2E', 'H',    '\x92', '\x4',
    '\x87', '\xC3', '\x81', '\x40', '\x20', '\x40', 'g',    '\x98', '\xE9',
    '6',    '\x1A', '\xA6', 'g',    '\x15', '\x4',  '\xE3', '\xD7', '\xC8',
    '\xBD', '\x15', '\xE1', 'i',    '\xB7', 'C',    '\xAB', '\xEA', 'x',
    '\x2F', 'j',    'X',    '\x92', '\xBB', '\x18', '\x20', '\x9F', '\xCF',
    '3',    '\xC3', '\xB8', '\xE9', 'N',    '\xA7', '\xD3', 'l',    'J',
    '\x0',  'i',    '6',    '\x7C', '\x8E', '\xE1', '\xFE', 'V',    '\x84',
    '\xE7', '\x3C', '\x9F', 'r',    '\x2B', '\x3A', 'B',    '\x7B', '7',
    'f',    'w',    '\xAE', '\x8E', '\xE',  '\xF3', '\xBD', 'R',    '\xA9',
    'd',    '\x2',  'B',    '\xAF', '\x85', '2',    'f',    'F',    '\xBA',
    '\xC',  '\xD9', '\x9F', '\x1D', '\x9A', 'l',    '\x22', '\xE6', '\xC7',
    '\x3A', '\x2C', '\x80', '\xEF', '\xC1', '\x15', '\x90', '\x7',  '\x93',
    '\xA2', '\x28', '\xA0', 'S',    'j',    '\xB1', '\xB8', '\xDF', '\x29',
    '5',    'C',    '\xE',  '\x3F', 'X',    '\xFC', '\x98', '\xDA', 'y',
    'j',    'P',    '\x40', '\x0',  '\x87', '\xAE', '\x1B', '\x17', 'B',
    '\xB4', '\x3A', '\x3F', '\xBE', 'y',    '\xC7', '\xA',  '\x26', '\xB6',
    '\xEE', '\xD9', '\x9A', '\x60', '\x14', '\x93', '\xDB', '\x8F', '\xD',
    '\xA',  '\x2E', '\xE9', '\x23', '\x95', '\x29', 'X',    '\x0',  '\x27',
    '\xEB', 'n',    'V',    'p',    '\xBC', '\xD6', '\xCB', '\xD6', 'G',
    '\xAB', '\x3D', 'l',    '\x7D', '\xB8', '\xD2', '\xDD', '\xA0', '\x60',
    '\x83', '\xBA', '\xEF', '\x5F', '\xA4', '\xEA', '\xCC', '\x2',  'N',
    '\xAE', '\x5E', 'p',    '\x1A', '\xEC', '\xB3', '\x40', '9',    '\xAC',
    '\xFE', '\xF2', '\x91', '\x89', 'g',    '\x91', '\x85', '\x21', '\xA8',
    '\x87', '\xB7', 'X',    '\x7E', '\x7E', '\x85', '\xBB', '\xCD', 'N',
    'N',    'b',    't',    '\x40', '\xFA', '\x93', '\x89', '\xEC', '\x1E',
    '\xEC', '\x86', '\x2',  'H',    '\x26', '\x93', '\xD0', 'u',    '\x1D',
    '\x7F', '\x9',  '2',    '\x95', '\xBF', '\x1F', '\xDB', '\xD7', 'c',
    '\x8A', '\x1A', '\xF7', '\x5C', '\xC1', '\xFF', '\x22', 'J',    '\xC3',
    '\x87', '\x0',  '\x3',  '\x0',  'K',    '\xBB', '\xF8', '\xD6', '\x2A',
    'v',    '\x98', 'I',    '\x0',  '\x0',  '\x0',  '\x0',  'I',    'E',
    'N',    'D',    '\xAE', 'B',    '\x60', '\x82',
};

void MimeType::init() {
  m_mapMime[".html"] = "text/html";
  m_mapMime[".avi"] = "video/x-msvideo";
  m_mapMime[".bmp"] = "image/bmp";
  m_mapMime[".c"] = "text/plain";
  m_mapMime[".doc"] = "application/msword";
  m_mapMime[".gif"] = "image/gif";
  m_mapMime[".gz"] = "application/x-gzip";
  m_mapMime[".htm"] = "text/html";
  m_mapMime[".ico"] = "image/x-icon";
  m_mapMime[".jpg"] = "image/jpeg";
  m_mapMime[".png"] = "image/png";
  m_mapMime[".txt"] = "text/plain";
  m_mapMime[".mp3"] = "audio/mp3";
  m_mapMime["default"] = "text/html";
}

std::string MimeType::getMime(const std::string &suffix) {
  pthread_once(&once_control, MimeType::init);
  if (m_mapMime.find(suffix) == m_mapMime.end())
    return m_mapMime["default"];
  else
    return m_mapMime[suffix];
}


HttpData::HttpData(EventLoop* loop,int fd)
    :loop_(loop),
    keepAlive_(false),
    channel_(new Channel(loop,fd)),
    fd_(fd)
    {
        channel_->setReadHander(std::bind(&HttpData::handleRead,this));
        channel_->setWriteHander(std::bind(&HttpData::handleWrite,this));
        channel_->setConnHander(std::bind(&HttpData::handleConn,this));
    }

HttpData::~HttpData()
{

}

void HttpData::newConnection()
{
    channel_->setEvents(DEFAULT_EVENT);
    loop_->addEvent(channel_,DEFAULT_EXPIRED_TIME);
}

void HttpData::handleClose()
{
    sockets::shutDownWR(channel_->getFd());
}

void HttpData::handleRead()
{

}

void HttpData::handleWrite()
{

}

void HttpData::handleConn()
{

}

void HttpData::handleErr(int fd,int err_num,std::string short_msg)
{
    short_msg = " " + short_msg;
    char send_buff[4096];
    string body_buff, header_buff;
     body_buff += "<html><title>ERROR</title>";
      body_buff += "<body bgcolor=\"ffffff\">";
    body_buff += to_string(err_num) + short_msg;
    body_buff += "<hr><em> LinYa's Web Server</em>\n</body></html>";
  header_buff += "HTTP/1.1 " + to_string(err_num) + short_msg + "\r\n";
  header_buff += "Content-Type: text/html\r\n";
  header_buff += "Connection: Close\r\n";
  header_buff += "Content-Length: " + to_string(body_buff.size()) + "\r\n";
  header_buff += "Server: LinYa's Web Server\r\n";
  ;
  header_buff += "\r\n";
  // you keneng meiyou xie wan
  sprintf(send_buff, "%s", header_buff.c_str());
  sockets::writen(fd, send_buff, strlen(send_buff));
  sprintf(send_buff, "%s", body_buff.c_str());
  sockets::writen(fd, send_buff, strlen(send_buff));
}

AnalysisState HttpData::analyzeRequest()
{
    if(method_ == METHOD_POST)
    {

    }
    else if(method_ == METHOD_GET || method_ == METHOD_HEAD)
    {
        string header;
        header += "HTTP/1.1 200 OK\r\n";
        if(headers_.find("Connection") != headers_.end() &&
             (headers_["Connection"] == "Keep-Alive" ||
         headers_["Connection"] == "keep-alive"))
        {
             keepAlive_ = true;
            header += string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" +
                to_string(DEFAULT_KEEP_ALIVE_TIME) + "\r\n";
        }
        int iTypePos = fileName_.find('.');
        string strFiletype;
        if(iTypePos < 0)
        {/* code */
            strFiletype = MimeType::getMime("default");
        }
        else
        {
           strFiletype = MimeType::getMime(fileName_.substr(0,iTypePos));
        }
        if(fileName_ == "hello")
        {
            outBuffer_ = "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\nHello World";
            return ANALYSIS_SUCCESS;
        }
        else if(fileName_ == "favicon.ico")
        {
            header +="Content-Type: image/png\r\n";
            header += "Content-Length: "+to_string(sizeof(favicon))+"\r\n";
            header += "Server: Lei's Web Server\r\n";
            header += "\r\n";
            outBuffer_ += header;
            outBuffer_ +=string(favicon,favicon+sizeof(favicon));
            return ANALYSIS_SUCCESS;
        }
        //find file
        struct  stat fBuf;
        if(stat(fileName_.c_str(),&fBuf) < 0) 
        {
            handleErr(fd_,404,"not find");
            return ANALYSIS_ERROR;
        }
        header +="Content-Type: "+strFiletype+"\r\n";
        header += "Content-Length: "+to_string(sizeof(fBuf.st_size))+"\r\n";
        header += "Server: Lei's Web Server\r\n";
        header += "\r\n";
        outBuffer_ += header;
         if (method_ == METHOD_HEAD) return ANALYSIS_SUCCESS;

        //here add image

        return ANALYSIS_ERROR;
    }
        
}
    

 HeaderState HttpData::parseHeaders()
 {
     string &str = readBuffer_;
     int key_start= -1,key_end =-1,value_start = -1,value_end=-1;
     int now_read_line_begin = 0;
     bool notFinsh = true;
     size_t i =0;
     for(;i< str.length() && notFinsh; i++)
     {
         switch (hState_)
         {
         case H_START:
             {
                 if(str[i] == '\r' || str[i] == '\n')
                    break;
                hState_ = H_KEY;
                key_start = i;
                now_read_line_begin = i;
                break;
             }
             case H_KEY:
             {
                 if(str[i] == ':')
                 {
                     key_end = i;
                     if(key_end - key_start < 0)
                     {
                         return PARSE_HEADER_ERROR;
                     }
                     hState_ = H_COLON;
                 }
                 else if(str[i] == '\r' || str[i] == '\n')
                 {
                     return PARSE_HEADER_ERROR;
                 }
                 break;
             }
             case H_COLON:
             {
                 if(str[i]  == ' ')
                 {
                     hState_ = H_SPACES_AFTER_COLON;
                 }
                 else
                 {
                      return PARSE_HEADER_ERROR;
                 }
                 break;
             }
             case H_SPACES_AFTER_COLON:
             {
                hState_ = H_VALUE;
                value_start = i;
                break;
             }
             case H_VALUE:
             {
                 if(str[i] == '\r')
                 {
                     hState_=H_CR;
                     value_end =i;
                     if(value_end - value_start <= 0)
                     {
                          return PARSE_HEADER_ERROR;
                     }
                    else if (i - value_start > 255)
                        return PARSE_HEADER_ERROR;
                    break;
                 }
             }
             case H_CR:
             {
                 if(str[i] == '\n')
                 {
                     hState_ = H_LF;
                     string key(str.begin()+key_start,str.begin()+key_end);
                     string value(str.begin()+value_start,str.begin()+value_end);
                     headers_[key] = value;
                     now_read_line_begin = i;
                 }
                 else
                 {
                       return PARSE_HEADER_ERROR;
                 }
                break; 
             }
        case H_LF: {
        if (str[i] == '\r') {
          hState_ = H_END_CR;
        } else {
          key_start = i;
          hState_ = H_KEY;
        }
        break;
      }
      case H_END_CR: {
        if (str[i] == '\n') {
          hState_ = H_END_LF;
        } else
          return PARSE_HEADER_ERROR;
        break;
      }
      case H_END_LF: {
        notFinsh = false;
        key_start = i;
        now_read_line_begin = i;
        break;
      }
         default:
             break;
         }
     }
      if(hState_ == H_END_LF)
      {
          str = str.substr(i);
          return PARSE_HEADER_SUCCESS;
      }
      str = str.substr(now_read_line_begin);
      return PARSE_HEADER_AGAIN;
 }

URIState HttpData::parseURI()
{
    std::string &str = readBuffer_;
    size_t pos = str.find('\r',0);
    if (pos < 0 )
    {
        return PARSE_URI_AGAIN;
    }
    
    std::string request_line = str.substr(0,pos);
    if(str.length() > pos+1)
    {
        str = str.substr(pos+1);
    }
    else
    {
        str.clear();
    }
    if(request_line.find("GET") > 0)
    {
        method_ = METHOD_GET;
    }
    else  if(request_line.find("POST") > 0)
    {
        method_ = METHOD_POST;
    }
    else  if(request_line.find("HEAD") > 0)
    {
        method_ = METHOD_HEAD;
    }
    else
    {
        return PARSE_URI_ERROR;
    }
    
    //filename
    pos = request_line.find_first_of("/",0);
    if(pos < 0) 
    {
        fileName_ ="index.html";
        httpVersion_ = HTTP_11;
        return PARSE_URI_SUCCESS;
    }
    else
    {
        size_t endPos = request_line.find_first_of(' ',pos);
        if (endPos < 0)
        {
           return PARSE_URI_ERROR;
        }
        if(endPos - pos > 1)
        {
            fileName_ = request_line.substr(pos,endPos - pos - 1);
            size_t _pos = fileName_.find_first_of('?');
            if(_pos > 0)
            {
                fileName_.substr(0,_pos);
            }
        }
        else
        {
             fileName_ ="index.html";
        }
        pos = endPos;
    }
    //http version

    pos = request_line.find_first_of("/",pos);
    if(pos < 0)
    {
        return PARSE_URI_ERROR;
    }
    else
    {
        if(request_line.length() - pos < 3)
        {
             return PARSE_URI_ERROR;
        }
        else
        {
            std::string tem = request_line.substr(pos+1,3);
            if(tem.compare("1.0") == 0)
            {
                httpVersion_ = HTTP_10;
            }
            else
            {
               httpVersion_ = HTTP_11;
            }
            
        }
        return PARSE_URI_SUCCESS;
    }
    
}
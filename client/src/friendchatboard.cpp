#include <initializer_list>
#include "sdldevice.hpp"
#include "client.hpp"
#include "hero.hpp"
#include "pngtexdb.hpp"
#include "processrun.hpp"
#include "friendchatboard.hpp"

extern Client *g_client;
extern PNGTexDB *g_progUseDB;
extern SDLDevice *g_sdlDevice;

FriendChatBoard::FriendChatBoard(int argX, int argY, ProcessRun *runPtr, Widget *widgetPtr, bool autoDelete)
    : Widget
      {
          DIR_UPLEFT,
          argX,
          argY,

          UIPage_BORDER[2] + UIPage_WIDTH  + UIPage_BORDER[3],
          UIPage_BORDER[0] + UIPage_HEIGHT + UIPage_BORDER[1],

          {},

          widgetPtr,
          autoDelete,
      }

    , m_processRun(runPtr)
    , m_frame
      {
          DIR_UPLEFT,
          0,
          0,
          {},
          {},
          [](const ImageBoard *){ return g_progUseDB->retrieve(0X00000800); },
      }

    , m_frameCropDup
      {
          DIR_UPLEFT,
          0,
          0,
          this->w(),
          this->h(),

          &m_frame,

          55,
          95,
          285 - 55,
          345 - 95,

          this,
          false,
      }

    , m_background
      {
          DIR_UPLEFT,
          0,
          0,
          {},
          {},
          [](const ImageBoard *){ return g_progUseDB->retrieve(0X00000810); },

          false,
          false,
          0,
          colorf::RGBA(160, 160, 160, 255),
      }

    , m_backgroundCropDup
      {
          DIR_UPLEFT,
          0,
          0,
          m_frameCropDup.w(),
          m_frameCropDup.h(),

          &m_background,

          0,
          0,
          510,
          187,

          this,
          false,
      }

    , m_slider
      {
          DIR_UPLEFT,
          m_frameCropDup.w() - 30,
          70,
          9,
          m_frameCropDup.h() - 140,

          false,
          3,
          nullptr,

          this,
          false,
      }

    , m_close
      {
          DIR_UPLEFT,
          m_frameCropDup.w() - 38,
          m_frameCropDup.h() - 40,
          {SYS_U32NIL, 0X0000001C, 0X0000001D},
          {
              SYS_U32NIL,
              SYS_U32NIL,
              0X01020000 + 105,
          },

          nullptr,
          nullptr,
          [this](ButtonBase *)
          {
              setShow(false);
          },

          0,
          0,
          0,
          0,

          true,
          true,
          this,
      }

    , m_uiPageList
      {
          UIPageWidgetGroup // UIPage_CHAT
          {
              .title = new LabelBoard
              {
                  DIR_NONE,
                  45 + (m_frameCropDup.w() - 45 - 190) / 2,
                  29,

                  u8"好友名称",
                  1,
                  14,
                  0,colorf::WHITE + colorf::A_SHF(255),

                  this,
                  true,
              },

              .control = new PageControl
              {
                  DIR_RIGHT,
                  m_frameCropDup.w() - 42,
                  29,
                  2,

                  {
                      {
                          new TritexButton
                          {
                              DIR_UPLEFT,
                              0,
                              0,

                              {0X000008F0, 0X000008F0, 0X000008F1},
                              {
                                  SYS_U32NIL,
                                  SYS_U32NIL,
                                  0X01020000 + 105,
                              },

                              nullptr,
                              nullptr,
                              [this](ButtonBase *)
                              {
                                  setUIPage(UIPage_CHATPREVIEW);
                              },
                          },

                          true,
                      },

                      {
                          new TritexButton
                          {
                              DIR_UPLEFT,
                              0,
                              0,

                              {0X00000023, 0X00000023, 0X00000024},
                              {
                                  SYS_U32NIL,
                                  SYS_U32NIL,
                                  0X01020000 + 105,
                              },

                              nullptr,
                              nullptr,
                              [this](ButtonBase *)
                              {
                              },
                          },

                          true,
                      },

                      {
                          new TritexButton
                          {
                              DIR_UPLEFT,
                              0,
                              0,

                              {0X000008B0, 0X000008B0, 0X000008B1},
                              {
                                  SYS_U32NIL,
                                  SYS_U32NIL,
                                  0X01020000 + 105,
                              },

                              nullptr,
                              nullptr,
                              [this](ButtonBase *)
                              {
                              },
                          },

                          true,
                      },

                      {
                          new TritexButton
                          {
                              DIR_UPLEFT,
                              0,
                              0,

                              {0X00000590, 0X00000590, 0X00000591},
                              {
                                  SYS_U32NIL,
                                  SYS_U32NIL,
                                  0X01020000 + 105,
                              },

                              nullptr,
                              nullptr,
                              [this](ButtonBase *)
                              {
                              },
                          },

                          true,
                      },

                  },

                  this,
                  true,
              },

              .page = new ChatPage
              {
                  DIR_UPLEFT,
                  UIPage_BORDER[2],
                  UIPage_BORDER[0],

                  this,
                  true,
              },
          },

          UIPageWidgetGroup // UIPage_CHATPREVIEW
          {
              .title = new LabelBoard
              {
                  DIR_NONE,
                  45 + (m_frameCropDup.w() - 45 - 190) / 2,
                  29,

                  u8"【聊天记录】",
                  1,
                  14,
                  0,colorf::WHITE + colorf::A_SHF(255),

                  this,
                  true,
              },

              .control = new PageControl
              {
                  DIR_RIGHT,
                  m_frameCropDup.w() - 42,
                  28,
                  2,

                  {
                      {
                          new TritexButton
                          {
                              DIR_UPLEFT,
                              0,
                              0,

                              {0X00000160, 0X00000160, 0X00000161},
                              {
                                  SYS_U32NIL,
                                  SYS_U32NIL,
                                  0X01020000 + 105,
                              },

                              nullptr,
                              nullptr,
                              [this](ButtonBase *)
                              {
                                  setUIPage(UIPage_FRIENDLIST);
                              },
                          },

                          true,
                      },
                  },

                  this,
                  true,
              },

              .page = new ChatPreviewPage
              {
                  DIR_UPLEFT,
                  UIPage_BORDER[2] + UIPage_MARGIN,
                  UIPage_BORDER[0] + UIPage_MARGIN,

                  this,
                  true,
              },
          },

          UIPageWidgetGroup // UIPage_FRIENDLIST
          {
              .title = new LabelBoard
              {
                  DIR_NONE,
                  45 + (m_frameCropDup.w() - 45 - 190) / 2,
                  29,

                  u8"【好友列表】",
                  1,
                  14,
                  0,colorf::WHITE + colorf::A_SHF(255),

                  this,
                  true,
              },

              .control = new PageControl
              {
                  DIR_RIGHT,
                  m_frameCropDup.w() - 42,
                  28,
                  2,

                  {
                      {
                          new TritexButton
                          {
                              DIR_UPLEFT,
                              0,
                              0,

                              {0X000008F0, 0X000008F0, 0X000008F1},
                              {
                                  SYS_U32NIL,
                                  SYS_U32NIL,
                                  0X01020000 + 105,
                              },

                              nullptr,
                              nullptr,
                              [this](ButtonBase *)
                              {
                                  setUIPage(UIPage_CHATPREVIEW);
                              },
                          },

                          true,
                      },

                      {
                          new TritexButton
                          {
                              DIR_UPLEFT,
                              0,
                              0,

                              {0X00000170, 0X00000170, 0X00000171},
                              {
                                  SYS_U32NIL,
                                  SYS_U32NIL,
                                  0X01020000 + 105,
                              },

                              nullptr,
                              nullptr,
                              [this](ButtonBase *)
                              {
                                  setUIPage(UIPage_FRIENDSEARCH);
                              },
                          },

                          true,
                      },
                  },

                  this,
                  true,
              },

              .page = new FriendListPage
              {
                  DIR_UPLEFT,
                  UIPage_BORDER[2] + UIPage_MARGIN,
                  UIPage_BORDER[0] + UIPage_MARGIN,

                  this,
                  true,
              },
          },

          UIPageWidgetGroup // UIPage_FRIENDSEARCH
          {
              .title = new LabelBoard
              {
                  DIR_NONE,
                  45 + (m_frameCropDup.w() - 45 - 190) / 2,
                  29,

                  u8"【查找用户】",
                  1,
                  14,
                  0,colorf::WHITE + colorf::A_SHF(255),

                  this,
                  true,
              },

              .control = new PageControl
              {
                  DIR_RIGHT,
                  m_frameCropDup.w() - 42,
                  28,
                  2,

                  {
                      {
                          new TritexButton
                          {
                              DIR_UPLEFT,
                              0,
                              0,

                              {0X000008F0, 0X000008F0, 0X000008F1},
                              {
                                  SYS_U32NIL,
                                  SYS_U32NIL,
                                  0X01020000 + 105,
                              },

                              nullptr,
                              nullptr,
                              [this](ButtonBase *)
                              {
                                  setUIPage(UIPage_CHATPREVIEW);
                              },
                          },

                          true,
                      },
                  },

                  this,
                  true,
              },

              .page = new SearchPage
              {
                  DIR_UPLEFT,
                  UIPage_BORDER[2] + UIPage_MARGIN,
                  UIPage_BORDER[0] + UIPage_MARGIN,

                  this,
                  true,
              },
          },
      }
{
    setShow(false);
}

void FriendChatBoard::drawEx(int dstX, int dstY, int srcX, int srcY, int srcW, int srcH) const
{
    for(const auto &p:
    {
        static_cast<const Widget *>(&m_backgroundCropDup),
        static_cast<const Widget *>( m_uiPageList[m_uiPage].page),
        static_cast<const Widget *>(&m_frameCropDup),
        static_cast<const Widget *>( m_uiPageList[m_uiPage].title),
        static_cast<const Widget *>( m_uiPageList[m_uiPage].control),
        static_cast<const Widget *>(&m_slider),
        static_cast<const Widget *>(&m_close),
    }){
        int drawSrcX = srcX;
        int drawSrcY = srcY;
        int drawSrcW = srcW;
        int drawSrcH = srcH;
        int drawDstX = dstX;
        int drawDstY = dstY;

        if(mathf::cropChildROI(
                    &drawSrcX, &drawSrcY,
                    &drawSrcW, &drawSrcH,
                    &drawDstX, &drawDstY,

                    w(),
                    h(),

                    p->dx(),
                    p->dy(),
                    p-> w(),
                    p-> h())){
            p->drawEx(drawDstX, drawDstY, drawSrcX, drawSrcY, drawSrcW, drawSrcH);
        }
    }
}

bool FriendChatBoard::processEvent(const SDL_Event &event, bool valid)
{
    if(!valid){
        return consumeFocus(false);
    }

    if(!show()){
        return consumeFocus(false);
    }

    if(m_close                        .processEvent(event, valid)){ return true; }
    if(m_slider                       .processEvent(event, valid)){ return true; }
    if(m_uiPageList[m_uiPage].page   ->processEvent(event, valid)){ return true; }
    if(m_uiPageList[m_uiPage].control->processEvent(event, valid)){ return true; }

    switch(event.type){
        case SDL_KEYDOWN:
            {
                if(focus()){
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            {
                                setShow(false);
                                setFocus(false);
                                return true;
                            }
                        default:
                            {
                                return false;
                            }
                    }
                }
                return false;
            }
        case SDL_MOUSEMOTION:
            {
                if((event.motion.state & SDL_BUTTON_LMASK) && (in(event.motion.x, event.motion.y) || focus())){
                    const auto [rendererW, rendererH] = g_sdlDevice->getRendererSize();
                    const int maxX = rendererW - w();
                    const int maxY = rendererH - h();

                    const int newX = std::max<int>(0, std::min<int>(maxX, x() + event.motion.xrel));
                    const int newY = std::max<int>(0, std::min<int>(maxY, y() + event.motion.yrel));
                    moveBy(newX - x(), newY - y());
                    return consumeFocus(true);
                }
                return consumeFocus(false);
            }
        case SDL_MOUSEBUTTONDOWN:
            {
                if(m_uiPageList[m_uiPage].page->in(event.button.x, event.button.y)){
                    if(m_uiPageList[m_uiPage].page->processEvent(event, true)){
                        return consumeFocus(true, m_uiPageList[m_uiPage].page);
                    }
                }
                return consumeFocus(in(event.button.x, event.button.y));
            }
        case SDL_MOUSEWHEEL:
            {
                if(m_uiPageList[m_uiPage].page->focus()){
                    if(m_uiPageList[m_uiPage].page->processEvent(event, true)){
                        return consumeFocus(true, m_uiPageList[m_uiPage].page);
                    }
                }
                return false;
            }
        default:
            {
                return false;
            }
    }
}

void FriendChatBoard::setFriendList(const SDFriendList &sdFL)
{
    m_sdFriendList = sdFL;
    std::unordered_set<uint32_t> seenDBIDList;

    seenDBIDList.insert(SYS_CHATDBID_SYSTEM);
    dynamic_cast<FriendListPage *>(m_uiPageList[UIPage_FRIENDLIST].page)->append(new FriendItem
    {
        DIR_UPLEFT,
        0,
        0,

        SYS_CHATDBID_SYSTEM,
        u8"系统消息",

        [](const ImageBoard *)
        {
            return g_progUseDB->retrieve(0X00001100);
        },

    }, true);

    seenDBIDList.insert(m_processRun->getMyHero()->dbid());
    dynamic_cast<FriendListPage *>(m_uiPageList[UIPage_FRIENDLIST].page)->append(new FriendItem
    {
        DIR_UPLEFT,
        0,
        0,

        m_processRun->getMyHero()->dbid(),
        to_u8cstr(m_processRun->getMyHero()->getName()),

        [this](const ImageBoard *)
        {
            return g_progUseDB->retrieve(Hero::faceGfxID(m_processRun->getMyHero()->gender(), m_processRun->getMyHero()->job()));
        },
    }, true);

    for(const auto &sdPC: sdFL){
        if(seenDBIDList.contains(sdPC.dbid)){
            continue;
        }

        seenDBIDList.insert(sdPC.dbid);
        dynamic_cast<FriendListPage *>(m_uiPageList[UIPage_FRIENDLIST].page)->append(new FriendItem
        {
            DIR_UPLEFT,
            0,
            0,

            sdPC.dbid,
            to_u8cstr(sdPC.name),

            [gender =sdPC.gender, job = sdPC.job, this](const ImageBoard *)
            {
                return g_progUseDB->retrieve(Hero::faceGfxID(gender, job));
            },
        }, true);
    }
}

void FriendChatBoard::addMessage(const SDChatMessage &newmsg)
{
    const auto saveInDBID = [&newmsg, this]
    {
        if(newmsg.from == m_processRun->getMyHero()->dbid()){
            return newmsg.to;
        }
        else if(newmsg.to == m_processRun->getMyHero()->dbid()){
            return newmsg.from;
        }
        else{
            throw fflerror("received invalid chat message: from %llu, to %llu, self %llu", to_llu(newmsg.from), to_llu(newmsg.to), to_llu(m_processRun->getMyHero()->dbid()));
        }
    }();

    auto p = std::find_if(m_friendMessageList.begin(), m_friendMessageList.end(), [saveInDBID](const auto &item)
    {
        return item.dbid == saveInDBID;
    });

    if(p == m_friendMessageList.end()){
        m_friendMessageList.emplace_front(saveInDBID);
    }
    else if(p != m_friendMessageList.begin()){
        m_friendMessageList.splice(m_friendMessageList.begin(), m_friendMessageList, p, std::next(p));
    }

    p = m_friendMessageList.begin();
    if(std::find_if(p->list.begin(), p->list.end(), [&newmsg](const auto &msg){ return msg.id == newmsg.id; }) == p->list.end()){
        p->unread++;
        p->list.push_back(newmsg);

        if(p->list.size() >= 2 && p->list.back().timestamp < p->list.rbegin()[1].timestamp){
            std::sort(p->list.begin(), p->list.end(), [](const auto &x, const auto &y)
            {
                if(x.timestamp != y.timestamp){
                    return x.timestamp < y.timestamp;
                }
                else{
                    return x.id < y.id;
                }
            });

            if(dynamic_cast<ChatPage *>(m_uiPageList[UIPage_CHAT].page)->dbid == p->dbid){
                loadChatPage(p->dbid);
            }
        }
        else{
            if(auto chatPage = dynamic_cast<ChatPage *>(m_uiPageList[UIPage_CHAT].page); chatPage->dbid == p->dbid){
                chatPage->chat.append(new ChatItem
                {
                    DIR_UPLEFT,
                    0,
                    0,

                    [currDBID = p->dbid, this]() -> const char8_t *
                    {
                        if(currDBID == SYS_CHATDBID_SYSTEM){
                            return u8"系统消息";
                        }

                        const auto friendIter = std::find_if(m_sdFriendList.begin(), m_sdFriendList.end(), [currDBID](const auto &x)
                        {
                            return currDBID == x.dbid;
                        });

                        if(friendIter == m_sdFriendList.end()){
                            return u8"未知";
                        }

                        return to_u8cstr(friendIter->name);
                    }(),

                    to_u8cstr(cerealf::deserialize<std::string>(p->list.back().message)),

                    [currDBID = p->dbid, this](const ImageBoard *)
                    {
                        if(currDBID == SYS_CHATDBID_SYSTEM){
                            return g_progUseDB->retrieve(0X00001100);
                        }

                        const auto friendIter = std::find_if(m_sdFriendList.begin(), m_sdFriendList.end(), [currDBID](const auto &x)
                        {
                            return currDBID == x.dbid;
                        });

                        if(friendIter == m_sdFriendList.end()){
                            return g_progUseDB->retrieve(0X010007CF);
                        }

                        return g_progUseDB->retrieve(Hero::faceGfxID(friendIter->gender, friendIter->job));
                    },

                    true,
                    true,

                    {},
                }, true);
            }
            dynamic_cast<ChatPreviewPage *>(m_uiPageList[UIPage_CHATPREVIEW].page)->updateChatPreview(saveInDBID, cerealf::deserialize<std::string>(p->list.back().message));
        }
    }
}

void FriendChatBoard::setChatPageDBID(uint32_t argDBID)
{
    if(auto chatPage = dynamic_cast<ChatPage *>(m_uiPageList[UIPage_CHAT].page); chatPage->dbid != argDBID){
        chatPage->dbid = argDBID;
        loadChatPage(argDBID);
    }
}

void FriendChatBoard::setUIPage(int uiPage, const char *titleStr)
{
    fflassert(uiPage >= 0, uiPage);
    fflassert(uiPage < UIPage_END, uiPage);

    if(m_uiPage != uiPage){
        m_uiLastPage = m_uiPage;
        m_uiPage     = uiPage;

        m_uiPageList[m_uiLastPage].page->setFocus(false);
        m_uiPageList[m_uiPage    ].page->setFocus(true );


        if(titleStr){
            m_uiPageList[m_uiPage].title->setText(to_u8cstr(titleStr));
        }
    }
}

void FriendChatBoard::loadChatPage(uint32_t argDBID)
{
    auto chatPage = dynamic_cast<ChatPage *>(m_uiPageList[UIPage_CHAT].page);

    chatPage->chat.canvas.clearChild();
    for(const auto &node: m_friendMessageList){
        if(node.dbid == argDBID){
            if(node.list.size() <= 1){
                m_processRun->requestLatestChatMessage({node.dbid}, 0, true, true);
            }

            for(const auto &msg: node.list){
                chatPage->chat.append(new ChatItem
                {
                    DIR_UPLEFT,
                    0,
                    0,

                    [chatPage, this]() -> const char8_t *
                    {
                        if(chatPage->dbid == SYS_CHATDBID_SYSTEM){
                            return u8"系统消息";
                        }

                        const auto p = std::find_if(m_sdFriendList.begin(), m_sdFriendList.end(), [chatPage](const auto &x)
                        {
                            return chatPage->dbid == x.dbid;
                        });

                        if(p == m_sdFriendList.end()){
                            return u8"未知";
                        }

                        return to_u8cstr(p->name);
                    }(),

                    to_u8cstr(cerealf::deserialize<std::string>(msg.message)),

                    [chatPage, this](const ImageBoard *)
                    {
                        if(chatPage->dbid == SYS_CHATDBID_SYSTEM){
                            return g_progUseDB->retrieve(0X00001100);
                        }

                        const auto p = std::find_if(m_sdFriendList.begin(), m_sdFriendList.end(), [chatPage](const auto &x)
                        {
                            return chatPage->dbid == x.dbid;
                        });

                        if(p == m_sdFriendList.end()){
                            return g_progUseDB->retrieve(0X010007CF);
                        }

                        return g_progUseDB->retrieve(Hero::faceGfxID(p->gender, p->job));
                    },

                    true,
                    true,

                    {},
                }, true);
            }
        }
    }

    chatPage->placeholder.setShow(!chatPage->chat.canvas.hasChild());
}

FriendChatBoard *FriendChatBoard::getParentBoard(Widget *widget)
{
    fflassert(widget);
    while(widget){
        if(auto p = dynamic_cast<FriendChatBoard *>(widget)){
            return p;
        }
        else{
            widget = widget->parent();
        }
    }
    throw fflerror("widget is not a decedent of FriendChatBoard");
}

const FriendChatBoard *FriendChatBoard::getParentBoard(const Widget *widget)
{
    fflassert(widget);
    while(widget){
        if(auto p = dynamic_cast<const FriendChatBoard *>(widget)){
            return p;
        }
        else{
            widget = widget->parent();
        }
    }
    throw fflerror("widget is not a decedent of FriendChatBoard");
}

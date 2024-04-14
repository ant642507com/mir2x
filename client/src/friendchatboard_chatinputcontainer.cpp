#include "client.hpp"
#include "pngtexdb.hpp"
#include "sdldevice.hpp"
#include "processrun.hpp"
#include "friendchatboard.hpp"

extern Client *g_client;
extern PNGTexDB *g_progUseDB;
extern SDLDevice *g_sdlDevice;

FriendChatBoard::ChatInputContainer::ChatInputContainer(dir8_t argDir,

        int argX,
        int argY,

        Widget *argParent,
        bool    argAutoDelete)

    : Widget
      {
          argDir,
          argX,
          argY,

          UIPage_WIDTH - UIPage_MARGIN * 2 - ChatPage::INPUT_MARGIN * 2,
          [this](const Widget *)
          {
              return mathf::bound<int>(layout.h(), ChatPage::INPUT_MIN_HEIGHT, ChatPage::INPUT_MAX_HEIGHT);
          },

          {},

          argParent,
          argAutoDelete,
      }

    , layout
      {
          DIR_UPLEFT,
          0,
          0,

          this->w(),

          nullptr,
          0,

          {},
          false,
          true,
          true,
          false,

          1,
          12,
          0,
          colorf::WHITE + colorf::A_SHF(255),
          0,

          LALIGN_JUSTIFY,
          0,
          0,

          2,
          colorf::WHITE + colorf::A_SHF(255),

          nullptr,
          [this]()
          {
              if(!layout.hasToken()){
                  return;
              }

              auto message = layout.getXML();
              layout.clear();

              auto chatBoard = FriendChatBoard::getParentBoard(this);
              auto chatPage  = dynamic_cast<ChatPage *>(chatBoard->m_uiPageList[UIPage_CHAT].page);

              const SDChatMessage chatMessage
              {
                  .group = chatPage->peer.group,
                  .from  = chatBoard->m_processRun->getMyHero()->dbid(),
                  .to    = chatPage->peer.dbid,
                  .message = cerealf::serialize(message),
              };

              chatPage->chat.append(chatMessage, [chatMessage, this](const FriendChatBoard::ChatItem *chatItem)
              {
                  auto dbidsv = as_sv(chatMessage.from);
                  auto msgbuf = std::string();

                  msgbuf.append(dbidsv.begin(), dbidsv.end());
                  msgbuf.append(chatMessage.message.begin(), chatMessage.message.end());

                  const auto widgetID = chatItem->id();
                  const auto pendingID = FriendChatBoard::getParentBoard(this)->addMessagePending(chatMessage);
                  const auto chatItemCanvas = std::addressof(dynamic_cast<FriendChatBoard::ChatPage *>(parent())->chat.canvas);

                  g_client->send({CM_CHATMESSAGE, msgbuf}, [widgetID, pendingID, chatItemCanvas, chatMessage, this](uint8_t headCode, const uint8_t *buf, size_t bufSize)
                  {
                      switch(headCode){
                          case SM_OK:
                              {
                                  const auto sdCMDBS = cerealf::deserialize<SDChatMessageDBSeq>(buf, bufSize);
                                  FriendChatBoard::getParentBoard(this)->finishMessagePending(pendingID, sdCMDBS);

                                  if(auto p = chatItemCanvas->hasChild(widgetID)){
                                      dynamic_cast<FriendChatBoard::ChatItem *>(p)->idOpt = sdCMDBS.id;
                                  }
                                  break;
                              }
                          default:
                              {
                                  throw fflerror("failed to send message");
                              }
                      }
                  });
              });
          },
          nullptr,

          this,
          false,
      }
{
    // there is mutual dependency
    // height of input container depends on height of layout
    //
    // layout always attach to buttom of input container, so argX needs container height
    // in initialization list we can not call this->h() since initialization of layout is not done yet
    layout.moveAt(DIR_DOWNLEFT, 0, [this](const Widget *){ return this->h() - 1; });
}

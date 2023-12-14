// basic button class to handle event logic only
// 1. no draw
// 2. no texture id field
//
// I support two callbacks only: off->on and on->click
// this class ask user to configure whether the on->click is triggered
// at the PRESS or RELEASE event.

#pragma once
#include <cstdint>
#include <functional>

#include "widget.hpp"
#include "sysconst.hpp"
#include "pngtexdb.hpp"
#include "sdldevice.hpp"

class ButtonBase: public Widget
{
    private:
        class InnButtonState final
        {
            // encapsulate it as a class
            // don't let button class to manipulate m_state directly
            private:
                int m_state[2]
                {
                    BEVENT_OFF,
                    BEVENT_OFF,
                };

            public:
                void setState(int state)
                {
                    m_state[0] = m_state[1];
                    m_state[1] = state;
                }

            public:
                int getState() const
                {
                    return m_state[1];
                }

                int getLastState() const
                {
                    return m_state[0];
                }
        };

    private:
        InnButtonState m_state;

    protected:
        const bool m_onClickDone;

    protected:
        const uint32_t m_seffID[3];

    protected:
        const int m_offset[3][2];

    protected:
        std::function<void()> m_onOverIn;
        std::function<void()> m_onOverOut;
        std::function<void()> m_onClick;

    public:
        ButtonBase(
                dir8_t argDir,
                int argX,
                int argY,
                int argW,
                int argH,

                std::function<void()> fnOnOverIn  = nullptr,
                std::function<void()> fnOnOverOut = nullptr,
                std::function<void()> fnOnClick   = nullptr,

                uint32_t seffIDOnOverIn  = SYS_U32NIL,
                uint32_t seffIDOnOverOut = SYS_U32NIL,
                uint32_t seffIDOnClick   = SYS_U32NIL,

                int offXOnOver  = 0,
                int offYOnOver  = 0,
                int offXOnClick = 0,
                int offYOnClick = 0,

                bool    onClickDone = true,
                Widget *widgetPtr   = nullptr,
                bool    autoFree    = false)

            : Widget
              {
                  argDir,
                  argX,
                  argY,
                  argW,
                  argH,

                  {},

                  widgetPtr,
                  autoFree,
              }

            , m_onClickDone(onClickDone)
            , m_seffID
              {
                  seffIDOnOverIn,
                  seffIDOnOverOut,
                  seffIDOnClick,
              }
            , m_offset
              {
                  {0            , 0          },
                  {offXOnOver   , offYOnOver },
                  {offXOnClick  , offYOnClick},
              }
            , m_onOverIn (std::move(fnOnOverIn))
            , m_onOverOut(std::move(fnOnOverOut))
            , m_onClick  (std::move(fnOnClick))
        {
            // we don't fail even if x, y, w, h are invalid
            // because derived class could do reset in its constructor
        }

    public:
        virtual ~ButtonBase() = default;

    public:
        bool processEvent(const SDL_Event &, bool) override;

    protected:
        int offX() const
        {
            return m_offset[getState()][0];
        }

        int offY() const
        {
            return m_offset[getState()][1];
        }

    public:
        int getState() const
        {
            return m_state.getState();
        }

        int getLastState() const
        {
            return m_state.getLastState();
        }

    public:
        void setState(int state)
        {
            m_state.setState(state);
        }

    public:
        // we can automatically do this or call this function
        // sometimes when we invoke the callback it changes the button location
        void setOff()
        {
            setState(BEVENT_OFF);
        }

    private:
        void onClick();
        void onOverIn();
        void onOverOut();
        void onBadEvent();
};

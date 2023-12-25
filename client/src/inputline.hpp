#pragma once
#include <functional>
#include "colorf.hpp"
#include "widget.hpp"
#include "xmltypeset.hpp"
#include "ime.hpp"

class InputLine: public Widget
{
    protected:
        bool m_imeEnabled = true;

    protected:
        XMLTypeset m_tpset;

    protected:
        int m_cursor = 0;

    protected:
        int      m_cursorWidth;
        uint32_t m_cursorColor;
        double   m_cursorBlink = 0.0;

    protected:
        std::function<void()> m_onTab;
        std::function<void()> m_onCR;

    public:
        InputLine(
                dir8_t argDir,
                int argX,
                int argY,
                int argW,
                int argH,

                bool argIMEEnabled,

                uint8_t  argFont      =  0,
                uint8_t  argFontSize  = 10,
                uint8_t  argFontStyle =  0,
                uint32_t argFontColor =  colorf::WHITE + colorf::A_SHF(255),

                int      argCursorWidth = 2,
                uint32_t argCursorColor = colorf::WHITE + colorf::A_SHF(255),

                std::function<void()> argOnTab = nullptr,
                std::function<void()> argOnCR  = nullptr,

                Widget *widgetPtr  = nullptr,
                bool    autoDelete = false)

            : Widget
              {
                  argDir,
                  argX,
                  argY,
                  argW,
                  argH,

                  {},

                  widgetPtr,
                  autoDelete,
              }

            , m_imeEnabled(argIMEEnabled)
            , m_tpset
              {
                  0,
                  LALIGN_LEFT,
                  false,
                  argFont,
                  argFontSize,
                  argFontStyle,
                  argFontColor,
              }
            , m_cursorWidth(argCursorWidth)
            , m_cursorColor(argCursorColor)
            , m_onTab(argOnTab)
            , m_onCR(argOnCR)
        {}

    public:
        bool processEvent(const SDL_Event &, bool);

    public:
        void drawEx(int, int, int, int, int, int) const override;

    public:
        void update(double ms) override
        {
            m_cursorBlink += ms;
        }

    public:
        std::string getRawString() const
        {
            return m_tpset.getRawString();
        }

        virtual void clear()
        {
            m_cursor = 0;
            m_cursorBlink = 0.0;
            m_tpset.clear();
        }

    public:
        void deleteChar();
        void insertChar(char);
};

#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include "buttonbase.hpp"

class TrigfxButton: public ButtonBase
{
    private:
        std::array<const Widget *, 3> m_gfxList;

    public:
        TrigfxButton(
                dir8_t,
                int,
                int,

                std::array<const Widget *, 3>,
                std::array<std::optional<uint32_t>, 3>,

                std::function<void()> = nullptr,
                std::function<void()> = nullptr,
                std::function<void()> = nullptr,

                int = 0,
                int = 0,
                int = 0,
                int = 0,

                bool = true,

                Widget * = nullptr,
                bool     = false);

    public:
        void drawEx(int,                 // dst x on the screen coordinate
                    int,                 // dst y on the screen coordinate
                    int,                 // src x on the widget, take top-left as origin
                    int,                 // src y on the widget, take top-left as origin
                    int,                 // size to draw
                    int) const override; // size to draw
    private:
        void initButtonSize();

    public:
        void setGfxList(const std::array<const Widget *, 3> &gfxList)
        {
            m_gfxList = gfxList;
        }
};

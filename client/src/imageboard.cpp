#include "colorf.hpp"
#include "totype.hpp"
#include "sdldevice.hpp"
#include "imageboard.hpp"

extern SDLDevice *g_sdlDevice;

ImageBoard::ImageBoard(
        dir8_t argDir,
        int argX,
        int argY,

        std::optional<int> argWOpt,
        std::optional<int> argHOpt,

        std::function<SDL_Texture *(const ImageBoard *)> argLoadFunc,

        bool argHFlip,
        bool argVFlip,
        int  argRotate,

        uint32_t argColor,

        Widget *argParent,
        bool    argAutoDelete)
    : Widget
      {
          argDir,
          argX,
          argY,
          0,
          0,

          argParent,
          argAutoDelete,
      }

    , m_loadFunc(std::move(argLoadFunc))
    , m_xformPair(getHFlipRotatePair(argHFlip, argVFlip, argRotate))
    , m_color(argColor)
{
    if(!m_loadFunc){
        throw fflerror("invalid texture load function");
    }

    auto texPtr = m_loadFunc(this);
    if(!texPtr){
        throw fflerror("load texture failed");
    }

    const auto [texW, texH] = SDLDeviceHelper::getTextureSize(texPtr);
    setSize(
            std::max<int>(0, (m_rotate % 2 == 0) ? argWOpt.value_or(texW) : argHOpt.value_or(texH)),
            std::max<int>(0, (m_rotate % 2 == 0) ? argHOpt.value_or(texH) : argWOpt.value_or(texW)));
}

void ImageBoard::drawEx(int dstX, int dstY, int srcX, int srcY, int srcW, int srcH) const
{
    if(!(w() > 0 && h() > 0)){
        return;
    }

    if(!colorf::A(m_color)){
        return;
    }

    if(!mathf::cropROI(&srcX, &srcY, &srcW, &srcH, &dstX, &dstY, w(), h())){
        return;
    }

    const auto [
        drawDstX, drawDstY,
        drawSrcX, drawSrcY,
        drawSrcW, drawSrcH,

        centerOffX,
        centerOffY,

        rotateDegree] = [dstX, dstY, srcX, srcY, srcW, srcH, this]() -> std::array<int, 9>
    {
        // draw rotate and flip
        // all corners are indexed as 0, 1, 2, 3

        // 0      1
        //  +----+
        //  |    |   empty square means dst region before rotation
        //  +----+
        // 3      2

        // 0      1
        //  +----+
        //  |....|   solid square means dst region after rotation, the real dst region
        //  +----+
        // 3      2

        switch(m_rotate % 4){
            case 0:
                {
                    //  0           1
                    //  +-----------+
                    //  |...........|
                    //  |...........|
                    //  +-----------+
                    //  3           2

                    return
                    {
                        dstX,
                        dstY,

                        srcX,
                        srcY,

                        srcW,
                        srcH,

                        0,
                        0,

                        0,
                    };
                }
            case 1:
                {
                    // 0           1
                    // +-----------+
                    // |           |
                    // |           |
                    // o-----+-----+
                    // |.....|     2
                    // |.....|
                    // |.....|
                    // |.....|
                    // +-----+
                    // 2     1

                    return
                    {
                        dstX,
                        dstY - srcW,

                        srcY,
                        w() - srcX - srcW,

                        srcH,
                        srcW,

                        0,
                        srcW,

                        90,
                    };
                }
            case 2:
                {
                    // 0           1
                    // +-----------+
                    // |           |
                    // |           |           3
                    // +-----------o-----------+
                    // 3           |...........|
                    //             |...........|
                    //             +-----------+
                    //             1           0

                    return
                    {
                        dstX - srcW,
                        dstY - srcH,

                        w() - srcW - srcX,
                        h() - srcH - srcY,

                        srcW,
                        srcH,

                        srcW,
                        srcH,

                        180,
                    };
                }
            default:
                {
                    // 1     2
                    // +-----+
                    // |.....|
                    // |.....|
                    // |.....|           1
                    // |.....+-----------+
                    // |.....|           |
                    // |.....|           |
                    // +-----o-----------+
                    // 0     3           2

                    return
                    {
                        dstX        + srcW,
                        dstY + srcH - srcW,

                        h() - srcH - srcY,
                        srcX,

                        srcH,
                        srcW,

                        0,
                        srcW,

                        270,
                    };
                }
        }
    }();

    const auto texPtr = m_loadFunc(this);
    if(!texPtr){
        return;
    }

    const auto [texW, texH] = SDLDeviceHelper::getTextureSize(texPtr);

    const auto  widthRatio = to_f(texW) / w();
    const auto heightRatio = to_f(texH) / h();

    // imgSrcX
    // size and position cropped from original image, no resize, well defined

    /**/  int imgSrcX = std::lround( widthRatio * drawSrcX);
    const int imgSrcY = std::lround(heightRatio * drawSrcY);
    const int imgSrcW = std::lround( widthRatio * drawSrcW);
    const int imgSrcH = std::lround(heightRatio * drawSrcH);

    if(m_hflip){
        imgSrcX = texW - imgSrcX - imgSrcW;
    }

    SDLDeviceHelper::EnableTextureModColor enableColor(texPtr, m_color);
    g_sdlDevice->drawTextureEx(
            texPtr,

            imgSrcX, imgSrcY,
            imgSrcW, imgSrcH,

            drawDstX, drawDstY,
            drawSrcW, drawSrcH,

            centerOffX,
            centerOffY,

            rotateDegree,
            m_hflip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

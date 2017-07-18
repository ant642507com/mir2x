/*
 * =====================================================================================
 *
 *       Filename: processsyrc.hpp
 *        Created: 08/14/2015 02:47:30
 *  Last Modified: 07/18/2017 15:20:53
 *
 *    Description: 
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */
#pragma once
#include <SDL2/SDL.h>
#include "process.hpp"
#include "labelboard.hpp"
#include "tokenboard.hpp"

class ProcessSyrc: public Process
{
    private:
        int        m_Ratio;
        LabelBoard m_Info;


    public:
        ProcessSyrc();

    public:
        ~ProcessSyrc() = default;

    public:
        int ID() const
        {
            return PROCESSID_SYRC;
        }

    public:
        void Update(double);
        void Draw();
        void ProcessEvent(const SDL_Event &);
};

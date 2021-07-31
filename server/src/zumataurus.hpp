#pragma once
#include "dbcomid.hpp"
#include "monster.hpp"

class ZumaTaurus final: public Monster
{
    private:
        bool m_standMode = false;

    public:
        ZumaTaurus(ServerMap *mapPtr, int argX, int argY, uint64_t masterUID)
            : Monster(DBCOM_MONSTERID(u8"祖玛教主"), mapPtr, argX, argY, DIR_BEGIN, masterUID)
        {}

    public:
        void setStandMode(bool standMode)
        {
            if(standMode != m_standMode){
                m_standMode = standMode;
                dispatchAction(ActionTransf
                {
                    .x = X(),
                    .y = Y(),
                    .direction = Direction(),
                    .extParam
                    {
                        .zumaTaurus
                        {
                            .standModeReq = m_standMode,
                        },
                    },
                });
            }
        }

    protected:
        corof::long_jmper updateCoroFunc() override;

    protected:
        ActionNode makeActionStand() const override
        {
            return ActionStand
            {
                .x = X(),
                .y = Y(),
                .direction = Direction(),
                .extParam
                {
                    .zumaTaurus
                    {
                        .standMode = m_standMode,
                    },
                },
            };
        }

    protected:
        void onAMMasterHitted(const ActorMsgPack &) override
        {
            setStandMode(true);
        }

    protected:
        void onAMAttack(const ActorMsgPack &) override;

    protected:
        bool canMove() const override
        {
            return m_standMode && Monster::canMove();
        }
};

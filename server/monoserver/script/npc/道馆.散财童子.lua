-- =====================================================================================
--
--       Filename: 道馆.散财童子.lua
--        Created: 11/22/2020 08:52:57 PM
--    Description: lua 5.3
--
--        Version: 1.0
--       Revision: none
--       Compiler: lua
--
--         Author: ANHONG
--          Email: anhonghe@gmail.com
--   Organization: USTC
--
-- =====================================================================================

-- NPC script
-- provides the table: processNPCEvent for event processing

addLog(LOGTYPE_INFO, string.format('NPC %s sources %s', getNPCFullName(), getFileName()))
setNPCLook(9)
setNPCGLoc(405, 125)

processNPCEvent =
{
    [SYS_NPCINIT] = function(uid, value)
        sayXML(uid, string.format(
        [[
            <layout>
                <par>客官%s你好我是%s，欢迎领取礼物！<emoji id="0"/></par>
                <par></par>
                <par><event id="npc_goto_1">领取金币</event></par>
                <par><event id="npc_goto_2">领取装备</event></par>
                <par><event id="%s">关闭</event></par>
            </layout>
        ]], uidQueryName(uid), getNPCName(), SYS_NPCDONE))
    end,

    ["npc_goto_1"] = function(uid, value)
        sendGift(uid, '金币', 1000)
    end,

    ["npc_goto_2"] = function(uid, value)
        sendGift(uid, '斩马刀', 2)
        sendGift(uid, '五彩鞋', 1)
        sendGift(uid, '井中月', 1)
    end,
}

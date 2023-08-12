--, u8R"###(
--

function runQuestThread(func)
    assertType(func, 'function')
    return runThread(rollKey(), func)
end

local function _RSVD_NAME_dbUpdateUIDQuestFieldTable(uid, field, key, value)
    assertType(uid, 'integer')
    assertType(field, 'string')

    local fieldTable = dbGetUIDQuestField(uid, field)
    assertType(fieldTable, 'nil', 'table')

    if fieldTable == nil then
        fieldTable = {}
    end

    if fieldTable[key] == value then
        return
    end

    fieldTable[key] = value

    if tableEmpty(fieldTable) then
        fieldTable = nil
    end
    dbSetUIDQuestField(uid, field, fieldTable)
end

function dbGetUIDQuestVar(uid, key)
    assertType(uid, 'integer')
    return (dbGetUIDQuestField(uid, 'fld_vars') or {})[key]
end

function dbSetUIDQuestVar(uid, key, value)
    assertType(uid, 'integer')
    _RSVD_NAME_dbUpdateUIDQuestFieldTable(uid, 'fld_vars', key, value)
end

function dbGetUIDQuestState(uid)
    assertType(uid, 'integer')
    return table.unpack(dbGetUIDQuestField(uid, 'fld_state') or {}, 1, 2)
end

function dbSetUIDQuestState(uid, state, args)
    assertType(uid, 'integer')
    assertType(state, 'string')
    dbSetUIDQuestField(uid, 'fld_state', {state, args})
end

function hasUIDQuestFlag(uid, flagName)
    assertType(uid, 'integer')
    assertType(flagName, 'string')

    local flags = dbGetUIDQuestField(uid, 'fld_flags') or {}
    return flags[flagName] or false
end

function addUIDQuestFlag(uid, flagName)
    assertType(uid, 'integer')
    _RSVD_NAME_dbUpdateUIDQuestFieldTable(uid, 'fld_flags', flagName, true)
end

function deleteUIDQuestFlag(uid, flagName)
    assertType(uid, 'integer')
    _RSVD_NAME_dbUpdateUIDQuestFieldTable(uid, 'fld_flags', flagName, nil)
end

function loadMap(map)
    return _RSVD_NAME_callFuncCoop('loadMap', map)
end

function getNPCharUID(mapName, npcName)
    local mapUID = loadMap(mapName)
    assertType(mapUID, 'integer')

    if mapUID == 0 then
        return 0
    end

    local npcUID = uidExecute(mapUID,
    [[
        return getNPCharUID('%s')
    ]], npcName)

    assertType(npcUID, 'integer')
    return npcUID
end

function setUIDQuestTeam(args)
    assertType(args, 'table')
    assertType(args.uid, 'integer')
    assertType(args.randRole, 'boolean', 'nil')
    assertType(args.propagate, 'boolean', 'nil')

    local team = uidExecute(args.uid,
    [[
        return {
            [SYS_QUESTFIELD.TEAM.LEADER] = getTeamLeader(),
            [SYS_QUESTFIELD.TEAM.MEMBERLIST] = getTeamMemberList(),
        }
    ]])

    if args.randRole then
        team[SYS_QUESTFIELD.TEAM.ROLELIST] = shuffleArray(team[SYS_QUESTFIELD.TEAM.MEMBERLIST])
    else
        team[SYS_QUESTFIELD.TEAM.ROLELIST] = team[SYS_QUESTFIELD.TEAM.MEMBERLIST]
    end

    for _, member in ipairs(team[SYS_QUESTFIELD.TEAM.MEMBERLIST]) do
        if args.propagate or (member == uid) then
            dbSetUIDQuestField(member, 'fld_team', team)
        end
    end
end

function getUIDQuestTeam(uid)
    assertType(uid, 'integer')
    local team = dbGetUIDQuestField(uid, 'fld_team')

    team.getRoleIndex = function(self, uid)
        assertType(self, 'table')
        assertType(uid, 'integer')

        for i, teamMember in ipairs(self[SYS_QUESTFIELD.TEAM.ROLELIST]) do
            if teamMember == uid then
                return i
            end
        end
        fatalPrintf('Can not find uid %d in team role list', uid)
    end
    return team
end

local _RSVD_NAME_questFSMTable = nil
function setQuestFSMTable(arg1, arg2)
    local fsmName  = nil
    local fsmTable = nil

    if type(arg1) == 'string' and type(arg2) == 'table' then
        fsmName  = arg1
        fsmTable = arg2

    elseif type(arg1) == 'table' and arg2 == nil then
        fsmName  = SYS_QSTFSM
        fsmTable = arg1

    else
        fatalPrintf('Invalid arguments')
    end

    assertType(fsmTable[SYS_ENTER], 'function')

    if getTLSTable().threadKey ~= getMainScriptThreadKey() then
        fatalPrintf('Can not modify quest FSM table other than in main script thread')
    end

    if _RSVD_NAME_questFSMTable == nil then
        _RSVD_NAME_questFSMTable = {}
    end
    _RSVD_NAME_questFSMTable[fsmName] = fsmTable
end

function hasQuestFSM(fsm)
    assertType(fsm, 'string')
    if not _RSVD_NAME_questFSMTable          then return false end
    if not _RSVD_NAME_questFSMTable[fsmName] then return false end
    return true
end

function hasQuestState(arg1, arg2)
    assertType(arg1, 'string')
    assertType(arg2, 'string', 'nil')

    local fsmName = nil
    local state   = nil

    if arg2 == nil then
        fsmName = SYS_QSTFSM
        state   = arg1
    else
        fsmName = arg1
        state   = arg2
    end

    if not _RSVD_NAME_questFSMTable                 then return false end
    if not _RSVD_NAME_questFSMTable[fsmName]        then return false end
    if not _RSVD_NAME_questFSMTable[fsmName][state] then return false end
    return true
end

function setUIDQuestState(fargs)
    assertType(fargs, 'table')
    assertType(fargs.uid, 'integer')
    assertType(fargs.fsm, 'string', 'nil')
    assertType(fargs.state, 'string')
    assertType(fargs.exitfunc, 'function', 'nil')

    local uid   = fargs.uid
    local fsm   = fargs.fsm or SYS_QSTFSM
    local state = fargs.state

    if (not hasQuestState(fsm, state)) and (state ~= SYS_DONE) then
        fatalPrintf('Invalid arguments: fsm %s, state %s', fsm, state)
    end

    -- don't save team member list here
    -- a player can be in a team but still start a single-role quest alone

    if (fsm == SYS_QSTFSM) and (state == SYS_DONE) then
        local npcBehaviors = dbGetUIDQuestField(uid, 'fld_npcbehaviors')
        if npcBehaviors then
            for _, v in pairs(npcBehaviors) do
                clearNPCQuestBehavior(v[1], v[2], uid)
            end
        end
        _RSVD_NAME_dbSetUIDQuestStateDone(uid)
    else
        dbSetUIDQuestState(fsm, uid, state, fargs.args)
    end

    local currFSMName = _RSVD_NAME_currFSMName
    assertType(currFSMName, 'string')

    _RSVD_NAME_closeUIDQuestState(uid, fsm)
    if hasQuestState(fsm, state) then
        runQuestThread(function()
            _RSVD_NAME_currFSMName = fsm
            _RSVD_NAME_enterUIDQuestState(uid, fsm, state, fargs.args, false)
            if args.exitfunc then
                runQuestThread(args.exitfunc)
            end
        end)
    end

    -- drop current thread in C layer
    -- next state will be executed in a new thread

    if currFSMName == fsm then
        while true do
            coroutine.yield()
        end
    end
end

function setUIDQuestDesp(args)
    assertType(args, 'table')
    assertType(args.uid, 'integer')
    assertType(args.fsm, 'string', 'nil')

    local uid = args.uid
    local fsm = args.fsm or SYS_QSTFSM

    if args.format == nil and #args == 0 then
        return _RSVD_NAME_setUIDQuestDesp(uid, fsm or SYS_QSTFSM, nil)
    end

    if args.format ~= nil then
        assertType(args.format, 'string')
        return _RSVD_NAME_setUIDQuestDesp(uid, fsm or SYS_QSTFSM, string.format(args.format, table.unpack(args, 1, #args)))
    end

    assertType(args[1], 'string')
    return _RSVD_NAME_setUIDQuestDesp(uid, fsm or SYS_QSTFSM, string.format(table.unpack(args, 1, #args)))
end

-- setup NPC chat logics
-- also save to database for next time loading, usage:
--
--     setupNPCQuestBehavior('仓库_1_007', '大老板_1', uid,
--     [[
--         return getUID(), getQuestName()
--     ]],
--
--     [[
--         local questUID, questName = ...
--         local questPath = {SYS_EPUID, questName}
--
--         return
--         {
--             [SYS_ENTER] = function(uid, value)
--                 uidPostXML(uid, questPath,
--                 [=[
--                     <layout>
--                         <par>是士官派你来的？</par>
--                         <par>嗯，那么先吩咐你做件简单的事儿吧！你能去把这个护身符交给武器库的<t color="red">阿潘</t>道友吗？</par>
--                         <par></par>
--                         <par><event id="npc_accept_quest">好的！</event></par>
--                     </layout>
--                 ]=])
--             end,
--
-- argstr shouldn't capture current environ's values
-- argstr get evalulated everytime when when setup the NPC behavior
--
function setupNPCQuestBehavior(mapName, npcName, uid, arg1, arg2)
    assertType(mapName, 'string')
    assertType(npcName, 'string')

    assertType(uid, 'integer')
    assert(uid > 0)

    local argstr = nil
    local code   = nil

    if type(arg1) == 'string' and type(arg2) == 'string' then
        argstr = arg1
        code   = arg2

    elseif type(arg1) == 'string' and arg2 == nil then
        argstr = nil
        code   = arg1

    elseif arg1 == nil and type(arg2) == 'string' then
        argstr = nil
        code   = arg2

    else
        fatalPrintf('Invalid arguments to setupNPCQuestBehavior(%s, %s, %d, ...)', asInitString(mapName), asInitString(npcName), uid)
    end

    -- re-evalulate argstr to capture current environ's values
    -- don't save the environ's specific value to database, which may causes error for next time loading

    local args = argstr and table.pack(load(argstr)()) or table.pack()
    args[args.n + 1] = string.format([[ setUIDQuestHandler(%d, %s, load(%s)(...)) ]], uid, asInitString(getQuestName()), asInitString(code))

    -- use array as {code, argstr}
    -- argstr can be nil, put ahead may cause trouble

    uidRemoteCall(getNPCharUID(mapName, npcName), table.unpack(args, 1, args.n + 1))
    _RSVD_NAME_dbUpdateUIDQuestFieldTable(uid, 'fld_npcbehaviors', strAny({mapName, npcName}), {mapName, npcName, code, argstr})
end

function clearNPCQuestBehavior(mapName, npcName, uid)
    assertType(mapName, 'string')
    assertType(npcName, 'string')

    assertType(uid, 'integer')
    assert(uid > 0)

    uidRemoteCall(getNPCharUID(mapName, npcName), uid, getQuestName(), [[ deleteUIDQuestHandler(...) ]])
    _RSVD_NAME_dbUpdateUIDQuestFieldTable(uid, 'fld_npcbehaviors', strAny({mapName, npcName}), nil)
end

function runNPCEventHandler(npcUID, playerUID, eventPath, event, value)
    getTLSTable().threadKey = getThreadKey()
    getTLSTable().threadSeqID = getThreadSeqID()

    uidRemoteCall(npcUID, playerUID, eventPath, event, value,
    [[
        local playerUID, eventPath, event, value = ...
        runEventHandler(playerUID, eventPath, event, value)
    ]])
end

function _RSVD_NAME_enterUIDQuestState(uid, fsm, state, args, restore)
    assertType(uid, 'integer')
    assertType(fsm, 'string')
    assertType(state, 'string')
    assertType(restore, 'boolean')

    if not hasQuestState(fsm, state) then
        fatalPrintf('Invalid quest: fsm %s, state %s', fsm, state)
    end

    if restore then
        local npcBehaviors = dbGetUIDQuestField(uid, 'fld_npcbehaviors')
        assertType(npcBehaviors, 'nil', 'table')

        if npcBehaviors then
            for _, v in pairs(npcBehaviors) do
                setupNPCQuestBehavior(v[1], v[2], uid, v[4], v[3])
            end
        end
    end

    _RSVD_NAME_questFSMTable[fsm][state](uid, args)
end

local _RSVD_NAME_triggers = {}
function addQuestTrigger(triggerType, callback)
    assertType(triggerType, 'integer')
    assertType(callback, 'function')

    assert(triggerType >= SYS_ON_BEGIN, triggerType)
    assert(triggerType <  SYS_ON_END  , triggerType)

    if not _RSVD_NAME_triggers[triggerType] then
        _RSVD_NAME_triggers[triggerType] = {}
        _RSVD_NAME_callFuncCoop('modifyQuestTriggerType', triggerType, true)
    end

    table.insert(_RSVD_NAME_triggers[triggerType], callback)
end

function _RSVD_NAME_trigger(triggerType, uid, ...)
    assertType(triggerType, 'integer')
    assertType(uid        , 'integer')

    assert(triggerType >= SYS_ON_BEGIN, triggerType)
    assert(triggerType <  SYS_ON_END  , triggerType)

    local args = table.pack(...)
    local config = _RSVD_NAME_triggerConfig(triggerType)

    if not config then
        fatalPrintf('Can not process trigger type %d', triggerType)
    end

    if args.n > #config[2] then
        addLog(LOGTYPE_WARNING, 'Trigger type %s expected %d parameters, got %d', config[1], #config[2] + 1, args.n + 1)
    end

    for i = 1, #config[2] do
        if type(args[i]) ~= config[2][i] and math.type(args[i]) ~= config[2][i] then
            fatalPrintf('The %d-th parmeter of trigger type %s expected %s, got %s', i + 1, config[1], config[2][i], type(args[i]))
        end
    end

    if config[3] then
        config[3](args)
    end

    local doneKeyList = {}
    for triggerKey, triggerFunc in pairs(_RSVD_NAME_triggers[triggerType]) do
        local result = triggerFunc(uid, table.unpack(args, 1, #config[2]))
        if type(result) == 'boolean' then
            if result then
                table.insert(doneKeyList, triggerKey)
            end
        elseif type(result) ~= 'nil' then
            table.insert(doneKeyList, triggerKey)
            addLog(LOGTYPE_WARNING, 'Trigger %s callback returns invalid type %s, key %s removed.', config[1], type(result), tostring(triggerKey))
        end
    end

    for _, key in ipairs(doneKeyList) do
        _RSVD_NAME_triggers[triggerType][key] = nil
    end

    if tableEmpty(_RSVD_NAME_triggers[triggerType]) then
        _RSVD_NAME_triggers[triggerType] = nil
        _RSVD_NAME_callFuncCoop('modifyQuestTriggerType', triggerType, false)
    end
end

--
-- )###"

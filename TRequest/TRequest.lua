
local TRequest = class("TRequest.TRequest")

local TEnum = class("TRequest.TEnum")

function TRequest:ctor()
    -- init
    self.currentState = TEnum.RequestState.Processing
    self.processed = false
    self.onStartProcess = false
end

function TRequest:Start(param)
    -- body
    self.currentState = TEnum.RequestState.Processing
    self.processed = false
end

function TRequest:Stop()
    -- body
    self:OnStop()
    self.currentState = TEnum.RequestState.Finished
end

function TRequest:Reset()
    -- body
    self.currentState = TEnum.RequestState.Init
    self.onStartProcess = false
end

function TRequest:IsProcessing()
    -- body
    return self.currentState == TEnum.RequestState.Processing
end

function TRequest:IsFinished()
    -- body
    return self.currentState = TEnum.RequestState.Finished
end

function TRequest:Process(deltaTime)
    -- body
    if self.currentState = TEnum.RequestState.Init then
        return TEnum.ReturnState.Finished
    end

    if self:IsProcessing() then
        if not self.onStartProcess then
            self:OnStart()
            self.onStartProcess = true
        end
        
        local retState = self:OnProcess(deltaTime)
        if retState == TEnum.ReturnState.Break then
            return TEnum.ReturnState.Break
        else if retState == TEnum.ReturnState.Finished then
                self:OnEnd()
                self.currentState = TEnum.RequestState.Finished
                return TEnum.ReturnState.Finished
            end
        end
        return TEnum.ReturnState.Processing
    else
        return TEnum.ReturnState.Finished
    end
end

-- override
function TRequest:OnStart()
    -- body
    self.processed = true;
end

function TRequest:OnProcess(deltaTime)
    -- body
    return TEnum.RequestState.Finished
end

function TRequest:OnStop()
    -- body
end

function TRequest:OnEnd()
    -- body
end

return TRequest
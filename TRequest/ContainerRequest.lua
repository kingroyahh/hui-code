
local ContainerRequest = class("TRequest.ContainerRequest", require("TRequest.TRequest"))

local super = ContainerRequest.super

local TEnum = class("TRequest.TEnum")

function ContainerRequst:ctor(param)
    -- body
    super.ctor(self, param)
    self.requestList = {}
end

function ContainerRequst:AddHeadRequest(req)
    -- body
    table.insert(self.requestList, 1, req)
end

function ContainerRequst:AddRequest(req)
    -- body
    table.insert(self.requestList, req)
end

function ContainerRequst:Start(param)
    -- body
    for k,v in pairs(self.requestList) do
        if v ~= nil then
            v:Start(param)
        end
    end
    super.Start(self)
end

function ContainerRequst:Stop()
    -- body
    for k,v in pairs(self.requestList) do
        if v ~= nil then
            v:Stop(param)
        end
    end
    super.Stop(self)
end

function ContainerRequst:IsFinished()
    -- body
    if self.currentState == TEnum.RequestState.Init then
        return false
    else if self.currentState == TEnum.RequestState.Processing then
            for k,v in pairs(self.requestList) do
                if v ~= nil then
                    if not v:IsFinished() then
                        return false
                    end
                end
            end
            self.currentState = TEnum.RequestState.Finished
        end
    end
    return true
end

function ContainerRequst:Clear()
    -- body
    self.requestList = {}
end

return ContainerRequst
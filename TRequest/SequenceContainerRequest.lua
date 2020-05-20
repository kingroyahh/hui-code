--顺序执行容器
local SequenceContainerRequest = class("TRequest.SequenceContainerRequest", require("TRequest.ContainerRequst"))
local super = SequenceContainerRequest.super

local TEnum = require("TRequest.TEnum")

function SequenceContainerRequest:ctor()
    -- body
    super.ctor(self)
    self.listIndex = 1
end

function SequenceContainerRequest:OnProcess(deltaTime)
    -- body
    local nowReq = self.requestList[self.listIndex]

    if nowReq == nil then
        return TEnum.ReturnState.Finished
    end

    if nowReq:IsFinished() then
        self.listIndex = self.listIndex + 1
    else
        if nowReq:IsProcessing() then
            local retState = nowReq:Process(deltaTime)
            if retState == TEnum.ReturnState.Break then
                return TEnum.ReturnState.Finished
            else if retState == TEnum.ReturnState.Finished then
                    self.listIndex = self.listIndex + 1
                    if self.requestList[self.listIndex] == nil then
                        return TEnum.ReturnState.Finished
                    end
                else
                    return TEnum.ReturnState.Processing
                end
            end
        else
            return TEnum.ReturnState.Finished
        end
    end

    if self.listIndex <= #self.requestList then
        return TEnum.ReturnState.Processing
    end

    return TEnum.ReturnState.Finished
end

return SequenceContainerRequest
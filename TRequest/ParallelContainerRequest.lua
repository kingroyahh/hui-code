--并行执行容器
local ParallelContainerRequest = class("TRequest.ParallelContainerRequest", require("TRequest.ContainerRequst"))
local super = ParallelContainerRequest.super

local TEnum = require("TRequest.TEnum")

function ParallelContainerRequest:ctor()
    -- body
    super.ctor(self)
end

function ParallelContainerRequest:OnProcess(deltaTime)
    -- body
    local retState = TEnum.ReturnState.Finished

    if self.requestList ~= nil then
        for k,v in pairs(self.requestList) do
            if v ~= nil then
                if not v:IsFinished() then
                    if v:IsProcessing() then
                        local ret = v:Process(deltaTime)
                        if ret = TEnum.ReturnState.Finished then

                        else
                            retState = TEnum.ReturnState.Processing
                        end
                    else
                        retState = TEnum.ReturnState.Processing
                    end
                end
            end
        end
    end

    return retState
end

return ParallelContainerRequest
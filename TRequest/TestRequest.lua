--测试类
local TestRequest = class("TRequest.TestRequest", require("TRequest.ContainerRequst"))
local super = TestRequest.super

local TEnum = require("TRequest.TEnum")
local processTime = 5

function TestRequest:ctor(param)
    -- body
    self.param = param
end

function TestRequest:OnStart()
    -- body
    super.OnStart(self)
    self.timeStamp = 0
    print("request start " .. self.param)
end

function TestRequest:OnProcess(deltaTime)
    -- body
    if self.timeStamp > processTime then
        return TEnum.ReturnState.Finished
    end

    self.timeStamp = self.timeStamp + deltaTime
    return TEnum.ReturnState.Processing
end

function TestRequest:OnEnd()
    -- body
    super.OnEnd(self)
    print("request end " .. self.param)
end

return TestRequest
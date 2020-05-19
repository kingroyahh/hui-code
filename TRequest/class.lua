Reflect = {}

function clone(object)
    local lookup_table = {}
    local function _copy(object)
        if type(object) ~= "table" then
            return object
        elseif lookup_table[object] then
            return lookup_table[object]
        end
        local new_table = {}
        lookup_table[object] = new_table
        for key, value in pairs(object) do
            new_table[_copy(key)] = _copy(value)
        end
        return setmetatable(new_table, getmetatable(object))
    end
    return _copy(object)
end

function is(ins, cls)
    if ins == nil then
        return false
    end
    if ins.__cname == nil or cls.__cname == nil then
        return false
    end
    if ins.__cname == cls.__cname then
        return true
    else
        return is(ins.super, cls)
    end
end

--Create a class.
function class(classname, super)
    local superType = type(super)
    local cls

    if superType ~= "function" and superType ~= "table" then
        superType = nil
        super = nil
    end

    if superType == "function" or (super and super.__ctype == 1) then
        -- inherited from native C++ Object
        cls = {}

        if superType == "table" then
            -- copy fields from super
            for k, v in pairs(super) do
                cls[k] = v
            end
            cls.__create = super.__create
            cls.super = super
        else
            cls.__create = super
        end

        cls.ctor = function()
        end
        cls.__cname = classname
        cls.__ctype = 1

        function cls.new(...)
            local instance = cls.__create(...)
            -- copy fields from class to native object
            for k, v in pairs(cls) do
                instance[k] = v
            end
            instance.class = cls
            instance:ctor(...)
            return instance
        end

    else
        -- inherited from Lua Object
        if super then
            if Reflect[super.__cname] == super then
                -- inherited from Lua class
                cls = {}
                setmetatable(cls, super)
            else
                -- inherited from Lua table
                cls = clone(super)
            end
            cls.super = super
        else
            cls = { ctor = function()
            end }
        end


        cls.__cname = classname
        cls.__ctype = 2 -- lua
        cls.__index = cls

        function cls.new(...)
            local instance = {}
            instance.class = cls
            setmetatable(instance, cls)
            instance:ctor(...)
            instance.is = is
            return instance
        end
    end
    if Reflect[classname] ~= nil then
        logWarning("the name " .. classname .. " is used multy times.")
    end
    Reflect[classname] = cls
    return cls
end

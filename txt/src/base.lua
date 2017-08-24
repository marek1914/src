--http://www.lua.org/   RIO
--nodemcu/openwrt使用
--http://www.eluaproject.net/
--Quickly prototype and develop embedded software applications with the power of Lua and 
--run them on a wide range of microcontroller architectures

--luarocks install lpack  md5 luafilesystem
--lua  elua的关系

--编译elua：
--lua build_elua.lua board=MIZAR32

--apt-get install luarocks
--luarocks install luafilesystem
--luarocks install lpack
--luarocks install md5


dofile("dir.lua")

function startup()
    if file.open("init.lua") == nil then
        print("init.lua deleted or renamed")
    else
        print("Running")
        file.close("init.lua")
        -- the actual application is stored in 'application.lua'
        -- dofile("application.lua")
    end
end

print("Connecting to WiFi access point...")
wifi.setmode(wifi.STATION)
wifi.sta.config('gaojie', 'gaojie6278129')
-- wifi.sta.connect() not necessary because config() uses auto-connect=true by default
tmr.alarm(1, 1000, 1, function()
    if wifi.sta.getip() == nil then
        print("Waiting for IP address...")
    else
        tmr.stop(1)
        print("WiFi connection established, IP address: " .. wifi.sta.getip())
        print("You have 3 seconds to abort")
        print("Waiting...")
        tmr.alarm(0, 3000, 0, startup)
    end
end)

--  

-- List all files
print("Listing all files on ESP8266 device:")
l = file.list();
for k,v in pairs(l) do
  print(k.. "\t"..v.." Bytes")
end
l = nil

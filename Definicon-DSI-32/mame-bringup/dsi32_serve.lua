local stage=0
local output={}
emu.register_periodic(function()
 local mac=manager.machine
 local cpu=mac.devices[":maincpu"]
 local prog=cpu.spaces["program"]
 local iosp=cpu.spaces["io"]
 local t=mac.time.seconds
 local function out(p,v) iosp:write_u8(p,v) end
 local function wb(a,v) prog:write_u8(a,v) end
 local function rb(a) return prog:read_u8(a) end
 local function rw(a) return rb(a)+256*rb(a+1) end
 local function rd(a) return rw(a)+65536*rw(a+2) end
 local function ww(a,v) wb(a,v%256); wb(a+1,math.floor(v/256)%256) end
 local function rb32(adr)  -- read a 32032-space byte via the window
  out(0x160, math.floor(adr/65536))
  local v=rb(0xE0000+(adr%65536))
  out(0x160,0)
  return v
 end

 if stage==0 and t>2 then stage=1
  out(0x160,0); out(0x150,0x05)
 elseif stage==1 then stage=2
  out(0x150,0x01)
 elseif stage==2 then stage=3
  local f=io.open("/tmp/dsi32-ram.img","rb")
  local img=f:read("*a"); f:close()
  for page=0,15 do
   out(0x160,page)
   local base=page*65536
   for i=0,65535 do
    local b=string.byte(img, base+i+1)
    if b~=0 then wb(0xE0000+i, b) end
   end
  end
  out(0x160,0)
 elseif stage==3 then stage=4
  wb(0xE2004,0xFF); wb(0xE2005,0xFF)
  out(0x150,0x05)
 elseif stage==4 then stage=5
  out(0x150,0x00)
 elseif stage==5 then stage=6
  wb(0xE2000,0); wb(0xE2001,0)
  wb(0xE2004,1); wb(0xE2005,0)
 elseif stage==6 then
  -- the LOADer service loop, console-write subset
  local sw86=rw(0xE2000)
  if sw86==4 then
   local dskrw=rw(0xE2014)
   if dskrw==6 then
    local fd=rw(0xE2016)
    local n=rw(0xE2018)
    local dta=rd(0xE201A)
    local s={}
    for i=0,n-1 do
     local x=rb32(dta+i)
     s[#s+1]=(x>=32 and x<127) and string.char(x) or ((x==13 or x==10) and "\n" or ".")
    end
    output[#output+1]=table.concat(s)
    print(string.format("[write fd=%d n=%d] %s", fd, n, table.concat(s)))
    ww(0xE201E,n)      -- dskrc = bytes written
    ww(0xE2030,0)      -- dskerr = 0
    ww(0xE2014,0)      -- dskrw = 0 (disk_ser does this)
   elseif dskrw==5 then -- read request (keyboard): answer with CR after a while
    if t>12 then
     local dta=rd(0xE201A)
     out(0x160,math.floor(dta/65536)); wb(0xE0000+(dta%65536), 13); out(0x160,0)
     ww(0xE201E,1); ww(0xE2030,0); ww(0xE2014,0)
    else
     return -- leave pending
    end
   else
    print(string.format("[disk op %d - completing with error 0]", dskrw))
    ww(0xE201E,0); ww(0xE2030,0); ww(0xE2014,0)
   end
   ww(0xE2000,0)  -- sw86 = 0: request complete
  elseif sw86~=0 then
   print(string.format("[sw86=%d v1=%04x v2=%04x - null service]", sw86, rw(0xE2032), rw(0xE2036)))
   ww(0xE2000,0)
  end
 end
end)

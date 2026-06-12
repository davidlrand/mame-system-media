local phase=0
local lastact=-10
local dumps={}
local lastr="" 
emu.register_periodic(function()
 local mac=manager.machine
 local t=mac.time.seconds
 local prog=mac.devices[":maincpu"].spaces["program"]
 local rows={}
 for r=0,24 do local l={} for c=0,79 do local x=prog:read_u8(0xB8000+(r*80+c)*2)
  l[#l+1]=(x>=32 and x<127) and string.char(x) or ' ' end rows[#rows+1]=table.concat(l) end
 local all=table.concat(rows,'\n')
 do
  local c=mac.devices[":isa3:pd32:cpu"]
  local k5=math.floor(t/5)
  if c and not dumps['pc'..k5] then dumps['pc'..k5]=true
   print(string.format("PC32 t=%d %06x", t, c.state["PC"].value)) end
 end
 do
  local c=mac.devices[":isa3:pd32:cpu"]
  if c then
   local pc=c.state["PC"].value
   local r
   if pc<0x100 then r="vec/stub"
   elseif pc<0x340 then r="loaderloop"
   elseif pc<0x800 then r="rom-high"
   elseif pc<0x20000 then r="KERNEL"
   else r="weeds" end
   if r~=lastr then lastr=r
    print(string.format("TRAJ t=%.2f pc=%06x %s", mac.time:as_double(), pc, r))
   end
  end
 end
 if t>=130 and t<=140 then
  local c=mac.devices[":isa3:pd32:cpu"]
  if c then
   local pc=c.state["PC"].value
   pchist=pchist or {}
   pchist[pc]=(pchist[pc] or 0)+1
  end
 end
 if t>140 and pchist and not pchist.done then pchist.done=true
  local arr={}
  for pc,n in pairs(pchist) do if pc~="done" and type(pc)=="number" then arr[#arr+1]={pc,n} end end
  table.sort(arr,function(a,b) return a[2]>b[2] end)
  for k2=1,math.min(#arr,12) do print(string.format("HIST %06x x%d", arr[k2][1], arr[k2][2])) end
 end

 if t>=295 and not dumps.proctab then dumps.proctab=true
  local c=manager.machine.devices[":isa3:pd32:cpu"]
  if c then
   local sp=c.spaces["program"]
   local function rd32(a) return sp:read_u8(a)|(sp:read_u8(a+1)<<8)|(sp:read_u8(a+2)<<16)|(sp:read_u8(a+3)<<24) end
   for p=0,3 do
    local base=0x2f19c+p*0x40
    local l={}
    for i=0,15 do l[#l+1]=string.format("%08x",rd32(base+i*4)) end
    print(string.format("PROC%d @%05x: %s",p,base,table.concat(l," ")))
   end
  end
 end

 if t>=100 and not dumps.taps then dumps.taps=true
  local c=manager.machine.devices[":isa3:pd32:cpu"]
  if c then
   local sp=c.spaces["program"]
   tapseen={}
   taplist={
    {0x11982,"getxfile"},{0x11c69,"get410"},{0xfeb8,"xalloc"},
    {0xbb1e,"allocreg"},{0xbcf8,"attachreg"},{0xcc22,"mapreg"},
    {0xcb11,"loadreg"},{0x139ae,"rexit"},{0xe4b4,"sleep"},
    {0xfb90,"swtch-CONTROL"},{0x466,"idle-CONTROL"}}
   taps={}
   for _,e in ipairs(taplist) do
    local a,n=e[1],e[2]
    taps[#taps+1]=sp:install_read_tap(a,a+1,"t"..n,function(off,data,mask)
     if not tapseen[n] then tapseen[n]=true print(string.format("TAP %s t=%.2f",n,manager.machine.time:as_double())) end
    end)
   end
   print("TAPS installed")
  end
 end
 local k=math.floor(t/120)
 if not dumps[k] then dumps[k]=true
  print(string.format("=== t=%d phase=%d",t,phase))
  for _,line in ipairs(rows) do local s=line:gsub('%s+$','') if #s>0 then print('|'..s) end end
 end
 if t-lastact<4 then return end
 local function act(x) mac.natkeyboard:post(x) lastact=t end
 local bare=false
 for _,r in ipairs(rows) do if r:match('^[AC]>%s*$') then bare=true end end
 if all:find('RESUME = "F1"') then mac.natkeyboard:post_coded("{F1}") lastact=t
 elseif all:find('Enter new date') and phase==0 then act("\n")
 elseif all:find('Enter new time') and phase==0 then act("\n") phase=1
 elseif bare and phase==1 then act("u\n") phase=4
 elseif phase==4 and all:find('INIT') and t-lastact>8 then act("ls /\n") phase=5
 elseif phase==5 and t-lastact>25 then act("ps -e\n") phase=6
 end
end)

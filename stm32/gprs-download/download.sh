mkdir -p split
curl -I http://123.56.196.100/device/updateFile/split/$1 >split/$1
curl http://123.56.196.100/device/updateFile/split/$1 >>split/$1

if [ "null$1" == "null-h"  ] ; then
	echo "help: "
	echo "    : freqtemp.sh         -- print only"
	echo "    : freqtemp.sh >file   -- save to file"
	exit
fi

while true; do 
	echo nowtime: `date "+%y-%m-%d %T"`;
  
	if [ -e "/sys/class/kgsl/kgsl-3d0/devfreq/cur_freq"  ] ; 
	then gpufreq=`cat /sys/class/kgsl/kgsl-3d0/devfreq/cur_freq`;  
	else gpufreq=0; 
	fi;

	echo gpufreq: $gpufreq

    base=/sys/devices/system/cpu	
	file=cpufreq/scaling_cur_freq
	index=0

	for tmp in ${base}/cpu?/${file}; do
        cpufreq[$index]=`cat $tmp`
		((index++))
	done
	
	echo cpufreq 0-7:	
	echo ${cpufreq[@]}
    
	sensor_index=(1 2 3 4 7 8 9 10 12)
	base=/sys/class/thermal/thermal_zone
  	index=0

	for i in ${sensor_index[@]}; do
		cputemp[$index]=`cat $base$i/temp`
		((index++))
	done
	
	echo cputemp 0-7 and gputemp:	
	echo ${cputemp[@]}

	fpsStr=$(logcat -d | grep "svr     : FPS:")
	fps=${fpsStr##*: FPS: }
	if [ "${fps}null" == "null"  ] ;
	then fps="0"
	fi

	echo fps: $fps

    if [ -e "/sys/class/kgsl/kgsl-3d0/thermal_pwrlevel" ] ; 
    then gpu_pwr_lvl=`cat /sys/class/kgsl/kgsl-3d0/thermal_pwrlevel`;  
    else gpu_pwr_lvl=0; 
    fi; 

	capa=`cat /sys/class/power_supply/battery/capacity`;

	gpuloadStr=$(cat /sys/devices/soc/5000000.qcom,kgsl-3d0/kgsl/kgsl-3d0/gpubusy)
	gpuload=$(echo ${gpuloadStr} | busybox awk '{ if ( $2 != 0  ) {printf "%.0f%%", $1 * 100/$2;} else {printf "0%%";} }')

	cpuloadStr=$(busybox top -d 1 -n 1)
    cpuloadStr=${cpuloadStr##* nic }
    cpuloadStr=${cpuloadStr%%"% idle "*}
	cpuload=$(echo ${cpuloadStr} | busybox awk '{printf "%.0f%%", 100.0 - $1}')

	echo gpu_pwr_lvl capa gpuload cpuload:
	echo $gpu_pwr_lvl $capa $gpuload $cpuload:

    sleep 2;      
done

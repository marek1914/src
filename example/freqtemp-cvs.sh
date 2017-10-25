if [ "null$1" == "null-h"  ] ; then
	echo "help: "
	echo "    : freqtemp.sh         -- print only"
	echo "    : freqtemp.sh >file   -- save to file"
	exit
fi

echo -n "gpufreq, cpufreq0, cpufreq1,cpufreq2,cpufreq3,cpufreq4,cpufreq5,cpufreq6,cpufreq7,"
echo    "cputemp0, cputemp1,cputemp2,cputemp3,cputemp4,cputemp5,cputemp6,cputemp7,gputemp, fps, gpu_pwr_lvl, capa, gpuload, cpuload"
while true; do 
  
	if [ -e "/sys/class/kgsl/kgsl-3d0/devfreq/cur_freq"  ] ; 
	then gpufreq=`cat /sys/class/kgsl/kgsl-3d0/devfreq/cur_freq`;  
	else gpufreq=0; 
	fi;

	echo -n $gpufreq,

    base=/sys/devices/system/cpu	
	file=cpufreq/scaling_cur_freq
	index=0

	for tmp in ${base}/cpu?/${file}; do
        cpufreq[$index]=`cat $tmp`
		((index++))
	done
	
	for tmp in ${cpufreq[@]}; do
	  echo -n $tmp,
    done
    
	sensor_index=(8 9 10 11 12 13 14 15 17)
	base=/sys/class/thermal/thermal_zone
  	index=0

	for i in ${sensor_index[@]}; do
		cputemp[$index]=`cat $base$i/temp`
		((index++))
	done

	for tmp in ${cputemp[@]}; do	
	  echo -n $tmp,
	done

	fpsStr=$(logcat -d | grep "svr     : FPS:")
	fps=${fpsStr##*: FPS: }
	if [ "${fps}null" == "null"  ] ;
	then fps="0"
	fi

	echo -n $fps,

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

	echo $gpu_pwr_lvl, $capa, $gpuload, $cpuload

    sleep 2;      
done

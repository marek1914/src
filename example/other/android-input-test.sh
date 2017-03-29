while true
do
input keyevent HOME
input keyevent ENTER
sleep 2
BJ=75
input keyevent KEYCODE_7 
input keyevent KEYCODE_4
input keyevent ENTER
sleep 2
	while [ ${BJ} -lt 100 ]
	do
		input keyevent KEYCODE_DPAD_UP
		input keyevent KEYCODE_BUTTON_14
		sleep 3
		input keyevent ENTER
        input keyevent ENTER
		input keyevent KEYCODE_DPAD_LEFT
		sleep 3
		input keyevent KEYCODE_DPAD_RIGHT
        input keyevent KEYCODE_DPAD_RIGHT
		sleep 3
		input keyevent ENTER
		input keyevent VOLUME_UP
		input keyevent VOLUME_DOWN
		sleep 40
		input keyevent KEYCODE_BACK
		BJ=$(($BJ+1))
	done
done


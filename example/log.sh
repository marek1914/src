logcat -c
logcat -v threadtime -f tmp &
pid=$!
while true; do
    sleep 1
        size=`busybox stat  -c "%s" tmp`
        echo --$size--
        if [ $size -gt 500000 ]; then
            echo "#### $size"
            cp tmp log1.txt
            rm tmp
            kill -9 $pid
            logcat -v threadtime -f tmp &
            pid=$!
        fi
done

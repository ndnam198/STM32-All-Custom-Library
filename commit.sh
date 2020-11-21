#!/bin/sh
git status
timestamp=$(date +"%D %T")
message=$1
git add *
git commit -m "[$timestamp]: $message"
git push https://github.com/ndnam198/STM32-All-Custom-Library.git master
echo "*******************************************************"
echo "Time commit: [$timestamp]"
echo "Commit message: \"$message\""
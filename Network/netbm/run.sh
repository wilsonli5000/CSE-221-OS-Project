localtxt="./log_shut_local.txt"
remotetxt="./log_shut_remote.txt"

if ! [ -f "$localtxt" ]
then
	touch $localtxt
fi

if ! [ -f "$remotetxt" ]
then
	touch $remotetxt
fi

> $localtxt
> $remotetxt

for i in {1..50}
do 
	./client 127.0.0.1 >> $localtxt
	echo "local repeat $i times!"
done

for i in {1..50}
do
	./client 192.168.0.6 >> $remotetxt
	echo "remote repeat $i times!"
done



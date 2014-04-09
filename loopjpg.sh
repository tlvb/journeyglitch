cp $1 /tmp/loop.jpg
f=`basename $1`
./worker c /tmp/loop.jpg /tmp/loop.jpg
./worker d /tmp/loop.jpg /tmp/loop.jpg
./worker b /tmp/loop.jpg /tmp/loop.jpg
./worker c /tmp/loop.jpg /tmp/loop.jpg
./worker c /tmp/loop.jpg /tmp/loop.jpg
./worker c /tmp/loop.jpg /tmp/loop.jpg
./worker d /tmp/loop.jpg /tmp/loop.jpg
./worker b /tmp/loop.jpg /tmp/loop.jpg
./worker c /tmp/loop.jpg /tmp/loop.jpg
./worker d /tmp/loop.jpg /tmp/loop.jpg
./worker b /tmp/loop.jpg /tmp/loop.jpg
./worker s /tmp/loop.jpg /tmp/loop.jpg
./worker d /tmp/loop.jpg /tmp/loop.jpg
./worker b /tmp/loop.jpg /tmp/loop.jpg
mv /tmp/loop.jpg looped-${f}
feh -F looped-${f}

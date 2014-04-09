cp $1 /tmp/loop.png
f=`basename $1`
./worker c /tmp/loop.png /tmp/loop.png
./worker d /tmp/loop.png /tmp/loop.png
./worker b /tmp/loop.png /tmp/loop.png
./worker c /tmp/loop.png /tmp/loop.png
./worker c /tmp/loop.png /tmp/loop.png
./worker c /tmp/loop.png /tmp/loop.png
./worker d /tmp/loop.png /tmp/loop.png
./worker b /tmp/loop.png /tmp/loop.png
./worker c /tmp/loop.png /tmp/loop.png
./worker d /tmp/loop.png /tmp/loop.png
./worker b /tmp/loop.png /tmp/loop.png
./worker s /tmp/loop.png /tmp/loop.png
./worker d /tmp/loop.png /tmp/loop.png
./worker b /tmp/loop.png /tmp/loop.png
mv /tmp/loop.png looped-${f}
feh -F looped-${f}

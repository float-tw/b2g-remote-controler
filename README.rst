B2G remote controler
====================

requirement
-----------

* python 2.7

* wxpython 2.8
  # sudo apt-get install python-wxgtk2.8

build
-----

::

    $ cd target
    $ export CROSS=your/arm-cross-compiler/path/cross-compiler-gcc
    $ make
    $ make install

run
---

connect to device run remote_server

::

    $ adb shell
    root@android:/ # cd /data
    root@android:/ # ./remote_server

use another terminal

::

    $ ./remote_client.py

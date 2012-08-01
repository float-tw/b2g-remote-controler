B2G remote controler
====================

requirement
-----------

* python 2.7

* wxpython 2.8

build
-----

::

    $ cd targe
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

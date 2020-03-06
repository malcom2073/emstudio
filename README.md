#EMStudio

Written by Michael Carpenter (malcom2073@gmail.com), and licensed under MIT. 

A large amount of the code has been reworked to eliminate any remnants of GPL, so at this point the entire code-base is MIT, with the exception of some libraries which are LGPL.

### Overview:

EMStudio is an open source tuning application for engines.

### Binaries:

Binaries are not currently maintained. A build server is in progress and will hopefully be up and running again soon.

### Compiling:

#### Linux:

TODO: Fix this, it is likely out of date for newer versions of Ubuntu 
A few packages are needed to compile EMStudio. In Ubuntu 14.04 or later:
```
$ sudo apt-get update
$ sudo apt-get install git qt5-qmake qt5-default qtscript5-dev libqt5webkit5-dev libqt5serialport5-dev libqt5svg5-dev flite1-dev libssl-dev libudev-dev qtquick1-5-dev freeglut3-dev build-essential
```
Once this has completed (or the equivalent packages on your own distro), you can then clone the repository and build it. 

To checkout and build:
```
$ git clone https://github.com/malcom2073/emstudio.git
$ cd emstudio
$ qmake -r
$ make
$ cd core
$ ./emstudio
```

#### Windows:

TODO: Put together instructions. It's not bad anymore.

##More Information

http://malcom2073.github.com/emstudio/

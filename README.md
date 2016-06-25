# Plasma Runner 

I really liked this runner:
http://kde-apps.org/content/show.php/PidginRunner?content=151998

But It hasn't been updated or ported to KF5, so I did it. Many things 
are still the same and I really liked what he did, So I forked it.


## Installation:


```sh
git clone https://github.com/freexploit/plasma-runner-pidgin.git
cd plasma-runner-pidgin
mkdir build
cd build
cmake ../ \
-DCMAKE_INSTALL_PREFIX=`kf5-config --prefix` \
-DQT_PLUGIN_INSTALL_DIR=`kf5-config --qt-plugins` \
-DCMAKE_BUILD_TYPE=Release
```

Then just look for your contacts on runner.


## Todo:

- Fix images issue
- Add actions
- Create a plasmoid

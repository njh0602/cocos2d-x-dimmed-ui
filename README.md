Cocos2d-x dimmed
============================

A simple, header-only dimmed ui class for cocos2d-x.



## Usage

The implementation is contained in a single header file, `Dimmed.hpp`. Simply copy
the file to a convenient place in your project and include it.

~~~cpp
#include "Dimmed.hpp"

...

auto dimmed = Dimmed::create(size); // create with size
dimmed->setDimmedColor(Color4F::BLACK); // default: (0.0, 0.0, 0.0, 0.5)
dimmed->setSwallowTouches(false); // default: true
dimmed->addDimmedTouchEvent([](Dimmed* self, const Vec2& p){ 
    self->removeFromParent(); 
});
addChild(dimmed, zorder); // zorder: touch priority
~~~

See the `TestScene.cpp` for more example.

![](https://github.com/njh0602/cocos2d-x-dimmed-ui/blob/master/showcase2.png)
![https://github.com/njh0602/cocos2d-x-dimmed-ui](showcase1.gif)



Contributions
-------------

Contributions are welcome. Please use the Github issue tracker.

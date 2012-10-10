//
//  gmu.delays.tap.h
//  gmu.objects
//
//  Created by charles bascou on 22/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef gmu_objects_gmu_delays_tap_h
#define gmu_objects_gmu_delays_tap_h

class gmu_delays_tap {
    
    public:
    gmu_delays_tap(float* _delayline, int _frames, double delay, double amp, double fadetime);
    ~gmu_delays_tap();
    
    
    int compute(float ** buffer, int frames);
    
    int release();
    
    bool is_alive();
    
    
    void randomize_amp(double amount);
    
    void randomize_time(double amount);
    
    // data
    
    double delay;
    long i_delay;
    
    double amp;
    
    float * delayline;
    long delayframes;
    
    // spat
    
    
    // xfade and ramp stuff
    
    
    
    
    // life stuffs
    
    bool local_mode; // if 1 taps are active after "delay" ms after creation
    
    long age;
    long birth; // birth will be set to "delay" when local_mode is on, otherwise will be 0
    long death; // death can be set at creation time otherwise will be 0
    
    bool zombie; // mark as dead
    
};

#endif

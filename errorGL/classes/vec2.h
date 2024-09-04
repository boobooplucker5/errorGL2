#pragma once

class vec2
{
public:
    float x = 0;
    float y = 0;



    void operator=(float a[2])
    {
        x = a[0];
        y = a[1];
   
    }
};
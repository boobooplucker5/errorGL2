#pragma once

    class vec3
    {
    public:
        float x = 0;
        float y = 0;
        float z = 0;


        void operator=(float a[3])
        {
            x = a[0];
            y = a[1];
            z = a[2];
        }

    };



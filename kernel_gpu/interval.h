#ifndef INTERVAL_H_
    #define INTERVAL_H_

struct interval {
    float tmin;
    float tmax;
};

    #define interval_surround(interval, t) ((interval).tmin < (t) && (t) < (interval).tmax)

#endif /* !INTERVAL_H_ */

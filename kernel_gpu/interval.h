#ifndef INTERVAL_H_
    #define INTERVAL_H_


struct interval {
    float tmin;
    float tmax;
};

    #define interval_init(tmin, tmax) {tmin, tmax}
    #define interval_contains(interval, t) ((interval).tmin <= (t) && (t) <= (interval).tmax)
    #define interval_surround(interval, t) ((interval).tmin < (t) && (t) < (interval).tmax)
    #define interval_clamp(t, interval) ((t) < (interval).tmin ? (interval).tmin : (t) > (interval).tmax ? (interval).tmax : (t))

#endif /* !INTERVAL_H_ */

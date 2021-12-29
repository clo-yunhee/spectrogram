#ifndef TRANSFORMCACHE_H
#define TRANSFORMCACHE_H

#include <list>

#include "scaletransform.h"

class TransformCache {
   public:
    TransformCache(int maxCapacity = 4);

    void setMaxCapacity(int maxCapacity);

    const ScaleTransform& get(ScaleTransform::Type type, double sampleRate, int nfft,
                              double minFreq, double maxFreq, int numBins);

   private:
    int m_maxCapacity;
    std::list<ScaleTransform> m_cache;
};

#endif  // TRANSFORMCACHE_H
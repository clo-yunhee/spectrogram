#include "transformcache.h"

#include <QtGlobal>

TransformCache::TransformCache(int maxCapacity) : m_maxCapacity(maxCapacity) {}

const ScaleTransform& TransformCache::get(ScaleTransform::Type type, double sampleRate, int nfft,
                                          double minFreq, double maxFreq, int numBins) {
    // Look for a cached transform with those parameters.
    for (auto it = m_cache.begin(); it != m_cache.end(); ++it) {
        if (it->matches(type, sampleRate, nfft, minFreq, maxFreq, numBins)) {
            return *it;
        }
    }

    // If there is none, we can create it and put it in cache.
    // Truncate cache if needed to keep it at max capacity.

    while (m_cache.size() >= m_maxCapacity) {
        m_cache.pop_back();
    }

    return m_cache.emplace_front(type, sampleRate, nfft, minFreq, maxFreq, numBins);
}
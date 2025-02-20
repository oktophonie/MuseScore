#include "compressor.h"

#include "log.h"

#include "audiomathutils.h"

using namespace mu::audio;
using namespace mu::audio::dsp;

static constexpr float RATIO = 4.f;

Compressor::Compressor(const unsigned int sampleRate)
    : m_filterConfig(sampleRate, RATIO),
    m_feedbackFactor(1.f)   // 0 = feed forward, 1 = feed back
{
}

volume_db_t Compressor::gainSmoothing(const float& newGainReduction) const
{
    float coefficient = 0.f;

    if (newGainReduction <= m_previousGainReduction) {
        coefficient = m_filterConfig.attackTimeCoefficient();
    } else {
        coefficient = m_filterConfig.releaseTimeCoefficient();
    }

    return (coefficient * m_previousGainReduction) + ((1 - coefficient) * newGainReduction);
}

volume_db_t Compressor::computeGain(const volume_db_t& logarithmSample) const
{
    if (logarithmSample < m_softThresholdLower) {
        return logarithmSample;
    }

    if (logarithmSample >= m_softThresholdLower && logarithmSample <= m_softThresholdUpper) {
        return logarithmSample
               + ((((1 / m_filterConfig.ratio()) - 1) * std::pow(logarithmSample - m_softThresholdUpper, 2))
                  / (2 * m_filterConfig.kneeWidth()));
    }

    return m_filterConfig.logarithmicThreshold()
           + ((logarithmSample - m_filterConfig.logarithmicThreshold()) / m_filterConfig.ratio());
}

void Compressor::process(const float linearRms, float* buffer, const audioch_t& audioChannelsCount, const samples_t samplesPerChannel)
{
    float dbGain = dbFromSample(linearRms);

    if (dbGain <= m_filterConfig.minimumOperableLevel()) {
        return;
    }

    // trying to predict the next sample by the previous gain reduction
    dbGain += m_feedbackFactor * m_feedbackGain;

    float dbDiff = computeGain(dbGain) - dbGain;

    m_feedbackGain = dbDiff;
    float gainFact = linearFromDecibels(dbDiff * (1.f + m_feedbackFactor));

    float currentGainReduction = std::min(gainFact, m_previousGainReduction);

    // apply gain
    for (audioch_t audioChNum = 0; audioChNum < audioChannelsCount; ++audioChNum) {
        multiplySamples(buffer, audioChannelsCount, audioChNum, samplesPerChannel, currentGainReduction);
    }

    m_previousGainReduction = currentGainReduction;
}

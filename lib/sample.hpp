#ifndef HQR_LIB_SAMPLE_HPP
#define HQR_LIB_SAMPLE_HPP

using sample_t = double;

constexpr double SAMPLING_FREQUENCY =
#ifdef SAMPFREQ24
  24000.0
#elif defined SAMPFREQ48
  48000.0
#elif defined SAMPFREQ88
  88200.0
#elif defined SAMPFREQ96
  96000.0
#elif defined SAMPFREQ192
  192000.0
#else
  44100.0
#endif
  ;

#endif // HQR_LIB_SAMPLE_HPP

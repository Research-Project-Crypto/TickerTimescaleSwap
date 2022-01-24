#pragma once
#include <cinttypes>

struct candle
{
    std::uint64_t m_timestamp;
    double m_open;
    double m_close;
    double m_high;
    double m_low;
    double m_volume;

    candle() = default;
    candle(double timestamp, double open, double close, double high, double low, double volume)
    {
        m_timestamp = (uint64_t)timestamp;
        m_open = open;
        m_close = close;
        m_high = high;
        m_low = low;
        m_volume = volume;
    }
};
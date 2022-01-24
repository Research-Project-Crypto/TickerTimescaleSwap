#pragma once
#include "common.hpp"
#include "candle.hpp"
#include "logger.hpp"

namespace program
{
    class timescale_swap
    {
        std::filesystem::path m_input_file;
        const char* m_out_dir;

        std::vector<std::unique_ptr<candle>> m_candles;
    public:
        timescale_swap(std::filesystem::path file_path, const char* out_dir) :
            m_input_file(file_path), m_out_dir(out_dir)
        {

        }
        ~timescale_swap()
        {

        }

        const char* file_name()
        {
            return m_input_file.filename().c_str();
        }

        bool read_input_file()
        {
            io::CSVReader<6> input_stream(m_input_file);
            input_stream.read_header(io::ignore_missing_column | io::ignore_extra_column, "event_time", "open", "close", "high", "low", "volume");

            try
            {
                double timestamp, open, close, high, low, volume;
                while (input_stream.read_row(timestamp, open, close, high, low, volume))
                {
                    std::unique_ptr<candle> new_candle = std::make_unique<candle>(timestamp, open, close, high, low, volume);

                    m_candles.push_back(std::move(new_candle));
                }

            }
            catch(const std::exception& e)
            {
                g_log->error("PREPROCESSOR", "Failure while reading csv:\n%s", e.what());

                return false;
            }

            g_log->verbose("PREPROCESSOR", "Loaded %d candles from %s", m_candles.size(), this->file_name());

            return true;
        }

        void start(const size_t timescale = 60)
        {
            this->read_input_file();

            std::vector<std::unique_ptr<candle>> rescaled_candles;
            rescaled_candles.reserve(static_cast<int>(m_candles.size() / timescale));

            std::unique_ptr<candle> rescaled_candle;
            for (size_t i = 0; i < m_candles.size(); i++)
            {
                std::unique_ptr<candle>& tmp_candle = m_candles.at(i);
                if (i && i % timescale == 0)
                {
                    rescaled_candle->m_close = tmp_candle->m_close;
                    rescaled_candle->m_timestamp = tmp_candle->m_timestamp;

                    rescaled_candles.push_back(std::move(rescaled_candle)); // moving our unique_ptr will make it a nullptr thus allowing us to create a new empty candle
                }

                if (rescaled_candle == nullptr)
                {
                    rescaled_candle = std::make_unique<candle>();

                    rescaled_candle->m_open = tmp_candle->m_open;
                    rescaled_candle->m_low = tmp_candle->m_low;
                }

                if (rescaled_candle->m_low > tmp_candle->m_low)
                    rescaled_candle->m_low = tmp_candle->m_low;
                if (rescaled_candle->m_high < tmp_candle->m_high)
                    rescaled_candle->m_high = tmp_candle->m_high;
                
                rescaled_candle->m_volume += tmp_candle->m_volume;
            }

            this->write_binary_out(&rescaled_candles);
        }

        void write_binary_out(std::vector<std::unique_ptr<candle>>* candles)
        {
            std::string out_dir = m_out_dir / m_input_file.stem();
            std::ofstream output_stream(out_dir + ".bin", std::ios::binary | std::ios::trunc);

            for (const std::unique_ptr<candle>& candle_struct : *candles)
                output_stream.write((char*)candle_struct.get(), sizeof(candle));

            output_stream.close();
        }
    };
}
#ifndef __UTILS_H__
    #define __UTILS_H__

    #include "main_header.h"

    namespace utils
    {
        /**
         * @brief Get the checksum of a string
         * 
         * @param data The data string
         * 
         * @return uint checksum value 
        **/ 
        static inline uint make_Checksum(string data)
        {
            uint checksum{0};

            for (char character : data)
                checksum += int(character);

            return (checksum % 7);
        }

        /**
         * @brief Tests bit errors
         * 
         * @param data          The data string
         * @param recv_checksum Cheksum received
         * 
         * @return boolean value if the checksum is different
        **/ 
        static inline bool test_Checksum(string data, uint recv_checksum)
        {
            return (make_Checksum(data) == recv_checksum);
        }

        /**
         * @brief Complete a number with zeros to the left so that it has n digits
         * 
         * @param number    An integer
         * @param n         Number of digits
         * 
         * @return string with n digits
        **/ 
        static inline string complete_Bytes(uint number, uint n)
        {
            string numb = std::to_string(number);
            uint complete = n - numb.size();

            while(complete--)
                numb.insert(numb.begin(), '0');

            return numb;
        }

        /**
         * @brief Fills a string with zeros
         * 
         * @param data      The data string (char*)
         * @param padding   Number of zeros
         * 
        **/ 
        static inline void fill_Zeros(char* data, uint padding)
        {
            for ( size_t j = MAX_DATA_SIZE - padding; j < MAX_DATA_SIZE; ++j)
            {
                data[j] = '0';
            }
        }
    }

    namespace EWMA
    {
        static inline std::vector<uint> history(1, 0.0);
        static inline int cur_idx = 1;
        const double beta = 0.2;

        static inline double get_next_value(double actual_value)
        {
            history.push_back(beta*history[cur_idx - 1] + (1-beta)*actual_value);
            return history[cur_idx++];
        }
    }

    namespace CRC
    {
        const unsigned int r = 12;
        const std::vector<bool> generator = {true, false, false, false, false, false, false, false, false, true, false, true, true};
        const std::vector<bool> null_divisor = {false, false, false, false, false, false, false, false, false, false, false, false, false};

        static inline std::vector<bool> XOR(const std::vector<bool> &one, const std::vector<bool> &two)
        {
            std::vector<bool> ans;
            for (size_t i = 1; i < two.size(); i++)
            {
                ans.push_back(one[i] ^ two[i]);
            }
            return ans;
        }

        static inline void append_zeros(std::vector<bool> &data)
        {
            for (size_t i = 0; i < r; i++)
            {
                data.push_back(false);
            }
        }

        static inline void append_vector(std::vector<bool> &data, const std::vector<bool> &to_append)
        {
            for (size_t i = 0; i < to_append.size(); i++)
            {
                data.push_back(to_append[i]);
            }
        }

        static inline std::vector<bool> to_boolean(char character)
        {
            std::vector<bool> ans;
            int idx = (int)character;
            for (size_t i = 0; i < CHAR_BITS_SIZE; i++)
            {
                int mask = 1 << i;
                ans.push_back(idx & mask);
            }
            return ans;
        }

        static inline std::vector<bool> to_dividend(std::string data)
        {
            std::vector<bool> dividend;
            for (size_t i = 0; i < data.size(); i++)
            {
                append_vector(dividend, to_boolean(data[i]));
            }
            return dividend;
        }

        static inline std::vector<bool> divide(const std::vector<bool> &dividend, const std::vector<bool> &divisor)
        {
            int cur_top = divisor.size();
            int dividend_size = dividend.size();

            std::vector<bool> chunk(dividend.begin(), dividend.begin() + divisor.size());
            while (cur_top < dividend_size)
            {
                if (chunk[0])
                {
                    chunk = XOR(chunk, divisor);
                    chunk.push_back(dividend[cur_top]);
                }
                else
                {
                    chunk = XOR(chunk, null_divisor);
                    chunk.push_back(dividend[cur_top]);
                }
                cur_top++;
            }

            if (chunk[0])
                chunk = XOR(chunk, divisor);
            else
                chunk = XOR(chunk, null_divisor);
            return chunk;
        }

        static inline int get_key(const std::vector<bool> &remainder)
        {
            int ans = 0;
            for (size_t i = 0; i < remainder.size(); i++)
            {
                if (remainder[i])
                    ans += (1 << i);
            }
            return ans;
        }

        static inline std::vector<bool> get_booleans(const int key)
        {
            std::vector<bool> ans;
            for(size_t i = 0; i < r; i++)
            {
                int mask = 1 << i;
                if(mask & key) ans.push_back(true);
                else ans.push_back(false);
            }
            return ans;
        }

        static inline uint encode(std::string data)
        {
            std::vector<bool> dividend = CRC::to_dividend(data);
            append_zeros(dividend);
            std::vector<bool> remainder = CRC::divide(dividend, CRC::generator);
            return CRC::get_key(remainder);
        }

        static inline uint decode(std::string data, const int key)
        {
            std::vector<bool> dividend = CRC::to_dividend(data);
            std::vector<bool> tail = CRC::get_booleans(key);
            append_vector(dividend, tail);
            std::vector<bool> remainder = CRC::divide(dividend, CRC::generator);
            return CRC::get_key(remainder);
        }
    }

#endif // !__UTILS_H__
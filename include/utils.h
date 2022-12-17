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

#endif // !__UTILS_H__
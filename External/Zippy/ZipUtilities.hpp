//
// Created by Kenneth Balslev on 08/06/2020.
//

#ifndef ZIPPY_ZIPUTILITIES_HPP
#define ZIPPY_ZIPUTILITIES_HPP

namespace Zippy::Impl
{

    /**
     * @brief Generates a random filename, which is used to generate a temporary archive when modifying and saving
     * archive files.
     * @param length The length of the filename to create.
     * @return Returns the generated filenamen, appended with '.tmp'.
     */
    inline std::string GenerateRandomName(int length) {

        std::string letters = "abcdefghijklmnopqrstuvwxyz0123456789";

        std::random_device                 rand_dev;
        std::mt19937                       generator(rand_dev());
        std::uniform_int_distribution<int> distr(0, letters.size() - 1);

        std::string result;
        for (int    i       = 0; i < length; ++i) {
            result += letters[distr(generator)];
        }

        return result + ".tmp";
    }

}

#endif //ZIPPY_ZIPUTILITIES_HPP

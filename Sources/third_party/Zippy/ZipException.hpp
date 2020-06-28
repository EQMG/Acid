/*
    MIT License

    Copyright (c) 2019 Kenneth Troldal Balslev

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

 */

#ifndef MINIZ_ZIPEXCEPTION_HPP
#define MINIZ_ZIPEXCEPTION_HPP

// ===== Standard Library Includes
#include <stdexcept>

namespace Zippy
{

    /**
     * @brief The ZipRuntimeError class is a custom exception class derived from the std::runtime_error class.
     * @details In case of an error in the Zippy library, an ZipRuntimeError object will be thrown, with a message
     * describing the details of the error.
     */
    class ZipRuntimeError : public std::runtime_error
    {
    public:

        /**
         * @brief Constructor.
         * @param err A string with a description of the error.
         */
        inline explicit ZipRuntimeError(const std::string& err)
                : runtime_error(err) {
        }

        /**
         * @brief Destructor.
         */
        inline ~ZipRuntimeError() override = default;
    };

    /**
     * @brief
     */
    class ZipLogicError : public std::logic_error
    {
    public:
        /**
         * @brief
         * @param err
         */
        inline explicit ZipLogicError(const std::string& err)
                : logic_error(err) {

        }

        /**
         * @brief
         */
        inline ~ZipLogicError() override = default;
    };


} // namespace Zippy


#endif //MINIZ_ZIPEXCEPTION_HPP

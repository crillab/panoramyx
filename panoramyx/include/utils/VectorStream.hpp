/**
* @date 11/10/22
* @file VectorStream.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_VECTORSTREAM_HPP
#define PANORAMYX_VECTORSTREAM_HPP

#include <vector>
#include "Stream.hpp"

namespace Panoramyx {

/**
 * @class VectorStream
 *
 * @brief 
 * @file VectorStream.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */


    template<typename E>
    class VectorStream : public Stream<E> {
    private:
        const std::vector<E> vector;
        size_t index;
    public:
        explicit VectorStream(const std::vector<E> vector) : vector(vector), index(0) {}

        E next() override {
            return vector[index++];
        }

        bool hasNext() override {
            return index < vector.size();
        }
    };

} // Panoramyx

#endif //PANORAMYX_VECTORSTREAM_HPP

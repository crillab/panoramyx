/**
* @date 11/10/22
* @file Stream.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_STREAM_HPP
#define PANORAMYX_STREAM_HPP

#include <functional>
#include <vector>
#include <iostream>

namespace Panoramyx {

/**
 * @class Stream
 *
 * @brief 
 * @file Stream.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    template<typename E>
    class IteratorStream;

    template<typename E>
    class Stream {

    public:

        virtual E next() = 0;

        virtual bool hasNext() = 0;

        IteratorStream<E> begin();

        IteratorStream<E> end();

        void forEach(std::function<void(E)> f);

        virtual ~Stream();

    };

    template<typename E>
    class IteratorStream {
    private:
        Stream<E> &stream;
        bool ended;

    public:
        explicit IteratorStream(Stream<E> &stream, bool ended = false) : stream(stream), ended(ended) {}

        E operator*() {
            return stream.next();
        }

        IteratorStream<E> &operator++() {
            if (!stream.hasNext()) {
                ended = true;
            }
            return *this;
        }

        bool operator!=(IteratorStream<E> &other) {
            return ended != other.ended;
        }
    };

    template<typename E>
    IteratorStream<E> Stream<E>::begin() {
        return IteratorStream<E>(*this);
    }

    template<typename E>
    IteratorStream<E> Stream<E>::end() {
        return IteratorStream<E>(*this, true);
    }

    template<typename E>
    void Stream<E>::forEach(std::function<void(E)> f) {
        for (E e: *this) {
            f(e);
        }
    }

    template<typename E>
    Stream<E>::~Stream()=default;

} // Panoramyx

#endif //PANORAMYX_STREAM_HPP

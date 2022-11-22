/**
* @date 14/11/22
* @file BlockingDeque.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_BLOCKINGDEQUE_HPP
#define PANORAMYX_BLOCKINGDEQUE_HPP

#include <deque>
#include <mutex>
#include <semaphore>
#include "../../../libs/exception/except/except.hpp"

namespace Panoramyx {

/**
 * @class BlockingDeque
 *
 * @brief 
 * @file BlockingDeque.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */
    template<typename E>
    class BlockingDeque {
    private:
        std::deque<E> deque;
        std::mutex mutex;
        std::binary_semaphore semaphore;
    public:
        explicit BlockingDeque(): semaphore(0){};
        void add(E e){
            deque.push_back(e);
            semaphore.release();
        }
        E get(){
            // warning get with multiple threads = ub
            semaphore.acquire();
            mutex.lock();
            if(deque.empty()){
                mutex.unlock();
                throw Except::NoSuchElementException("deque is empty");
            }
            E e = deque.front();
            deque.pop_front();
            mutex.unlock();
            return e;
        }
        void clear(){
            deque.clear();
            semaphore.release();
        }
        ~BlockingDeque()=default;
    };

} // Panoramyx

#endif //PANORAMYX_BLOCKINGDEQUE_HPP

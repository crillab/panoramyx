/**
* @date 21/06/23
* @file ThreadCommunication.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_THREADCOMMUNICATION_HPP
#define PANORAMYX_THREADCOMMUNICATION_HPP

#include <map>
#include <thread>
#include <vector>
#include <functional>
#include "INetworkCommunication.hpp"
#include "../utils/BlockingDeque.hpp"

namespace Panoramyx {

/**
 * @class ThreadCommunication
 *
 * @brief 
 * @file ThreadCommunication.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class ThreadCommunication: public INetworkCommunication {
    private:
        std::map<std::thread::id,int> ids;
        std::vector<std::map<std::pair<int,int>,Panoramyx::BlockingDeque<Message*>*>> queues;
        std::map<int, BlockingDeque<BlockingDeque<Message*>*>*> anyqueues;
        int nbThreads;
        std::vector<std::thread> threads;
        std::mutex newqmutex;
    public:
        explicit ThreadCommunication(int nbThreads);

        ~ThreadCommunication() override = default;

        int getId() override;

        int nbProcesses() override;

        Message *receive(int tag, int src, unsigned long size) override;

        void send(Message *message, int dest) override;

        void finalize() override;

        void start(std::function<void()> runnable) override;
    };

} // Panoramyx

#endif //PANORAMYX_THREADCOMMUNICATION_HPP

/**
* @date 21/06/23
* @file ThreadCommunication.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "../../include/network/ThreadCommunication.hpp"

namespace Panoramyx {

/**
@class ThreadCommunication
@brief Definition of the class ThreadCommunication. 
@file ThreadCommunication.cpp
*/


    ThreadCommunication::ThreadCommunication(int nbThreads) : nbThreads(nbThreads) {}

    int ThreadCommunication::getId() {
        return ids[std::this_thread::get_id()];
    }

    int ThreadCommunication::nbProcesses() {
        return (int)ids.size();
    }

    void ThreadCommunication::start(std::function<void()> runnable) {
        for(int i=0;i<nbThreads;i++){
            threads.emplace_back(runnable);
            ids[threads.back().get_id()]=i;
            queues.emplace_back();
        }
    }

    Message *ThreadCommunication::receive(int tag, int src, unsigned long size) {
        std::pair<int,int> k={tag,src};
        newqmutex.lock();
        if(queues[this->getId()].find(k)==queues[this->getId()].end()){
            queues[this->getId()][k]=new BlockingDeque<Message*>();
        }
        newqmutex.unlock();
        return queues[this->getId()][{tag,src}]->get();
    }

    void ThreadCommunication::send(Message *message, int dest) {
        auto* newmsg = static_cast<Message *>(malloc(message->size));
        memcpy(newmsg,message,message->size);
        message->src=this->getId();
        auto k=std::make_pair(message->tag,message->src);
        newqmutex.lock();
        if(queues[dest].find(k)==queues[dest].end()){
            queues[dest][k]=new BlockingDeque<Message*>();
        }
        newqmutex.unlock();
        queues[dest][{message->tag,message->src}]->add(newmsg);
    }

    void ThreadCommunication::finalize() {
        for(int i=0;i<nbThreads;i++){
            threads[i].join();
        }
    }

} // Panoramyx
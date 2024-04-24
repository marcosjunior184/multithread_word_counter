#ifndef _COMPUTATION_QUEUE_H_
#define _COMPUTATION_QUEUE_H_

#include <condition_variable>
#include <iostream>
#include <string>
#include <vector>
#include <mutex> 
#include <queue> 
#include <map>


namespace Computation
{
    class Computation_Queue
    {
        public:
            struct Comp_Unit{
                std::map<std::string, int> _task;
                bool is_sentinel;
            };


            Computation_Queue();
            void put(Comp_Unit unit);
            Comp_Unit get();

        private:
            std::queue<Comp_Unit> task_queue_;
            std::mutex queue_mutex_;
            std::condition_variable cv_;  

    };
}

#include "Computation_Queue.cpp"

#endif
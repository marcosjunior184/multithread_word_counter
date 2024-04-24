#include "Task_Queue.hpp"
#include "Computation_Queue.hpp"

#include <condition_variable> 
#include <iostream>
#include <fstream>
#include <string>
#include <mutex> 
#include <queue> 
#include <thread>
#include <vector>
#include <map>
#include <regex>


using namespace std;
using namespace Queue;
using namespace Computation;

class Multi_Thread{
    
    public:
    Multi_Thread(string file_path, size_t num_of_threads = 2, size_t lines_per_thread = 10){
        file_path_ = file_path;
        num_of_threads_ = num_of_threads;
        lines_per_thread_ = lines_per_thread;
    };



    void run()
    {
        Task_Queue task;

        Computation_Queue comp;

        producers(&task);



        Task_Queue::Task terminator{
            {},
            true
        };

        task.put(terminator);
        workers(&task, &comp);

        Computation_Queue::Comp_Unit comp_unit; 

        comp_unit = comp.get();

        for(auto const& item : comp_unit._task)
        {
            cout << "Key: " << item.first
                << " value: " << item.second << endl;
        }
    };



    private:

    string file_path_;
    size_t num_of_threads_;
    size_t lines_per_thread_;

    void producers(Task_Queue* w_queue){


        fstream file;
        file.open(file_path_, ios::in);

        if (file.is_open()){

            string line;
            vector<string> task_vec;
            int string_count = 0;

            while (getline(file, line)){

                string_count ++;
                task_vec.push_back(line);
                
                if (task_vec.size() == lines_per_thread_)
                {
                    Task_Queue::Task task_ {
                        move(task_vec),
                        false
                    };
                    w_queue->put(task_);
                    task_vec.clear();
                }

            }

            if (task_vec.size() > 0)
            {
                Task_Queue::Task task_ {
                    move(task_vec),
                    false
                };
                w_queue->put(task_);
                task_vec.clear();

            }
            file.close();
        }
    };

    void workers(Task_Queue* w_queue, Computation_Queue* comp_queue){

    
        while (true) {

            Task_Queue::Task task = w_queue->get();

            if (task._task.empty() || task.is_sentinel){
                return;
            }

            map<string, int> counter;

            for (auto const& value : task._task)
            {
                regex rgx("[\\b\\W\\b]+");

                sregex_token_iterator iter(value.begin(), value.end(), rgx, -1);
                sregex_token_iterator end;

                while (iter != end){

                    counter[*iter] ++;

                    iter ++;
                }

            }

            
            Computation_Queue::Comp_Unit unit{
                move(counter),
                false
            };

            comp_queue->put(unit);

        }
    }
};


int main(){

    Multi_Thread multi_thrd("Input/Control_test_file.txt", 2, 10);
    multi_thrd.run();
}
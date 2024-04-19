#include "Task_Queue.hpp"

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

class Multi_Thread{
    
    public:
    Multi_Thread(string file_path, size_t num_of_threads = 2, size_t lines_per_thread = 10){
        file_path_ = file_path;
        num_of_threads_ = num_of_threads;
        lines_per_thread_ = lines_per_thread;
    };



    private:

    string file_path_;
    size_t num_of_threads_;
    size_t lines_per_thread_;

    void producers(Task_Queue w_queue){

        fstream file;
        file.open(file_path_, ios::in);

        if (file.is_open()){

            string line;
            vector<string> task_vec;

            while (getline(file, line)){


                task_vec.push_back(line);

                if (task_vec.size() == lines_per_thread_)
                {
                    Task_Queue::Task task_ {
                        task_vec,
                        false
                    };
                    
                    w_queue.put(task_);
                    task_vec.clear();
                }

            }
            file.close();
        }
    };

    void workers(Task_Queue w_queue){

        map<string, int> counter;

        while (true) {


            Task_Queue::Task task = w_queue.get();

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
                    std::cout << *iter << '\n';

                    counter[*iter] ++;

                    iter ++;
                }

            }


            for (auto const& entry : counter){

                cout << "Key " << entry.first
                    <<", Value " << entry.second << endl;
            }

        }
    }
};


int main(){



    string a_str = "In the heart of the forest,";

    vector<string> vec;

    vec.push_back("a");
    vec.push_back("simple");
    vec.push_back("test");

    Task_Queue::Task task {
        vec,
        true
    };


    map<string, int> cnt;


    map<string, int> cnt1;

    cnt["Hi"] ++;
    cnt1["There"] ++;


    for (auto const& entry : cnt){

        cout << "Key " << entry.first
            <<", Value " << entry.second << endl;
    }
}
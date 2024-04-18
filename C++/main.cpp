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

class Work_Queue{


    public:

    Work_Queue(){
        task_queue_ = {};
    }

    void put(vector<std::string> task){
        {
        unique_lock<mutex> lock(queue_mutex_);
        task_queue_.emplace(move(task));
        }
        cv_.notify_one();
    };


    vector<string> get(){
        vector<string> task;

        {
            unique_lock<mutex> lock(queue_mutex_);

            cv_.wait(lock, [this]{
                return !task_queue_.empty();
            });

            task = move(task_queue_.front());
            task_queue_.pop();
        }

        cv_.notify_one();

        return task;

    };

    bool empty(){
        return task_queue_.empty();
    }

    private:

    queue<vector<std::string>> task_queue_;
    mutex queue_mutex_;
    condition_variable cv_;
};


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

    void producers(Work_Queue w_queue){

        fstream file;
        file.open(file_path_, ios::in);

        if (file.is_open()){

            string line;
            vector<string> task;

            while (getline(file, line)){


                task.push_back(line);

                if (task.size() == lines_per_thread_){
                    w_queue.put(task);
                    task.clear();
                }

            }
            file.close();
        }
    };

    void workers(Work_Queue w_queue){

        map<string, int> counter;

        while (true) {


            vector<string> task = w_queue.get();

            if (task.empty()){
                return;
            }

            string a_str = "Test string, replace with task";

            regex rgx("[\\b\\W\\b]+");

            sregex_token_iterator iter(a_str.begin(), a_str.end(), rgx, -1);
            sregex_token_iterator end;

            map<string, int> counter;

            while (iter != end){
                std::cout << *iter << '\n';

                counter[*iter] ++;

                iter ++;
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


    for(auto const& entry : vec){
        cout << entry;
    }
    
    
    // fstream file;

    // Work_Queue w_queue;
    // vector<string> test;
    

    // file.open("Input/test.txt", ios::in);

    // if (file.is_open()){

    //     string line;

    //     while (getline(file, line)){

    //         // cout << line << "\n";
    //         test.push_back(line);

    //         if (test.size() == 2 ){
    //              w_queue.put(test);
    //              test.clear();
    //         }

    //     }
    //     file.close();
    // }

    // w_queue.put(test);
    // vector<string> tt;
    // // tt.push_back(tt);


    // w_queue.put(tt);

    // int counter = 0;

    // while (!w_queue.empty()){
    //     vector<string> internal = w_queue.get();

    //     for (string& str : internal){
    //         cout << counter << ' ' << str << '\n';
    //     }

    //     if (internal.empty()){
    //         cout << "Empty";
    //     }

    //     counter ++;
    // }

}
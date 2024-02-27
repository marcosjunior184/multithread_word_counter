import re
import sys
import time
import pandas as pd
from collections import Counter
from queue import Queue
from threading import Thread
from threading import Lock


class Multi_Thread:
    """
    A class used to count the number of words on a file using multithreading.

    Attributes
    ----------
    _file_path : str
        The file path of a file to be analyzed.
    _num_threads : int
        Number of worker threads to be created.
    _lines_per_thread : int
        Number of lines that a task will have. That is, the number of lines a thread will analyze per task.

    Methods
    -------
    producer(queue, step):
        Crates tasks and add them to the queue.
        A task is an array with step number of lines.

    worker(queue, safe_report):
        A thread target that consumes tasks from the queue and saves the final computation into safe_report.

    generate_report(report_object):
        Print a final report.

    run()
        Run the multithread word counter on a file and print a final report.
    """

    class Thread_Safe_Report(object):
        """
        A report object shared by worker threads. 
        When a worker thread finishes a task they add their result into the Thread_Safe_Report object.


        Attributes
        ----------
        _word_counter : Counter()
            A counter object for storing the word count.
        _lock : Lock()
            A lock to ensure thread safety when workers are accessing the shared Thread_Safe_Report object.

        Methods
        ----------
        add(counter_list):
            Receive a Counter object from a worker thread and sum it with the internal counter.

        get_results():
            Return a list of words and their count sorted by their count descending.
        """

        def __init__(self):
            """
            Construct all necessary attributes for a Thread_Safe_Report.
            """
            self._word_counter = Counter()
            self._lock = Lock()
        
        def add(self, counter_list):
            """
            Receive a Counter object from a worker thread and sum it with the internal counter object.

            parameters
            ----------
            counter_list : Counter()
                A counter object with words and their count.

            Returns
            -------
                None
            """
            with self._lock:
                self._word_counter += counter_list

        def get_result(self):
            """
            Return a list of words and their count sorted by their count descending. 

            Returns
            -------
                None
            """
            with self._lock:
                    return self._word_counter.most_common()



    def __init__(self, file_path, num_threads = 2, lines_per_thread=10):
        """
        Construct all necessary attributes for Multi_Thread

        Parameters
        ----------
        file_path : str
            The file path to be analyzed 
        num_threads : int, optional
            Number of worker threads to be created (default is 2)
        lines_per_thread : int, optional
            Number of lines that a task will have. That is, the number of lines a thread will analyze per task (default = 10)
        """
        self._file_path = file_path
        self._num_threads = num_threads
        self._lines_per_thread = lines_per_thread


    def producer(self, queue, step):
        """
        The target for a producer thread. It is responsible for adding tasks to a queue so workers can consume it.

        Producers open a file, read a specified number of lines (step), add those lines into an array, and add the array(a task) into the queue.
        Repeat until the entire file has been read and converted into tasks.

        if step 2 then
            task = ['First line of a file as an example.', 'Second line of a file as an example.']
        
        Attributes
        ----------
        queue : queue
            A queue of tasks.
        step : int
            Number of lines that a task will have.

        Returns
        -------
            None
        """
        try:

            if not self._file_path.endswith('.txt'):
                raise Exception("ERROR: Wrong file type, file {0} is not accepted.\nThe input file has to be a text file (.txt).".format(self._file_path))

            file = open(self._file_path, 'r')

            content = [ line.strip() for line in file.readlines() if line != ' ' ]  # strip leading and trailing white space
            content = [line for line in content if line]                            # remove empty strings. 


            for index in range(0, len(content), step):
                queue.put(content[index : index + step])    # Insert an array with step number of lines (step is the length of the array).

            file.close()

        except OSError as e:
            print("ERROR I/O: Could not read file {0}. \nerror({1}): {2}\n\n".format(self._file_path, e.errno, e.strerror))
            sys.exit(1)
        except Exception as e:
            print(e, '\n\n')
            sys.exit(1)


    def worker(self, queue, safe_report):
        """
        The target for a worker thread. It is responsible to fetch and consume a task from the queue.

        Workers count the number of words in a collection of strings. It does so by getting a task from a queue, that is, a list of strings.
        Once the worker receives the signal to terminate, it inserts their result (Counter object with words and their count) into the shared object Thread_Safe_Report.

        Attributes
        ----------
        queue : queue
            A queue with tasks to be consumed.
        safe_report : Thread_Safe_Report()
            An object to hold the word count.

        Returns
        -------
            None
        """

        internal_counter = Counter()    # start a Counter so it can compute and add to safe_report only once before it terminates.
        while True:

            data = queue.get()
            if data == None:            # terminate signal.
                safe_report.add(internal_counter)
                queue.task_done()
                break

            for line in data:
                words_arr = [ word for word in re.split(r'[\b\W\b]+', line) if word != ""]   # Split on characters with regex and remove the empty string it generates from array.
                internal_counter += Counter(words_arr)


            queue.task_done()


    def generate_report(self, report_object):
        """
        Generate and print a report.

        Attributes
        ----------
        report_object : Thread_Safe_Report()
            The object holding the words count.

        Returns
        -------
            None
        """
        words, words_count = zip(*report_object.get_result())

        data_frame = pd.DataFrame({
                                    'Words': list(words),           
                                    'Words Count': list(words_count)
                                })

        data_frame['words length'] = data_frame['Words'].str.len()             # Add word leght column and get word length
        

        word_count_group = data_frame.groupby('Words Count')['Words']\
                           .apply(', '.join)\
                           .reset_index()\
                           .sort_values(by=['Words Count'], ascending=False)    # Group based on "Words Count" column joining the "Words" with the same count by comma and sort it.


        word_length_group = data_frame.groupby("words length")['Words']\
                            .apply(', '.join)\
                            .reset_index()\
                            .sort_values(by=['words length'], ascending=False)  # Group based on "words length" column joining the "Words" with the same length by comma and sort it.

        word_length_group["Same length count"] =  word_length_group['Words'].str.split(",").str.len()   # Add "Same length count" column and count words with the same length

        pd.set_option('display.max_rows', None)
        pd.set_option('display.max_columns', None)
        pd.set_option('display.width', None)
        pd.set_option('display.colheader_justify', 'center')
        pd.set_option('display.precision', 2)

        print('----------------------------Final report--------------------------------------')
        print('\nTotal Words counted: ', data_frame['Words Count'].sum())
        print('Heighest count: "', words[0], '" counted ',words_count[0], ' times', sep='')                   # words and words_count are sorted with highest count first.
        print('\n\n------------------------Report grouped by words count------------------------')
        print(word_count_group)
        print('\n\n------------------------Report grouped by words with the same length------------------------')
        print(word_length_group)
            

    def run(self):
        """
        Run a multithread word count on the specified document.

        Returns
        -------
            None
        """

        report_object = self.Thread_Safe_Report()                # Thread_Safe_Report to be shared with workers
        workers_dict = dict()                                    # Dictionary to keep track of workers and terminate them 
        work_queue = Queue()                                     # Queue to be populated with task by producers, and consumed by workers


        producer_thread = Thread(target= self.producer, args = (work_queue, self._lines_per_thread))    # Producer Thread
        producer_thread.start() 



        for thread_id in range(self._num_threads):                                                       # Create worker threads
            worker_thread = Thread(target = self.worker, args = (work_queue, report_object))
            worker_thread.daemon = True
            workers_dict[thread_id] = worker_thread
            workers_dict[thread_id].start()


        producer_thread.join()


        for thread_id in range(self._num_threads):  # Send signal to terminate worker.
            work_queue.put(None)

        work_queue.join()

        for thread_id in range(self._num_threads):  # Wait for threads to finish thei execution
            workers_dict[thread_id].join()

        self.generate_report(report_object)

       
def main():
    start_time = time.time()

    Control_test_file = 'input/Control_test_file.txt'
    Romeo_and_Juliet = 'input/Romeo_and_Juliet.txt'
    Alices_adventure_in_wonderland = 'input/Alice\'s_Adventure_in_wonderland.txt'
    Thus_spake_zarathustra = 'input/Thus_spake_zarathustra.txt'

    multi_thread = Multi_Thread(file_path = Control_test_file,                      # Has to be a text file (.txt).
                                num_threads = 20,
                                lines_per_thread = 500)
    multi_thread.run()

    print('\nExecution time: ', time.time() - start_time)


if __name__ == '__main__':
    main()
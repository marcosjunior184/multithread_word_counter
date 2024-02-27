Author: Marcos da Silva Junior

# Word Csounter.
Word counter is a Python script that reads a file from the "input/" directory, counts the words and returns a report. It accomplishes it by using multithread.
The report contains:

The report contains:
 - Total words counted.
 - The word with the highest count.
 - A table with 'words count' and 'words', where words with the same 'words count' are grouped.
 - A table with 'words legth', 'Words', and 'Same length count', where words with the same length are grouped.
 - Execution time.

## Report Example.
```python
----------------------------Final report--------------------------------------

Total Words counted:  20
Heighest count: "the" counted 2 times


------------------------Report grouped by words count------------------------
   Words Count                       Words
1       2                                                     the
0       1       In, heart, of, forest, where, ancient, trees, ...


------------------------Report grouped by words with the same length------------------------
   words length                Words                 Same length count
7       12                             moss-covered          1        
6        8                                 tranquil          1        
5        7       ancient, whisper, secrets, through          4        
4        6                   forest, stream, stones          3        
3        5               heart, where, trees, winds          4        
2        3                            the, its, way          3        
1        2                                   In, of          2        
0        1                                        a          1        

Execution time:  0.008411169052124023
```

# Multithreading.
The multithreading consists of a producer thread and x number of workers threads.
- Producer threads: Create tasks and insert them into a list.
- Worker threads: Get tasks from the list and work on them.

# Dependencies.

Python version: 3.10.12

To install Python3 please refer to the documentation:
 - [WSL](https://learn.microsoft.com/en-us/windows/python/web-frameworks)
 - [Windows](https://www.python.org/downloads/)
 - [Linux](https://docs.python-guide.org/starting/install3/linux/)

Pandas version: 2.2.1

```bash
pip install wheel
```

```bash
pip install pandas
```

or

```bash
pip install -r requirements.txt
```

If using virtual environment

```bash
python3 -m venv .venv

# Activate virtual environment. Then run pip on requirements.txt.
source .venv/bin/activate
```
### Updating dependencies.

To update (or generate) the requirements.txt please run.

```bash
pip freeze > requirements.txt
```
# Usage.

At this time, this script does not support stdin from the command line. Modifications on which file to read, number of threads working and number of lines per task have to be manually changed on main()


```python

Romeo_and_Juliet = 'input/Romeo_and_Juliet.txt'
Alices_adventure_in_wonderland = 'input/Alice\'s_Adventure_in_wonderland.txt'
Thus_spake_zarathustra = 'input/Thus_spake_zarathustra.txt'

multi_thread = Multi_Thread(file_path = Thus_spake_zarathustra,  <== Change file path here 
                            num_threads = 20,                    <== Change num of threads here
                            lines_per_thread= 500)               <== Change num of lines per task here
```

### Terminal Usage.

```python
python3 main.py
```
or to get real, user and sys time

```python
time python3 main.py
```
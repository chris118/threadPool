//
//  main.cpp
//  Demo
//
//  Created by admin on 2017/3/21.
//  Copyright © 2017年 admin. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <vector>
#include <pthread.h>
#include <thread>

#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)

#include "ThreadPool.h"

using namespace std;
using namespace hh;


class DetectionTask;
ThreadPool threadPool(2);
int task_id = 0;

class DetectionTask: public Task
{
public:
    DetectionTask(){
    }
    
    virtual int run()
    {
//         cout<<"thread name:"<<std::this_thread::get_id()<< "  task args: " <<  (char*)this->arg_ << endl;
        cout << taskName_ << "   Running !!!" << endl;
        
        sleep(4);
         cout << taskName_ << "  finished !!!" << endl;
       
       
        return 0;
    }
    
    ~DetectionTask(){
        cout << taskName_ <<" destroy ~~~" << endl;
    }
};

void sigalrm_fn(int sig)
{
    alarm(1);
    
//    cout << "threadPool count: " << threadPool.size() << endl;
    if(task_id >= 1000){
        task_id = 0;
    }
    else{
        task_id++;
    }
    
    if(threadPool.size() >= 5)
    {
        threadPool.clearTask();
    }
    
    DetectionTask *task = new DetectionTask();
    task->setName(std::to_string(task_id));
    task->setArg((void*)std::to_string(task_id).c_str());
    

    threadPool.addTask(task);
    return;
}


int main(int argc, const char * argv[]) {

    signal(SIGALRM, sigalrm_fn);
    alarm(1);
    
    getchar();
    return 0;
}

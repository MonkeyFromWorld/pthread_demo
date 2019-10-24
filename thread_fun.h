//
// Created by cxj on 2019/10/24.
//

#ifndef PTHREAD_DEMO_THREAD_FUN_H
#define PTHREAD_DEMO_THREAD_FUN_H

#include <thread>
#include <pthread.h>
#include <iostream>
#include <windows.h>
#include <unistd.h>
#include <strstream>
using namespace std;
#define NUM_THREADS 10
#define NAME_LINE   40

void* sayHello();
void *PrintHello(void *threadid);
void *wait(void *t);
DWORD WINAPI ThreadProcSayHello(LPVOID lpParameter);
DWORD WINAPI ThreadProc(LPVOID lpParameter);
DWORD WINAPI ThreadProc2(LPVOID lpParameter);
DWORD WINAPI ThreadProc3(LPVOID lpParameter);
DWORD WINAPI SaleTicket(LPVOID lpParameter);

//定义线程函数传入参数的结构体
typedef struct __THREAD_DATA
{
    int nMaxNum;
    char strThreadName[NAME_LINE];

    __THREAD_DATA() : nMaxNum(0)
    {
        memset(strThreadName, 0, NAME_LINE * sizeof(char));
    }
}THREAD_DATA;

//定义线程函数传入参数的结构体
typedef struct __TICKET
{
    int nCount;
    char strTicketName[NAME_LINE];

    __TICKET() : nCount(0)
    {
        memset(strTicketName, 0, NAME_LINE * sizeof(char));
    }
}TICKET;

typedef struct __THD_DATA
{
    TICKET* pTicket;
    char strThreadName[NAME_LINE];

    __THD_DATA() : pTicket(NULL)
    {
        memset(strThreadName, 0, NAME_LINE * sizeof(char));
    }
}THD_DATA;

//基本类型数据转换成字符串
template<class T>
string convertToString(const T val)
{
    string s;
    strstream ss;
    ss << val;
    ss >> s;
    return s;
}


#endif //PTHREAD_DEMO_THREAD_FUN_H

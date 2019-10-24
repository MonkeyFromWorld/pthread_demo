//
// Created by cxj on 2019/10/24.
//

#include "thread_fun.h"
HANDLE g_hMutex = nullptr;//互斥量

// 线程的运行函数
void* say_hello(void* args)
{
    cout << "Hello Runoob！" << endl;
    return 0;
}

void* sayHello1() {
    WaitForSingleObject(g_hMutex, INFINITE);
    for(int i=0; i<NUM_THREADS; i++) {
        cout << "Hello Robot!" << endl;
        //Sleep(10);
    }
    ReleaseMutex(g_hMutex);//释放互斥量锁
    return nullptr;
}

void* sayHello2() {

    for(int i=0; i<NUM_THREADS; i++) {
        WaitForSingleObject(g_hMutex, INFINITE);
        cout << "Hello Robot!,2," << endl;
        //Sleep(10);
        ReleaseMutex(g_hMutex);//释放互斥量锁
    }
    return nullptr;
}

void *PrintHello(void *threadid) {
    // 对传入的参数进行强制类型转换，由无类型指针变为整形数指针，然后再读取
    int tid = *((int*)threadid);
    cout << "Hello Runoob! 线程 ID: " << tid << "\n";
    pthread_exit(nullptr);
    return nullptr;
}

void *wait(void *t){
    long tid = *(long*)t;

    sleep(1);
    cout << "Sleeping in thread " << endl;
    cout << "Thread with id : " << tid << "  ...exiting " << endl;
    pthread_exit(nullptr);
    return nullptr;
}

DWORD WINAPI ThreadProcSayHello(LPVOID lpParameter){
    for (int i = 0; i < 5; ++ i)
    {
        cout << "Hello,i = " << i << endl;
        Sleep(100);
    }
    return 0L;
}


DWORD WINAPI ThreadProc(LPVOID lpParameter){
    for (int i = 0; i < 5; ++ i)
    {
        cout << "子线程:i = " << i << endl;
        Sleep(100);
    }
    return 0L;
}

//线程函数
DWORD WINAPI ThreadProc2(LPVOID lpParameter)
{
    auto* pThreadData = (THREAD_DATA*)lpParameter;

    for (int i = 0; i < pThreadData->nMaxNum; ++ i)
    {
        cout << pThreadData->strThreadName << " --- " << i << endl;
        //Sleep(50);
    }
    return 0L;
}

//线程函数
DWORD WINAPI ThreadProc3(LPVOID lpParameter)
{
    THREAD_DATA* pThreadData = (THREAD_DATA*)lpParameter;

    for (int i = 0; i < pThreadData->nMaxNum; ++ i)
    {
        //请求获得一个互斥量锁
        WaitForSingleObject(g_hMutex, INFINITE);
        cout << pThreadData->strThreadName << " --- " << i << endl;
        Sleep(50);
        //释放互斥量锁
        ReleaseMutex(g_hMutex);
    }
    return 0L;
}

//售票程序
DWORD WINAPI SaleTicket(LPVOID lpParameter) {

    THD_DATA *pThreadData = (THD_DATA *) lpParameter;
    TICKET *pSaleData = pThreadData->pTicket;
    while (pSaleData->nCount > 0) {
        //请求获得一个互斥量锁
        WaitForSingleObject(g_hMutex, INFINITE);
        if (pSaleData->nCount > 0) {
            cout << pThreadData->strThreadName << "出售第" << pSaleData->nCount-- << "的票,";
            if (pSaleData->nCount >= 0) {
                cout << "出票成功!剩余" << pSaleData->nCount << "张票." << endl;
            } else {
                cout << "出票失败！该票已售完。" << endl;
            }
        }
        //Sleep(1);
        //释放互斥量锁
        ReleaseMutex(g_hMutex);
    }
    return 0L;
}
#include <iostream>
#include <pthread.h>
#include <windows.h>
#include <unistd.h>
#include <thread>
#include <backward/strstream>

using namespace std;
#define NUM_THREADS 10
#define NAME_LINE   40
#define EASY 0
#define MID 0
#define JOIN_DETACH 0
#define DIFF 1

void* sayHello() {
    cout << "Hello Robot!" << endl;
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
}

DWORD WINAPI ThreadProc(LPVOID lpParameter){
    for (int i = 0; i < 5; ++ i)
    {
        cout << "子线程:i = " << i << endl;
        Sleep(100);
    }
    return 0L;
}

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

HANDLE g_hMutex = NULL;     //互斥量
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

int main() {
#if EASY
    // 定义线程的 id 变量，多个变量使用数组
    pthread_t tids[NUM_THREADS];
    for(int i=0; i<NUM_THREADS; i++) {
        //参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
        int ret = pthread_create(&tids[i], nullptr, reinterpret_cast<void *(*)(void *)>(sayHello), nullptr);
        if(ret != 0)
            cout << "pthread_create error: error_code=" << ret << endl;
    }
    pthread_exit(nullptr);
#endif
#if MID
    pthread_t threads[NUM_THREADS];
    int index[NUM_THREADS];
    int rc, i;
    for(i=0; i<NUM_THREADS; i++) {
        cout << "main(),创建线程: " << i << "\n";
        index[i] = i;
        // 传入的时候必须强制转换为void* 类型，即无类型指针
        rc = pthread_create(&threads[i], nullptr, reinterpret_cast<void *(*)(void *)>(PrintHello), (void *)&(index[i]));
        if(rc) {
            cout << "Error:无法创建线程," << rc << endl;
            exit(-1);
        }
    }
    pthread_exit(nullptr);
#endif
#if JOIN_DETACH
    int rc;
    int i;
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    void *status;

    // 初始化并设置线程为可连接的（joinable）
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for( i=0; i < NUM_THREADS; i++ ){
        cout << "main() : creating thread, " << i << endl;
        rc = pthread_create(&threads[i], NULL, wait, (void *)&i );
        if (rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }
    // 删除属性，并等待其他线程
    pthread_attr_destroy(&attr);
    for( i=0; i < NUM_THREADS; i++ ){
        rc = pthread_join(threads[i], &status);
        if (rc){
            cout << "Error:unable to join," << rc << endl;
            exit(-1);
        }
        cout << "Main: completed thread id :" << i ;
        cout << "  exiting with status :" << status << endl;
    }

    cout << "Main: program exiting." << endl;
    pthread_exit(NULL);
#endif
#if DIFF
    /*
     * 【Demo1】：创建一个最简单的线程
    //创建一个线程
    HANDLE thread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
    //关闭线程
    CloseHandle(thread);
    //主线程的执行路径
    for (int i = 0; i < 5; ++ i)
    {
        cout << "主线程:i = " << i << endl;
        Sleep(500);
    }
     */
    /*
     * 【Demo2】：在线程函数中传入参数
    //初始化线程数据
    THREAD_DATA threadData1, threadData2;
    threadData1.nMaxNum = 5;
    strcpy(threadData1.strThreadName, "线程1");
    threadData2.nMaxNum = 10;
    strcpy(threadData2.strThreadName, "线程2");
    //创建第一个子线程
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadProc2, &threadData1, 0, NULL);
    //创建第二个子线程
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadProc2, &threadData2, 0, NULL);
    //关闭线程
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    //主线程的执行路径
    for (int i = 0; i < 5; ++ i)
    {
        cout << "主线程 === " << i << endl;
        Sleep(50);
    }
    system("pause");
     */
    /*【Demo3】：线程同步
    //创建一个互斥量
    g_hMutex = CreateMutex(NULL, FALSE, NULL);
    //初始化线程数据
    THREAD_DATA threadData1, threadData2;
    threadData1.nMaxNum = 5;
    strcpy(threadData1.strThreadName, "线程1");
    threadData2.nMaxNum = 10;
    strcpy(threadData2.strThreadName, "线程2");
    //创建第一个子线程
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadProc3, &threadData1, 0, NULL);
    //创建第二个子线程
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadProc3, &threadData2, 0, NULL);
    //关闭线程
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    //主线程的执行路径
    for (int i = 0; i < 5; ++ i)
    {
        //请求获得一个互斥量锁
        WaitForSingleObject(g_hMutex, INFINITE);
        cout << "主线程 === " << i << endl;
        Sleep(50);
        //释放互斥量锁
        ReleaseMutex(g_hMutex);
    }
    system("pause");
     */
    //【Demo4】：模拟火车售票系统
    //创建一个互斥量
    g_hMutex = CreateMutex(NULL, FALSE, NULL);

    //初始化火车票
    TICKET ticket;
    ticket.nCount = 100;
    strcpy(ticket.strTicketName, "北京-->赣州");

    const int THREAD_NUMM = 8;
    THD_DATA threadSale[THREAD_NUMM];
    HANDLE hThread[THREAD_NUMM];
    for(int i = 0; i < THREAD_NUMM; ++ i)
    {
        threadSale[i].pTicket = &ticket;
        string strThreadName = convertToString(i);

        strThreadName = "窗口" + strThreadName;

        strcpy(threadSale[i].strThreadName, strThreadName.c_str());

        //创建线程
        hThread[i] = CreateThread(NULL, NULL, SaleTicket, &threadSale[i], 0, NULL);

        //请求获得一个互斥量锁
        WaitForSingleObject(g_hMutex, INFINITE);
        cout << threadSale[i].strThreadName << "开始出售 " << threadSale[i].pTicket->strTicketName << " 的票..." << endl;
        //释放互斥量锁
        ReleaseMutex(g_hMutex);

        //关闭线程
        CloseHandle(hThread[i]);
    }

    system("pause");
#endif
    return 0;
}
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
    // �Դ���Ĳ�������ǿ������ת������������ָ���Ϊ������ָ�룬Ȼ���ٶ�ȡ
    int tid = *((int*)threadid);
    cout << "Hello Runoob! �߳� ID: " << tid << "\n";
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
        cout << "���߳�:i = " << i << endl;
        Sleep(100);
    }
    return 0L;
}

//�����̺߳�����������Ľṹ��
typedef struct __THREAD_DATA
{
    int nMaxNum;
    char strThreadName[NAME_LINE];

    __THREAD_DATA() : nMaxNum(0)
    {
        memset(strThreadName, 0, NAME_LINE * sizeof(char));
    }
}THREAD_DATA;

//�̺߳���
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

HANDLE g_hMutex = NULL;     //������
//�̺߳���
DWORD WINAPI ThreadProc3(LPVOID lpParameter)
{
    THREAD_DATA* pThreadData = (THREAD_DATA*)lpParameter;

    for (int i = 0; i < pThreadData->nMaxNum; ++ i)
    {
        //������һ����������
        WaitForSingleObject(g_hMutex, INFINITE);
        cout << pThreadData->strThreadName << " --- " << i << endl;
        Sleep(50);
        //�ͷŻ�������
        ReleaseMutex(g_hMutex);
    }
    return 0L;
}

//�����̺߳�����������Ľṹ��
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

//������������ת�����ַ���
template<class T>
string convertToString(const T val)
{
    string s;
    strstream ss;
    ss << val;
    ss >> s;
    return s;
}

//��Ʊ����
DWORD WINAPI SaleTicket(LPVOID lpParameter) {

    THD_DATA *pThreadData = (THD_DATA *) lpParameter;
    TICKET *pSaleData = pThreadData->pTicket;
    while (pSaleData->nCount > 0) {
        //������һ����������
        WaitForSingleObject(g_hMutex, INFINITE);
        if (pSaleData->nCount > 0) {
            cout << pThreadData->strThreadName << "���۵�" << pSaleData->nCount-- << "��Ʊ,";
            if (pSaleData->nCount >= 0) {
                cout << "��Ʊ�ɹ�!ʣ��" << pSaleData->nCount << "��Ʊ." << endl;
            } else {
                cout << "��Ʊʧ�ܣ���Ʊ�����ꡣ" << endl;
            }
        }
        //Sleep(1);
        //�ͷŻ�������
        ReleaseMutex(g_hMutex);
    }

    return 0L;
}

int main() {
#if EASY
    // �����̵߳� id �������������ʹ������
    pthread_t tids[NUM_THREADS];
    for(int i=0; i<NUM_THREADS; i++) {
        //���������ǣ��������߳�id���̲߳��������õĺ���������ĺ�������
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
        cout << "main(),�����߳�: " << i << "\n";
        index[i] = i;
        // �����ʱ�����ǿ��ת��Ϊvoid* ���ͣ���������ָ��
        rc = pthread_create(&threads[i], nullptr, reinterpret_cast<void *(*)(void *)>(PrintHello), (void *)&(index[i]));
        if(rc) {
            cout << "Error:�޷������߳�," << rc << endl;
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

    // ��ʼ���������߳�Ϊ�����ӵģ�joinable��
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
    // ɾ�����ԣ����ȴ������߳�
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
     * ��Demo1��������һ����򵥵��߳�
    //����һ���߳�
    HANDLE thread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
    //�ر��߳�
    CloseHandle(thread);
    //���̵߳�ִ��·��
    for (int i = 0; i < 5; ++ i)
    {
        cout << "���߳�:i = " << i << endl;
        Sleep(500);
    }
     */
    /*
     * ��Demo2�������̺߳����д������
    //��ʼ���߳�����
    THREAD_DATA threadData1, threadData2;
    threadData1.nMaxNum = 5;
    strcpy(threadData1.strThreadName, "�߳�1");
    threadData2.nMaxNum = 10;
    strcpy(threadData2.strThreadName, "�߳�2");
    //������һ�����߳�
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadProc2, &threadData1, 0, NULL);
    //�����ڶ������߳�
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadProc2, &threadData2, 0, NULL);
    //�ر��߳�
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    //���̵߳�ִ��·��
    for (int i = 0; i < 5; ++ i)
    {
        cout << "���߳� === " << i << endl;
        Sleep(50);
    }
    system("pause");
     */
    /*��Demo3�����߳�ͬ��
    //����һ��������
    g_hMutex = CreateMutex(NULL, FALSE, NULL);
    //��ʼ���߳�����
    THREAD_DATA threadData1, threadData2;
    threadData1.nMaxNum = 5;
    strcpy(threadData1.strThreadName, "�߳�1");
    threadData2.nMaxNum = 10;
    strcpy(threadData2.strThreadName, "�߳�2");
    //������һ�����߳�
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadProc3, &threadData1, 0, NULL);
    //�����ڶ������߳�
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadProc3, &threadData2, 0, NULL);
    //�ر��߳�
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    //���̵߳�ִ��·��
    for (int i = 0; i < 5; ++ i)
    {
        //������һ����������
        WaitForSingleObject(g_hMutex, INFINITE);
        cout << "���߳� === " << i << endl;
        Sleep(50);
        //�ͷŻ�������
        ReleaseMutex(g_hMutex);
    }
    system("pause");
     */
    //��Demo4����ģ�����Ʊϵͳ
    //����һ��������
    g_hMutex = CreateMutex(NULL, FALSE, NULL);

    //��ʼ����Ʊ
    TICKET ticket;
    ticket.nCount = 100;
    strcpy(ticket.strTicketName, "����-->����");

    const int THREAD_NUMM = 8;
    THD_DATA threadSale[THREAD_NUMM];
    HANDLE hThread[THREAD_NUMM];
    for(int i = 0; i < THREAD_NUMM; ++ i)
    {
        threadSale[i].pTicket = &ticket;
        string strThreadName = convertToString(i);

        strThreadName = "����" + strThreadName;

        strcpy(threadSale[i].strThreadName, strThreadName.c_str());

        //�����߳�
        hThread[i] = CreateThread(NULL, NULL, SaleTicket, &threadSale[i], 0, NULL);

        //������һ����������
        WaitForSingleObject(g_hMutex, INFINITE);
        cout << threadSale[i].strThreadName << "��ʼ���� " << threadSale[i].pTicket->strTicketName << " ��Ʊ..." << endl;
        //�ͷŻ�������
        ReleaseMutex(g_hMutex);

        //�ر��߳�
        CloseHandle(hThread[i]);
    }

    system("pause");
#endif
    return 0;
}
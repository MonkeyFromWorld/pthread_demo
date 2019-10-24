#include "thread_fun.h"

#define EASY 0
#define MID 0
#define MID2 1
#define JOIN_DETACH 0
#define DIFF 0
extern HANDLE g_hMutex;//������

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
#ifdef MID2
    // �����̵߳� id �������������ʹ������
    /*
    pthread_t tids[NUM_THREADS];
    for(int i=0; i<NUM_THREADS; i++) {
        //���������ǣ��������߳�id���̲߳��������õĺ���������ĺ�������
        int ret = pthread_create(&tids[i], nullptr, reinterpret_cast<void *(*)(void *)>(sayHello), nullptr);
        if(ret != 0)
            cout << "pthread_create error: error_code=" << ret << endl;
    }
     */
    g_hMutex = CreateMutex(NULL, FALSE, NULL);
    //������һ�����߳�
    HANDLE hThread1 = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(sayHello1), NULL, 0, NULL);
    CloseHandle(hThread1);
    //�����ڶ������߳�
    HANDLE hThread2 = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(sayHello2), NULL, 0, NULL);
    //�ر��߳�
    CloseHandle(hThread2);
    for (int i = 0; i < NUM_THREADS; ++ i)
    {
        //������һ����������
        WaitForSingleObject(g_hMutex, INFINITE);
        cout << "���߳� === " << i << endl;
        //Sleep(50);
        //�ͷŻ�������
        ReleaseMutex(g_hMutex);
    }
    system("pause");
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
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadProc2, &threadData2, 0, NULL);
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
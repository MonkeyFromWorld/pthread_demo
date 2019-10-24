#include "thread_fun.h"

#define EASY 0
#define MID 0
#define MID2 0
#define JOIN_DETACH 0
#define DIFF 0
#define ANOTHER_FUN 1

extern HANDLE g_hMutex;//������
extern pthread_mutex_t mutex;
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
        cout << "main(),�����߳�: " << i << endl;
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
#if MID2
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
#if ANOTHER_FUN
    pthread_t id1;
    pthread_t id2;
    pthread_mutex_init(&mutex,NULL);
    //pthread_mutexattr_setpshared(reinterpret_cast<pthread_mutexattr_t *>(&mutex), 0);
    pthread_create(&id1,NULL,print_msg,NULL);
    pthread_create(&id2,NULL,print_msg2,NULL);
    pthread_join(id1,NULL);  //�ػ�thread1����
    //pthread_join(id2,NULL);  //�ػ�thread2����
    int i=0;
    for(i=0;i<20;i++)
    {
        //pthread_mutex_lock(&mutex);  //mutex����
        printf("���߳� : %d\n",i);
        //usleep(200);
        //pthread_mutex_unlock(&mutex);  //mutex����
    }
    pthread_mutex_destroy(&mutex);
#endif
    return 0;
}
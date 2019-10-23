#include <iostream>
#include <pthread.h>
#include <windows.h>
#include <unistd.h>
using namespace std;
#define NUM_THREADS 10
#define EASY 0
#define MID 0
#define JOIN_DETACH 1
#define DIFF 0

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
    return 0;
}
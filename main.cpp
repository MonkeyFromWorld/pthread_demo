#include <iostream>
#include <pthread.h>
using namespace std;
#define NUM_THREADS 10
#define EASY 0
#define MID 1
#define DIFF 0

void* sayHello() {
    cout << "Hello Robot!" << endl;
    return nullptr;
}

void *PrintHello(void *threadid) {
    // �Դ���Ĳ�������ǿ������ת������������ָ���Ϊ������ָ�룬Ȼ���ٶ�ȡ
    int tid = *((int*)threadid);
    cout << "Hello Runoob! �߳� ID: " << tid << endl;
    pthread_exit(nullptr);
    return nullptr;
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
        cout << "main(),�����߳�: " << i << endl;
        index[i] = i;
        // �����ʱ�����ǿ��ת��Ϊvoid* ���ͣ���������ָ��
        rc = pthread_create(&threads[i], nullptr, PrintHello, (void *)&(index[i]));
        if(rc) {
            cout << "Error:�޷������߳�," << rc << endl;
            exit(-1);
        }
    }
    pthread_exit(nullptr);
#endif
    return 0;
}
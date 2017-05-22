#include <iostream>
#include <stdio.h>
#include <process.h>  
#include <windows.h>  
#include <list>

using std::cout;
using std::endl;

class Hall
{
public:
    Hall();
    ~Hall();
    
};

long g_nNum;  
DWORD WINAPI Fun(void *pPM);  
const int THREAD_NUM = 10;  
HANDLE  g_hThreadEvent;  
CRITICAL_SECTION g_csThreadCode;  
int main()  
{  
    g_hThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    cout << "thread event is " << g_hThreadEvent << endl;
    // InitializeCriticalSection(&g_csThreadCode);  
  
    HANDLE  handle[THREAD_NUM];
    g_nNum = 0;  
    int i = 0;  
    while (i < THREAD_NUM)
    {  
        // DWORD threadId;
        // handle[i] = (HANDLE)_beginthreadex(NULL, 0, Fun, &i, 0, NULL);
        int c = i;
        cout << "c addr = " << &c << endl;
        handle[i] = CreateThread(NULL, 0, Fun, &c, 0, NULL);
        // WaitForSingleObject(g_hThreadEvent, INFINITE);
        // WaitForSingleObject(handle[i], INFINITE);
        i++;  
    }  
    Sleep(1000);
    // WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);  
  
    // CloseHandle(g_hThreadEvent);  
    // DeleteCriticalSection(&g_csThreadCode);  
    return 0;  
}

DWORD WINAPI Fun(LPVOID pPM)
{  
    int nThreadNum = *(int *)pPM;
    // cout << "run thread " << nThreadNum << endl;
    // SetEvent(g_hThreadEvent); //触发事件  
      
    // Sleep(50);//some work should to do  
      
    // EnterCriticalSection(&g_csThreadCode);  
    g_nNum++;  
    // Sleep(0);//some work should to do  
    printf("Thread num is %d  gloabl num is %d\n", nThreadNum, g_nNum);   
    // LeaveCriticalSection(&g_csThreadCode);  
    return 0;  
}  
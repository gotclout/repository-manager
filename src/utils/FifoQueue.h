#ifndef _Queue_
#define _Queue_

#include <pthread.h>

//Allow processes to share mutex
#ifndef _POSIX_THREAD_PROCESS_SHARED
#warning "Shared mutex not supported by this platform"
#endif

using namespace std;

/**
 * @author Robert L. Foster, Jr.
 * @version 0.0
 * @since March 9, 2012
 * @class Mutex
 * @brief This class is a pthread mutex wrapper
 */
class Mutex
{
  private:

  /** A pthread mutex pointer **/
  pthread_mutex_t* mutex;

  public:

  /**
   * Class constructor Allocates and initializes the pthread mutex pointer
   */
  Mutex()
  {
    mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mutex, NULL);
  };

  /**
   * Locks the critical section
   */
  void lock()   { pthread_mutex_lock(mutex); };

  /**
   * Unlocks the critical section
   */
  void unlock() { pthread_mutex_unlock(mutex); };

  /**
   * Destroys an allocated mutex after unlocking it
   */
  ~Mutex()
  {
    if(mutex)
    {
      unlock();
      pthread_mutex_destroy(mutex);
      mutex = NULL;
    }
  };
};

template <class Object>
class qnode{

  public:
    Object data;
    qnode* next;

    qnode(Object & o){next = NULL; data = o;};
};

template <class Object>
class queue{

  private:

  int qsize;
  class qnode<Object>* head;
  class qnode<Object>* tail;
  class qnode<Object>* current;
  Mutex mutex;

  public:

  queue(){qsize = 0; head = tail = current = NULL;};

  void enqueue(Object & o)
  {
    mutex.lock();
    current = new qnode<Object>(o);
    qsize++;
    if(!head)
    {
      tail = head = current;
      head->next = NULL;
    }
    else
    {
      if(head == tail)
        head->next = tail;
      tail->next = current;
      current->next = NULL;
      tail = current;
    }
    mutex.unlock();
  };

  Object dequeue()
  {
    Object o = NULL;
    if(head)
    {
      mutex.lock();
      qsize--;
      o = head->data;
      current = head->next;
      delete head;
      if(!current)
        head = tail = current = NULL;
      else
        head = current;
      mutex.unlock();
    }
    return o;
  };

  int const size() const {return qsize;};

  bool empty() {return qsize == 0;};

  ~queue()
  {
    while(head)
    {
      current = head;
      head = head->next;
      delete current;
      current = NULL;
    }
  }
};

#endif //_Queue_

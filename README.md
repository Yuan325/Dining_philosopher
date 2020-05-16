# Dining_philosopher
Solution for the Dining Philosopher problem using pthread_mutex locks and condition variables.


# The Philosophers

  Begin by creating five philosophers, each identified by a number 0 . . 4. Each philosopher will run as a separate thread. Thread creation using Pthreads is covered in Section 4.4.1. Philosophers alternate between thinking and eating. To simulate both activities, have the thread sleep() for a random period between one and three seconds. When a philosopher wishes to eat, she invokes the function

          pickup_forks(int philosopher_number)

  where philosopher number identifies the number of the philosopher wishing to eat. When a philosopher finishes eating, she invokes

          return_forks(int philosopher_number)

# Pthreads Condition Variables

  Condition variables in Pthreads behave similarly to those described in Section 5.8. However, in that section, condition variables are used within the context of a monitor, which provides a locking mechanism to ensure data integrity. Since Pthreads is typically used in C programs — and since C does not have a monitor — we accomplish locking by associating a condition variable with a mutex lock. Pthreads mutex locks are covered in Section 5.9.4. We cover Pthreads condition variables here.

  Condition variables in Pthreads use the pthread_cond_t data type and are initialized using the pthread¬_cond_init() function. The following code creates and initializes a condition variable as well as its associated mutex lock:

          pthread_mutex_t mutex;
          pthread_cond_t cond_var;

          pthread_mutex_init(&mutex, NULL);
          pthread_cond_init(&cond_var, NULL);

  The pthread_cond_wait() function is used for waiting on a condition variable. The following code illustrates how a thread can wait for the condition a == b to become true using a Pthread condition variable:

          pthread_mutex_lock(&mutex);
          while (a != b)
          pthread_cond_wait(&mutex, &cond_var);
          pthread_mutex_unlock(&mutex);

  The mutex lock associated with the condition variable must be locked before the pthread_cond_wait() function is called, since it is used to protect the data in the conditional clause from a possible race condition. Once this lock is acquired, the thread can check the condition. If the condition is not true, the thread then invokes pthread_cond_wait(), passing the mutex lock and the condition variable as parameters. Calling pthread_cond_wait() releases the mutex lock, thereby allowing another thread to access the shared data and possibly update its value so that the condition clause evaluates to true. (To protect against program errors, it is important to place the conditional clause within a loop so that the condition is rechecked after being signaled.) 

  A thread that modifies the shared data can invoke the pthread_cond_signal() function, thereby signaling one thread waiting on the condition variable. This is illustrated below:

          pthread_mutex_lock(&mutex);
          a = b;
          pthread_cond_signal(&cond_var);
          pthread_mutex_unlock(&mutex);

  It is important to note that the call to pthread_cond_signal() does not release the mutex lock. It is the subsequent call to pthread_mutex_unlock() that releases the mutex. Once the mutex lock is released, the signaled thread becomes the owner of the mutex lock and returns control from the call to pthread_cond_wait().

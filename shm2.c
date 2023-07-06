#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int shm_id;
key_t mem_key;
int *shm_ptr;
int main()
{
    mem_key = ftok(".", 'a');
    shm_id = shmget(mem_key, 4 * sizeof(int), 0666);
    if (shm_id < 0)
    {
        printf("*** shmget error (client) ***\n");
        return 1;
    }

    shm_ptr = (int *)shmat(shm_id, NULL, 0);
    if ((int)shm_ptr == -1)
    { /* attach */
        printf("*** shmat error (client) ***\n");
        return 1;
    }
    printf("shm = %d\n", *shm_ptr);
    return 0;
}
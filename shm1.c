#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
int main()
{
    int shm_id;
    key_t mem_key;
    int *shm_ptr;

    mem_key = ftok(".", 'a');
    shm_id = shmget(mem_key, 4 * sizeof(int), IPC_CREAT | 0666);
    if (shm_id < 0)
    {
        printf("*** shmget error (server) ***\n");
        return 1;
    }

    shm_ptr = (int *)shmat(shm_id, NULL, 0); /* attach */

    if ((int)shm_ptr == -1)
    {
        printf("*** shmat error (server) ***\n");
        return 1;
    }
    *shm_ptr = 99;
    return 0;
}
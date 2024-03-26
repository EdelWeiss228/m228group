#include "Mem.h"
#include "iostream"

int _size = -1, real_size = 0, count = 0;
typedef Mem::mem_handle_t mem_handle_t;
mem_handle_t* array = NULL;

int create(int size) {
    if (_size != -1) {
        return 0;
    }
    _size = size;
    return 1;
}

int destroy() {
    if (_size == -1) {
        return 0;
    }
    if (array != NULL)
    {
        free(array);
        array = NULL;
    }
    _size = -1;
    
    return 1;
}

mem_handle_t allocMem(int block_size) {
    if (_size == -1)
    {
        return mem_handle_t(0, 0);
    }
    if (_size - real_size < block_size)   
    {
        return mem_handle_t(0, 0);
    }
    mem_handle_t* time_array = array; 
    for (int i = 0; i < count; i++) {

        if (array[i].addr == -1 && array[i].size == -1) {
            int curblocksize = (i == 0) ? 0 : (array[i - 1].addr + array[i - 1].size);
            int size_of_corent_bloc = (i == count - 1) ? (real_size - curblocksize) : array[i + 1].addr - curblocksize;//���� ���� ������ ����
            if (size_of_corent_bloc >= block_size)
            {
                mem_handle_t t((i == 0) ? 0 : array[i - 1].addr + array[i - 1].size,block_size);
                if (size_of_corent_bloc > block_size)
                {
                    array = (mem_handle_t*)realloc(array, (count + 1) * sizeof(mem_handle_t));
                    if (array == NULL) {
                        return mem_handle_t(0, 0);
                    }
                    int k = 0;
                    for (int j = 0; j < count + 1; j++) {
                        if (i == j)
                        {
                            array[j] = t;
                            mem_handle_t t = { -1,-1 };
                            array[j + 1] = t;
                            j++;
                            k++;
                            continue;
                        }
                        array[j] = time_array[k];
                        k++;
                    }
                    real_size += block_size;
                    count++;
                    return array[i];
                }
                array[i] = t;
                real_size += block_size;
                count++;
                return array[i];
               
            }
        }
    }
    array = (mem_handle_t*)realloc(array, (count + 1) * sizeof(mem_handle_t));
    if (array == NULL) {
        return mem_handle_t(0, 0);
    }
    for (int i = 0; i < count; i++) {
        array[i] = time_array[i];
    }
    mem_handle_t t(real_size, block_size);
    array[count] = t;
    count++;
    real_size += block_size;
    return array[count - 1];
}

int freeMem(mem_handle_t h) {
    bool flag = false;
    for (int i = 0; i < count; i++) {
        if (array[i] == h) {
            flag = true;
            if (i == (count - 1))
            {
                count--;
            }
            array[i].addr = -1;
            array[i].size = -1;
            break;
        }
    }
    real_size -= h.size;
    if (flag == false) { return 0; }
    return 1;
}

int get_max_block_size() {
    return _size - real_size;
}

int get_free_space() {
    return _size - real_size;
}

void print_blocks() {
    for (int i = 0; i < count; i++) {
        if (array[i].size != -1 && array[i].addr != -1)
            printf("%d %d\n", array[i].addr, array[i].size);
        else
            printf("EMPTY\n");
    }
}

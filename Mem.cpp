#include "Mem.h"
#include "iostream"

typedef Mem::mem_handle mem_handle_t;

void* Mem::allocMem(size_t block_size) {
    // if (full_size == -1){
    //     //return mem_handle_t(0, 0);
    //     //сюда надо поставить эксепш
    // }
    // if (full_size - real_size < block_size)   {
    //     //return mem_handle_t(0, 0);
    //     //сюда он нужон тоже
    // }
    // mem_handle_t* time_array = array_of_blocks; 
    // for (int i = 0; i < count; i++) {

    //     if (array_of_blocks[i].addr == -1 && array_of_blocks[i].size == -1) {
    //         int curblocksize = (i == 0) ? 0 : (array_of_blocks[i - 1].addr + array_of_blocks[i - 1].size);
    //         int size_of_corent_bloc = (i == count - 1) ? (real_size - curblocksize) : array_of_blocks[i + 1].addr - curblocksize;
    //         if (size_of_corent_bloc >= block_size)
    //         {
    //             mem_handle_t t((i == 0) ? 0 : array_of_blocks[i - 1].addr + array_of_blocks[i - 1].size,block_size);
    //             if (size_of_corent_bloc > block_size)
    //             {
    //                 array_of_blocks = (mem_handle_t*)realloc(array_of_blocks, (count + 1) * sizeof(mem_handle_t));
    //                 if (array_of_blocks == NULL) {
    //                     return array_of_elements[0];
    //                 }
    //                 int k = 0;
    //                 for (int j = 0; j < count + 1; j++) {
    //                     if (i == j)
    //                     {
    //                         array_of_blocks[j] = t;
    //                         mem_handle_t t = { -1,-1 };
    //                         array_of_blocks[j + 1] = t;
    //                         j++;
    //                         k++;
    //                         continue;
    //                     }
    //                     array_of_blocks[j] = time_array[k];
    //                     k++;
    //                 }
    //                 real_size += block_size;
    //                 count++;
    //                 return array_of_elements[array_of_blocks[i].addr];
    //             }
    //             array_of_blocks[i] = t;
    //             real_size += block_size;
    //             count++;
    //             return array_of_elements[array_of_blocks[i].addr];
               
    //         }
    //     }
    // }
    // array_of_blocks = (mem_handle_t*)realloc(array_of_blocks, (count + 1) * sizeof(mem_handle_t));
    // if (array_of_blocks == NULL) {
    //     return array_of_elements[0];
    // }
    // for (int i = 0; i < count; i++) {
    //     array_of_blocks[i] = time_array[i];
    // }
    // mem_handle_t t(real_size, block_size);
    // array_of_blocks[count] = t;
    // count++;
    // real_size += block_size;
    // return array_of_elements[array_of_blocks[count - 1].addr];
    return new char[block_size];
}

void Mem::freeMem(void*ptr) {
    // bool flag = false;
    // for (int i = 0; i < count; i++) {
    //     if (array_of_blocks[i] == h) {
    //         flag = true;
    //         if (i == (count - 1))
    //         {
    //             count--;
    //         }
    //         array_of_blocks[i].addr = -1;
    //         array_of_blocks[i].size = -1;
    //         break;
    //     }
    // }
    // real_size -= h.size;
    // if (flag == false) { return false; }
    // return true;
    delete[] ptr;
}

int Mem::get_max_block_size() {
    return full_size - real_size;
}

int Mem::get_free_space() {
    return full_size - real_size;
}

void Mem::print_blocks() {
    for (int i = 0; i < count; i++) {
        if (array_of_blocks[i].size != -1 && array_of_blocks[i].addr != -1)
            printf("%d %d\n", array_of_blocks[i].addr, array_of_blocks[i].size);
        else
            printf("EMPTY\n");
    }
}

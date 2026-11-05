                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
   
                                                                                      

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "hash-ops.h"

 
 
 
size_t tree_hash_cnt(size_t count) {
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 

	assert( count >= 3 );  
	assert( count <= 0x10000000 );  

	size_t pow = 2;
	while(pow < count) pow <<= 1;
	return pow >> 1;
}

void tree_hash(const char (*hashes)[HASH_SIZE], size_t count, char *root_hash) {
 
 
 
 
 
 
 
 
 
 
 
 

  assert(count > 0);
  if (count == 1) {
    memcpy(root_hash, hashes, HASH_SIZE);
  } else if (count == 2) {
    cn_fast_hash(hashes, 2 * HASH_SIZE, root_hash);
  } else {
    size_t i, j;

    size_t cnt = tree_hash_cnt( count );

    char *ints = calloc(cnt, HASH_SIZE);   
    assert(ints);

    memcpy(ints, hashes, (2 * cnt - count) * HASH_SIZE);

    for (i = 2 * cnt - count, j = 2 * cnt - count; j < cnt; i += 2, ++j) {
      cn_fast_hash(hashes[i], 64, ints + j * HASH_SIZE);
    }
    assert(i == count);

    while (cnt > 2) {
      cnt >>= 1;
      for (i = 0, j = 0; j < cnt; i += 2, ++j) {
        cn_fast_hash(ints + i * HASH_SIZE, 64, ints + j * HASH_SIZE);
      }
    }

    cn_fast_hash(ints, 64, root_hash);
    free(ints);
  }
}

bool tree_path(size_t count, size_t idx, uint32_t *path)
{
  if (count == 0)
    return false;

  if (count == 1) {
    *path = 0;
  } else if (count == 2) {
    *path = idx == 0 ? 0 : 1;
  } else {
    size_t i, j;

    *path = 0;
    size_t cnt = tree_hash_cnt( count );

    for (i = 2 * cnt - count, j = 2 * cnt - count; j < cnt; i += 2, ++j) {
      if (idx == i || idx == i+1)
      {
        *path = (*path << 1) | (idx == i ? 0 : 1);
        idx = j;
      }
    }
    assert(i == count);

    while (cnt > 2) {
      cnt >>= 1;
      for (i = 0, j = 0; j < cnt; i += 2, ++j) {
        if (idx == i || idx == i + 1)
        {
          *path = (*path << 1) | (idx == i ? 0 : 1);
          idx = j;
        }
      }
    }

    if (idx == 0 || idx == 1)
    {
      *path = (*path << 1) | (idx == 0 ? 0 : 1);
      idx = 0;
    }
  }
  return true;
}

bool tree_branch(const char (*hashes)[HASH_SIZE], size_t count, const char *hash, char (*branch)[HASH_SIZE], size_t *depth, uint32_t *path)
{
  size_t idx;

  if (count == 0)
    return false;

  for (idx = 0; idx < count; ++idx)
    if (!memcmp(hash, hashes[idx], HASH_SIZE))
      break;
  if (idx == count)
    return false;

  assert(count > 0);
  if (count == 1) {
    *depth = 0;
    *path = 0;
  } else if (count == 2) {
    *depth = 1;
    *path = idx == 0 ? 0 : 1;
    memcpy(branch[0], hashes[idx ^ 1], HASH_SIZE);
  } else {
    size_t i, j;

    *depth = 0;
    *path = 0;
    size_t cnt = tree_hash_cnt( count );

    char *ints = calloc(cnt, HASH_SIZE);   
    assert(ints);

    memcpy(ints, hashes, (2 * cnt - count) * HASH_SIZE);

    for (i = 2 * cnt - count, j = 2 * cnt - count; j < cnt; i += 2, ++j) {
      if (idx == i || idx == i+1)
      {
        memcpy(branch[*depth], hashes[idx == i ? i + 1 : i], HASH_SIZE);
        ++*depth;
        *path = (*path << 1) | (idx == i ? 0 : 1);
        idx = j;
      }
      cn_fast_hash(hashes[i], 64, ints + j * HASH_SIZE);
    }
    assert(i == count);

    while (cnt > 2) {
      cnt >>= 1;
      for (i = 0, j = 0; j < cnt; i += 2, ++j) {
        if (idx == i || idx == i + 1)
        {
          memcpy(branch[*depth], ints + (idx == i ? i + 1 : i) * HASH_SIZE, HASH_SIZE);
          ++*depth;
          *path = (*path << 1) | (idx == i ? 0 : 1);
          idx = j;
        }
        cn_fast_hash(ints + i * HASH_SIZE, 64, ints + j * HASH_SIZE);
      }
    }

    if (idx == 0 || idx == 1)
    {
      memcpy(branch[*depth], ints + (idx == 0 ? 1 : 0) * HASH_SIZE, HASH_SIZE);
      ++*depth;
      *path = (*path << 1) | (idx == 0 ? 0 : 1);
      idx = 0;
    }

    free(ints);
  }
  return true;
}

bool tree_branch_hash(const char hash[HASH_SIZE], const char (*branch)[HASH_SIZE], size_t depth, uint32_t path, char root[HASH_SIZE])
{
  size_t d;
  char partial[HASH_SIZE];

  memcpy(partial, hash, HASH_SIZE);

  for (d = 0; d < depth; ++d)
  {
    char buffer[2 * HASH_SIZE];
    if ((path >> (depth - d - 1)) & 1)
    {
      memcpy(buffer, branch[d], HASH_SIZE);
      memcpy(buffer + HASH_SIZE, partial, HASH_SIZE);
    }
    else
    {
      memcpy(buffer, partial, HASH_SIZE);
      memcpy(buffer + HASH_SIZE, branch[d], HASH_SIZE);
    }
    cn_fast_hash(buffer, 2 * HASH_SIZE, partial);
  }

  memcpy(root, partial, HASH_SIZE);
  return true;
}

bool is_branch_in_tree(const char hash[HASH_SIZE], const char root[HASH_SIZE], const char (*branch)[HASH_SIZE], size_t depth, uint32_t path)
{
  char res[HASH_SIZE];
  if (!tree_branch_hash(hash, branch, depth, path, res))
    return false;
  return memcmp(res, root, HASH_SIZE) == 0;
}

                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  
                                  
                                                                          
                                                                                         
                                                                            
                           

#pragma once

#include "misc_language.h"

#include <stdlib.h>
#include <stdint.h>

namespace epee
{
namespace misc_utils
{

template<typename Item>
struct rolling_median_t
{
private:
  Item* data;   
  int*  pos;    
  int*  heap;   
  int   N;      
  int   idx;    
  int   minCt;  
  int   maxCt;  
  int   sz;     

private:

   
  bool mmless(int i, int j) const
  {
    return data[heap[i]] < data[heap[j]];
  }

   
  bool mmexchange(int i, int j)
  {
    const int t = heap[i];
    heap[i] = heap[j];
    heap[j] = t;
    pos[heap[i]] = i;
    pos[heap[j]] = j;
    return 1;
  }

   
  bool mmCmpExch(int i, int j)
  {
     return mmless(i, j) && mmexchange(i, j);
  }

   
  void minSortDown(int i)
  {
    for (i *= 2; i <= minCt; i *= 2)
    {
       if (i < minCt && mmless(i + 1, i))
         ++i;
       if (!mmCmpExch(i, i / 2))
         break;
    }
  }

   
  void maxSortDown(int i)
  {
    for (i *= 2; i >= -maxCt; i *= 2)
    {
      if (i > -maxCt && mmless(i, i - 1))
        --i;
      if (!mmCmpExch(i / 2, i))
        break;
    }
  }

   
   
  bool minSortUp(int i)
  {
    while (i > 0 && mmCmpExch(i, i / 2))
      i /= 2;
    return i == 0;
  }

   
   
  bool maxSortUp(int i)
  {
     while (i < 0 && mmCmpExch(i / 2, i))
       i /= 2;
     return i == 0;
  }

protected:
  rolling_median_t &operator=(const rolling_median_t&) = delete;

public:
   
  rolling_median_t(size_t N): N(N)
  {
    int size = N * (sizeof(Item) + sizeof(int) * 2);
    data = (Item*)malloc(size);
    pos = (int*) (data + N);
    heap = pos + N + (N / 2);  
    clear();
  }

  rolling_median_t(const rolling_median_t &other)
  {
    N = other.N;
    int size = N * (sizeof(Item) + sizeof(int) * 2);
    data = (Item*)malloc(size);
    memcpy(data, other.data, size);
    pos = (int*) (data + N);
    heap = pos + N + (N / 2);  
    idx = other.idx;
    minCt = other.minCt;
    maxCt = other.maxCt;
    sz = other.sz;
  }

  rolling_median_t(rolling_median_t &&m)
  {
    memcpy(this, &m, sizeof(rolling_median_t));
    m.data = NULL;
  }
  rolling_median_t &operator=(rolling_median_t &&m)
  {
    free(data);
    memcpy(this, &m, sizeof(rolling_median_t));
    m.data = NULL;
    return *this;
  }

  ~rolling_median_t()
  {
    free(data);
  }

  void clear()
  {
    idx = 0;
    minCt = 0;
    maxCt = 0;
    sz = 0;
    int nItems = N;
    while (nItems--)   
    {
      pos[nItems] = ((nItems + 1) / 2) * ((nItems & 1) ? -1 : 1);
      heap[pos[nItems]] = nItems;
    }
  }

  int size() const
  {
    return sz;
  }

   
  void insert(Item v)
  {
    int p = pos[idx];
    Item old = data[idx];
    data[idx] = v;
    idx = (idx + 1) % N;
    sz = std::min<int>(sz + 1, N);
    if (p > 0)          
    {
      if (minCt < (N - 1) / 2)
      {
        ++minCt;
      }
      else if (v > old)
      {
        minSortDown(p);
        return;
      }
      if (minSortUp(p) && mmCmpExch(0, -1))
        maxSortDown(-1);
    }
    else if (p < 0)    
    {
      if (maxCt < N / 2)
      {
        ++maxCt;
      }
      else if (v < old)
      {
        maxSortDown(p);
        return;
      }
      if (maxSortUp(p) && minCt && mmCmpExch(1, 0))
        minSortDown(1);
    }
    else  
    {
      if (maxCt && maxSortUp(-1))
        maxSortDown(-1);
      if (minCt && minSortUp(1))
        minSortDown(1);
    }
  }

   
  Item median() const
  {
    Item v = data[heap[0]];
    if (minCt < maxCt)
    {
      v = get_mid<Item>(v, data[heap[-1]]);
    }
    return v;
  }
};

}
}

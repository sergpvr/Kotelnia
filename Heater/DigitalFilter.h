#ifndef DigitalFilter_h
#define DigitalFilter_h

#ifdef __cplusplus

#define DIGITAL_FILTER_CAPACITY       7

// el0 el1 el2 el3
//      ^
//     head

class DigitalFilter {
  private:
    int8_t elements[DIGITAL_FILTER_CAPACITY];
    int8_t *head = elements;
    
    void insert(int8_t in) {
      *head++ = in;
      if(head - elements >= DIGITAL_FILTER_CAPACITY) {
        head = elements;
      }
    }
    
    int8_t* getMin() {
      int8_t *min, *p;
      min = p = elements;
      while (++p - elements < DIGITAL_FILTER_CAPACITY) {
        if (*p < *min) {
          min = p;
        }
      }
      return min;
    }
    
    int8_t* getMax() {
      int8_t* max, *p;
      max = p = elements;
      while (++p - elements < DIGITAL_FILTER_CAPACITY) {
        if (*p >= *max) {
          max = p;
        }
      }
      return max;
    }
    
	public:
	  DigitalFilter(int8_t fillerVal) {
	    for(int8_t i = 0; i < DIGITAL_FILTER_CAPACITY; i++) {
	      insert(fillerVal);
	    }
	  }
	  
    int8_t doFilter(int8_t in) {
      insert(in);
      
      int8_t *min, *max, *p;
      min = getMin();
      max = getMax();
      p = elements;

      int avg = 0;
      while (p - elements < DIGITAL_FILTER_CAPACITY) {
        if (p != min && p != max) {
          avg += *p;
        }
        p++;
      }
      
      return (int8_t)(avg/(DIGITAL_FILTER_CAPACITY - 2));
    }
  
};

#endif // __cplusplus
#endif // DigitalFilter_h

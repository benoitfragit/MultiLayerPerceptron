#ifndef BRAIN_DATA_READER_H
#define BRAIN_DATA_READER_H

#include "brain_xml_utils.h"

typedef double* Signal;

typedef struct Data
{
    Signal* _signals;
    int*    _subset;
    int     _subset_length;
    int     _signal_length;
    int     _number_of_signal;
} Data;

Data*  new_data_from_context(Context context);
void   delete_data(Data* data);
const Signal signal(Data* data, const int index);
int get_next_random_subset_index(const int* subset,
                                 const int length,
                                 const int max_index);
#endif /* BRAIN_DATA_READER_H */

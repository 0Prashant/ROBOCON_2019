
#ifndef __FILTER_H__
#define __FILTER_H__



struct str_Filter
{
	int no_filter ; // no of data to be left unfitered
	int samples ;   // no of samples for average
	int index ;
	float last_sum;
};

void filter_init(struct str_Filter* filter,int _no_filter, int _samples);
float apply_filter(struct str_Filter* filter ,int input);

#endif //__FILTER_H__

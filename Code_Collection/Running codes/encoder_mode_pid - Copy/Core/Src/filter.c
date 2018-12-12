#include "filter.h"

float apply_filter(struct str_Filter* filter ,int input)
{
	if( filter->index <= filter->no_filter) // output unfiltered 
	{
			
		if((filter->no_filter-filter->index) <= (filter->samples-1))
		{
			filter->last_sum += input;
		}
		
		filter->index ++;
		return input;	
	}
	
	// filter algorithm : subtracting mean and adding a data each time
	float sum = filter->last_sum + input - filter->last_sum/filter->samples;
	filter->last_sum = sum;
	return sum/filter->samples ; 
				
}

void filter_init(struct str_Filter* filter,int _no_filter, int _samples)
{
	filter->no_filter = _no_filter;
	filter->samples = _samples;
	filter->index = 1;
	
}

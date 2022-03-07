#include <limits.h>

int
	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int
	ft_atoiu(unsigned int *out, const char *str)
{
	long	result;

	result = 0;
	while (ft_isdigit(*str))
	{
		result = 10 * result + (*str - '0');
		str++;
	}
	*out = (int) result;
	return (result > UINT_MAX || *str != '\0');
}

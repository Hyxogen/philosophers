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
	int		len;

	result = 0;
	len = 0;
	while (len < 12 && ft_isdigit(*str))
	{
		result = 10 * result + (*str - '0');
		str++;
		len++;
	}
	*out = (int) result;
	return (result <= UINT_MAX && *str == '\0' && len < 12);
}

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 1000

char	*get_next_line(int fd)
{
	static int	pos;
	char		*c;
	int			sz;
	int			i;
	char		*line;
	int			j;

	pos = 0;
	c = malloc(BUFFER_SIZE + 1);
	if (!c)
		return (NULL);
	sz = read(fd, c, BUFFER_SIZE);

	if (sz == -1)
	{
		free(c);
		return (NULL);
	}
	c[sz] = '\0';
	i = pos;
	while (i < sz)
	{
		if (c[i] == '\n')
		{
			i++;
			break ;
		}
		i++;
	}
	line = malloc(i - pos + 1);
	j = 0;
	while (pos < i)
		line[j++] = c[pos++];
	line[j] = '\0';
	free(c);
	return (line);
}

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;

	if (argc < 2)
		return (1);
	fd = open(argv[1], O_RDONLY);
	line = get_next_line(fd);
	printf("%s", line);
	free(line);

	line = get_next_line(fd);
	printf("%s", line);
	free(line);

	close(fd);
	return (0);
}

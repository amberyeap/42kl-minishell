#include <stdio.h>
#include <stdlib.h>

// typedef struct s_test1
// {
// 	struct s_test2 *test2;
// }	t_test1;

// struct s_test2
// {
// 	int x;
// } ;

// int main()
// {
// 	t_test1 test1;

// 	test1.test2 = malloc(sizeof(t_test1));
// 	if (!test1.test2)
// 		return (1);

// 	test1.test2->x = 5;

// 	printf("%d\n", test1.test2->x);
// }

int main(int argc, char **argv)
{
	(void)argv;
	if (argc != 2)
	{
		perror("Error\n");
		// fprintf(stderr, "fprintf error\n");
		exit(EXIT_FAILURE);
	}
}
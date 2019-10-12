#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define STANDARD_INPUT_FD 0

char buf[512];

void
cpt(int source_fd, int destination_fd)
{
  int read_length;

  while((read_length = read(source_fd, buf, sizeof(buf))) > 0)
  {
    if (write(destination_fd, buf, read_length) != read_length)
    {
      printf(1, "cpt: write error!\n");
      exit();
    }
  }

  if(read_length < 0)
  {
    printf(1, "cpt: read error!\n");
    exit();
  }
}

int
main(int argc, char *argv[])
{
    if(argc <= 1)
    {
        printf(1, "cpt: too few arguments!\n");
        exit();
    }

    if (argc > 3)
    {
        printf(1, "cpt: too many arguments!\n");
        exit();
    }

    int source_fd, destination_fd;

    if(argc == 3)
    {
        if ((source_fd = open(argv[1], O_RDONLY)) < 0)
        {
            printf(1, "cpt: cannot open source file %s\n", argv[1]);
            exit();
        }

        if (!((destination_fd = open(argv[2], O_RDWR)) < 0) && unlink(argv[2]) < 0)
        {
            printf(1, "cpt: cannot open destination file %s\n", argv[2]);
            exit();
        }

        if ((destination_fd = open(argv[2], O_CREATE | O_RDWR)) < 0)
        {
            printf(1, "cpt: cannot create destination file %s\n", argv[2]);
            exit();
        }
    }
    else
    {
        if (!((destination_fd = open(argv[1], O_RDWR)) < 0) && unlink(argv[1]) < 0)
        {
            printf(1, "cpt: cannot open destination file %s\n", argv[1]);
            exit();
        }

        if ((destination_fd = open(argv[1], O_CREATE | O_RDWR)) < 0)
        {
            printf(1, "cpt: cannot create destination file %s\n", argv[1]);
            exit();
        }

        source_fd = STANDARD_INPUT_FD;
    }

    cpt(source_fd, destination_fd);

    if (source_fd)
        close(source_fd);

    close(destination_fd);
    exit();
}

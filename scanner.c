#define __USE_MINGW_ANSI_STDIO 1

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

struct str
{
    char data;
    struct str *prev;
    struct str *next;
};

int input(struct str *top, int *k)
{
    if (top == NULL)
    {
        printf("\ninput error");
        return 1;
    }

    if (k == NULL)
    {
        printf("\ninput error");
        return 2;
    }

    int check;
    struct str *p, *buf;
    *k = 0;

    check = printf(" file path: ");
    if (check < 0)
    {
        printf("\noutput error");
        return 3;
    }
    top->data = getchar();
    if (feof(stdin))
    {
        printf("\neof / input error");
        return 4;
    }
    else if (ferror(stdin))
    {
        printf("\ninput error");
        return 4;
    }

    while (top->data != '\n')
    {
        p = (struct str *)malloc(sizeof(struct str));
        if (p == NULL)
        {
            printf("\nmemory allocation error");
            return 5;
        }
        p->data = getchar();
        if (feof(stdin))
        {
            printf("\neof / input error");
            return 6;
        }
        else if (ferror(stdin))
        {
            printf("\ninput error");
            return 6;
        }
        p->prev = top;
        top->next = p;
        top = p;
        *k = *k + 1;
    }
    top->next = NULL;

    buf = (struct str *)malloc(sizeof(struct str));
    if (buf == NULL)
    {
        printf("\nmemory allocation error");
        return 7;
    }

    while (top != NULL)
    {
        buf = top;
        top = top->prev;
    }

    top = buf;
    if (top == NULL)
    {
        printf("\nmemory allocation error");
        return 8;
    }

    return 0;
}

int direct(struct str *dir, char *direction)
{
    if (dir == NULL)
    {
        printf("\ninput error");
        return 1;
    }

    int i = 0;

    while (dir != NULL)
    {
        direction[i] = dir->data;
        i = i + 1;
        dir = dir->next;
    }

    if (direction == NULL)
    {
        printf("\ninput error");
        return 2;
    }

    return 0;
}

int main()
{
    FILE *base;
    FILE *sample;
    unsigned char signature[8];
    unsigned char object[8];
    int offset;
    char format[2];
    int i;
    int check;
    int k = 0;
    struct str *path = NULL;
    char *base_path = NULL;
    char *file_path = NULL;

    path = (struct str *)malloc(sizeof(struct str));
    if (path == NULL)
    {
        printf("\nmemory allocation error");
        return 1;
    }
    path->prev = NULL;
    path->next = NULL;

    check = printf("enter database");
    if (check < 0)
    {
        printf("\noutput error");
        return 2;
    }
    check = input(path, &k);
    if (check != 0)
    {
        printf("\n\npress enter to exit");
        getchar();
        return 3;
    }

    base_path = (char *)malloc((k + 1) * sizeof(char));
    if (base_path == NULL)
    {
        printf("\nmemory allocation error");
        return 4;
    }

    check = direct(path, base_path);
    if (check != 0)
    {
        printf("\n\npress enter to exit");
        getchar();
        return 3;
    }
    base_path[k] = '\0';

    base = fopen(base_path, "r");
    if (base == NULL)
    {
        printf("\ninvalid path");
        printf("\n\npress enter to exit");
        getchar();
        return 5;
    }

    check = fscanf(base, "%hhu%hhu%hhu%hhu%hhu%hhu%hhu%hhu%d",
                   &signature[0], &signature[1], &signature[2],
                   &signature[3], &signature[4], &signature[5],
                   &signature[6], &signature[7], &offset);
    if (check != 9)
    {
        printf("\nerror entering values / invalid file");
        printf("\ncheck your file and try again");
        printf("\n\npress enter to exit");
        fclose(base);
        getchar();
        return 6;
    }
    if (fclose(base) != 0)
    {
        printf("\ninvalid file");
        printf("\ncheck your file and try again");
        printf("\n\npress enter to exit");
        fclose(base);
        getchar();
        return 7;
    }

    check = printf("enter");
    if (check < 0)
    {
        printf("\noutput error");
        return 8;
    }
    check = input(path, &k);
    if (check != 0)
    {
        printf("\n\npress enter to exit");
        getchar();
        return 9;
    }
    file_path = (char *)malloc((k + 1) * sizeof(char));
    if (file_path == NULL)
    {
        printf("\nmemory allocation error");
        return 4;
    }

    check = direct(path, file_path);
    if (check != 0)
    {
        printf("\n\npress enter to exit");
        getchar();
        return 3;
    }
    file_path[k] = '\0';

    sample = fopen(file_path, "rb");
    if (sample == NULL)
    {
        printf("\ninvalid path");
        printf("\n\npress enter to exit");
        getchar();
        return 10;
    }

    check = fread(format, sizeof(char), 2, sample);
    if (check != 2)
    {
        printf("\nerror reading file / invalid file");
        printf("\ncheck your file and try again");
        printf("\n\npress enter to exit");
        fclose(sample);
        getchar();
        return 11;
    }

    if (format[0] != 'M' || format[1] != 'Z')
    {
        printf("\nnot PE file -- safe");
        printf("\n\npress enter to exit");
        fclose(sample);
        getchar();
        return 0;
    }

    check = fseek(sample, offset, SEEK_SET);
    if (check != 0)
    {
        printf("\nerror reading file");
        printf("\ncheck your file and try again");
        printf("\n\npress enter to exit");
        fclose(sample);
        getchar();
        return 12;
    }

    if (fread(object, sizeof(unsigned char), 8, sample) != 8)
    {
        if (feof(sample))
            printf("\nerror: end of file");
        else
            printf("\nerror reading file / invalid file");
        printf("\ncheck your file and try again");
        printf("\n\npress enter to exit");
        fclose(sample);
        getchar();
        return 13;
    }

    if (fclose(sample) != 0)
    {
        printf("\ninvalid file");
        printf("\ncheck your file and try again");
        printf("\n\npress enter to exit");
        fclose(sample);
        getchar();
        return 14;
    }

    printf("\nchecking your file...");
    if (check < 0)
    {
        printf("\noutput error");
        return 15;
    }

    for (i = 0; i < 8; i++)
    {
        if (object[i] != signature[i])
        {
            check = printf("\nsafe!\n\npress enter to exit");
            if (check < 0)
            {
                printf("\noutput error");
                return 16;
            }
            getchar();
            return 0;
        }
    }

    check = printf("\noops! INfECTION !! ! delete this file for safety reasons\n\npress enter to exit");
    if (check < 0)
    {
        printf("\noutput error");
        return 16;
    }

    getchar();
    return 0;
}

#include "ClUtils.h"

using namespace std;

// obtain source code from cl file
string clLoadProgramSource(const char* filename)
{
    // open file
    FILE* file = fopen(filename, "rb");
    if(file == NULL)
    {
        return string();
    }

    // get file size
    if(fseek(file, 0, SEEK_END) != 0)
    {
        perror("fseek: ");
        printf("%s, %d\n", __FILE__, __LINE__);
        exit(1);
    }

    size_t size = ftell(file);
    if(size == -1)
    {
        perror("ftell: ");
        printf("%s, %d\n", __FILE__, __LINE__);
        exit(1);
    }

    // begin of file
    if(fseek(file, 0, SEEK_SET) != 0)
    {
        perror("fseek: ");
        printf("%s, %d\n", __FILE__, __LINE__);
        exit(1);
    }

    // read file
    char* source = (char *)malloc(size + 1);
    if(fread(source, size, 1, file) != 1)
    {
        fclose(file);
        free(source);
        perror("fread: ");
        printf("%s, %d\n", __FILE__, __LINE__);
        exit(1);
    }

    string s(source, size);

    fclose(file);
    free(source);

    return s;
}
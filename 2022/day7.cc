#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <cstdlib>
#include <unordered_map>

#define TOTAL_DISK_SPACE 70000000
#define SPACE_NEEDED 30000000

typedef struct dir
{
    std::string name;
    unsigned int size;
    struct dir *parent;
    std::vector<std::string> children;
} dir;

void calculateDirectorySizes(std::unordered_map<std::string, dir *> &fs, dir *d)
{
    for(auto &child : d->children)
    {
        calculateDirectorySizes(fs, fs[child]);
    }

    if(d->parent != nullptr) // parent exists
    {
        d->parent->size += d->size;
    }
}

int main()
{
    FILE *f = fopen("day7.txt", "r");
    char line_buffer[32];

    std::unordered_map<std::string, dir *> fs;
    dir *dir_s = nullptr;
    dir *parent_dir_s = nullptr;
    std::string filepath("");  // key is to use filepath as filesystem key, not just directory name, which isn't unique

    while (fgets(line_buffer, 32, f) != NULL && feof(f) == 0)
    {
        line_buffer[strlen(line_buffer) - 1] = 0; // remove '\n'

        // Command
        if(line_buffer[0] == '$')
        {
            if(line_buffer[2] == 'c' && line_buffer[3] == 'd')
            {
                if(strstr(line_buffer, ".."))
                {
                    if(dir_s && dir_s->parent)
                    {
                        int pos = filepath.find_last_of('/');
                        filepath.erase(pos, 1);
                        pos = filepath.find_last_of('/');
                        filepath.erase(pos + 1, filepath.size() - (pos + 1));

                        // Go down a level
                        dir_s = dir_s->parent;
                    }
                }
                else
                {
                    // Go up a level
                    parent_dir_s = dir_s;

                    // Create dir entry
                    dir_s = new dir();
                    filepath.append(&line_buffer[5]);
                    if(line_buffer[5] != '/')
                    {
                        filepath.append("/");
                    }
                    dir_s->name = filepath;
                    dir_s->size = 0;
                    dir_s->parent = parent_dir_s;

                    // Add dir to fs
                    if(fs.find(dir_s->name) == fs.end())
                    {
                        fs[dir_s->name] = dir_s;
                    }
                    else
                    {
                        printf("found dup directory\n");
                    }
                }
            }
            else if(line_buffer[2] == 'l' && line_buffer[3] == 's')
            {
                continue;
            }
        }
        else if(strstr(line_buffer, "dir"))
        {
            std::string child(filepath);
            child.append(&line_buffer[4]);
            child.append("/");
            dir_s->children.push_back(child);
        }
        else
        {
            line_buffer[strcspn(line_buffer, " ")] = 0;
            dir_s->size += atoi(line_buffer);
        }
    }
    fclose(f);

    // Recursively calculate directory sizes
    calculateDirectorySizes(fs, fs["/"]);

    // Calculate total size for dirs > 100000
    // unsigned int total_size = 0;
    // for(auto itr = fs.begin(); itr != fs.end(); itr++)
    // {
    //     // printf("%s: %u\n", fs[itr->first]->name.c_str(), fs[itr->first]->size);
    //     // for(auto &child : fs[itr->first]->children)
    //     // {
    //     //     printf("%s ", child.c_str());
    //     // }
    //     // printf("\n");
    //     if(fs[itr->first]->size <= 100000)
    //     {
    //         total_size += fs[itr->first]->size;
    //     }
    // }
    unsigned int total_size = fs["/"]->size;
    printf("Total size:              %u\n", total_size);
    printf("Available space:         %u\n", TOTAL_DISK_SPACE - total_size);
    unsigned int directory_threshold = SPACE_NEEDED - (TOTAL_DISK_SPACE - total_size);
    printf("Additional space needed: %u\n", directory_threshold);

    int min = total_size;
    std::string min_name;
    for(auto itr = fs.begin(); itr != fs.end(); itr++)
    {
        if(fs[itr->first]->size <= min && fs[itr->first]->size >= directory_threshold)
        {
            min_name = fs[itr->first]->name;
            min = fs[itr->first]->size;
        }
    }
    printf("%s\n", min_name.c_str());
    printf("%u\n", fs[min_name]->size);
    return 0;
}
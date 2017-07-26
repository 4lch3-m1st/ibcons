#include "postview.h"

void dispose_post(postcontent_t* content)
{
    free(sub);
    free(com);
    free(name);
    // free(filename);
    // free(ext);
    // free(tim);
    // free(md5);
    free(content);
}

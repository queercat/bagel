enum token_type
{
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_SYMBOL,
    TOKEN_TYPE_LIST,
    TOKEN_TYPE_LIST_END,
};

typedef struct bgl_token
{
    char *text;
    enum token_type type;
} bgl_token;

typedef struct bgl_dynamic_array
{
    int length;
    int capacity;
    size_t type_size;
    void **data;
    // reference -> array -> start of memory
} bgl_dynamic_array;

typedef struct bgl_token_list
{
    struct bgl_token_list *previous;
    bgl_token *current;
    struct bgl_token_list *next;
} bgl_token_list;

enum bgl_data_type
{
    BGL_DATA_TYPE_STRING,
    BGL_DATA_TYPE_NUMBER,
    BGL_DATA_TYPE_SYMBOL,
    BGL_DATA_TYPE_LIST,
};

typedef struct bgl_data
{
    enum bgl_data_type type;
    union
    {
        char *string;
        int number;
        char *symbol;
        bgl_dynamic_array *array;
    } value;
} bgl_data;

bgl_data *bgl_read_string(bgl_token_list *);
bgl_data *bgl_read_number(bgl_token_list *);
bgl_data *bgl_read_symbol(bgl_token_list *);

bgl_data *bgl_read_form(bgl_token_list **);
bgl_data *bgl_read_atomic(bgl_token_list **);
bgl_data *bgl_read_list(bgl_token_list **);
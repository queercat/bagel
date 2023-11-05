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
} type;

typedef struct bgl_data
{
    enum bgl_data_type type;

    union bgl_data_value
    {
        char *string;
        int number;
        char *symbol;
    } value;

    struct bgl_data *next;
} bgl_data;

bgl_data *bgl_read_list(bgl_token_list *);
bgl_data *bgl_read_atom(bgl_token_list *);
bgl_data *bgl_read_form(bgl_token_list *);
bgl_data *bgl_read_string(bgl_token_list *);
bgl_data *bgl_read_number(bgl_token_list *);
bgl_data *bgl_read_symbol(bgl_token_list *);
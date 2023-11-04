#define bool int
#define false 0
#define true 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** bgl token handling */

enum token_type
{
  TOKEN_TYPE_STRING,
  TOKEN_TYPE_NUMBER,
  TOKEN_TYPE_SYMBOL,
  TOKEN_TYPE_LIST,
};

typedef struct bgl_token
{
  char *text;
  enum token_type type;
} bgl_token;

bgl_token *bgl_create_token(char *text)
{
  bgl_token *token = malloc(sizeof(bgl_token));

  token->text = text;

  return token;
}

typedef struct bgl_token_list
{
  struct bgl_token_list *previous;
  bgl_token *current;
  struct bgl_token_list *next;
} bgl_token_list;

bgl_token_list *bgl_create_token_list(bgl_token *current)
{
  bgl_token_list *list = malloc(sizeof(bgl_token_list));

  list->previous = NULL;
  list->current = current;
  list->next = NULL;

  return list;
}

bgl_token_list *bgl_list_at(bgl_token_list *list, int idx)
{
  bgl_token_list *current = list;
  int i = 0;

  while (i < idx)
  {
    if (current == NULL)
      return NULL;

    i++;
    current = current->next;
  }

  return current;
}

void *bgl_token_list_insert(bgl_token_list **list, bgl_token *token)
{
  bgl_token_list *current = *list;

  bgl_token_list *new_list = bgl_create_token_list(token);

  new_list->next = current;
  current->previous = new_list;

  *list = new_list;
}

void *bgl_token_list_append(bgl_token_list **list, bgl_token *token)
{
  bgl_token_list *current = *list;

  while (current->next != NULL)
  {
    current = current->next;
  }

  bgl_token_list *new_list = bgl_create_token_list(token);

  current->next = new_list;
  new_list->previous = current;
}

bgl_token_list *bgl_list_get_head(bgl_token_list *list)
{
  bgl_token_list *current = list;

  while (current->previous != NULL)
  {
    current = current->previous;
  }

  return current;
}

void *bgl_token_list_print(bgl_token_list *list)
{
  bgl_token_list *current = list;

  while (current != NULL)
  {
    printf("%s\n", current->current->text);
    current = current->next;
  }
}

bgl_token_list *bgl_tokenize(char *text)
{
  bgl_token_list *list = NULL;
  bgl_token_list *current = NULL;

  char *c = text;

  while (*c != '\0')
  {
    char *start = c;
    char *end = NULL;
    bgl_token *token = bgl_create_token(NULL);

    if (*c == '(' || *c == ')')
    {
      token->type = TOKEN_TYPE_LIST;
      end = c + 1;
    }

    else if (*c == ' ')
    {
      c++;
      continue;
    }

    else if (*c >= '0' && *c <= '9')
    {
      token->type = TOKEN_TYPE_NUMBER;
      end = c + 1;

      while (*end >= '0' && *end <= '9')
      {
        end++;
      }
    }

    else if (*c == '"')
    {
      token->type = TOKEN_TYPE_STRING;
      end = c + 1;

      while (*end != '"')
      {
        end++;
      }

      end++;
    }

    else if ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z'))
    {
      token->type = TOKEN_TYPE_SYMBOL;
      end = c + 1;

      while ((*end >= 'a' && *end <= 'z') || (*end >= 'A' && *end <= 'Z') || *end == '-')
      {
        end++;
      }
    }

    char *token_text = malloc(sizeof(char) * (end - start + 1));
    strncpy(token_text, start, end - start);
    token_text[end - start] = '\0';
    token->text = token_text;

    if (token->type == TOKEN_TYPE_STRING)
    {
      token->text++;
      token->text[end - start - 2] = '\0';
    }

    if (list == NULL)
    {
      list = bgl_create_token_list(token);
      current = list;
    }
    else
    {
      bgl_token_list_append(&list, token);
      current = current->next;
    }

    c = end;
  }

  list = bgl_token_list_get_head(list);

  bgl_token_list_print(list);

  return list;
}

/** end bgl token handling */

void _strip_buffer(char *buffer)
{
  buffer[strcspn(buffer, "\n\r")] = 0;
}

void bgl_error(char *message)
{
  printf("%s", message);
  exit(1);
}

bgl_token_list *bgl_read()
{
  int size = 1024;
  char *line = malloc(sizeof(char) * size);
  printf("> ");

  fgets(line, size, stdin);
  _strip_buffer(line);

  bgl_token_list *tokens = bgl_tokenize(line);

  free(line);

  return tokens;
}

/** bgl data types */
typedef struct bgl_data
{
  enum bgl_data_type
  {
    BGL_DATA_TYPE_STRING,
    BGL_DATA_TYPE_NUMBER,
    BGL_DATA_TYPE_SYMBOL,
    BGL_DATA_TYPE_LIST,
  } type;

  union bgl_data_value
  {
    char *string;
    int number;
    char *symbol;
    struct bgl_data *list;
  } value;

  struct bgl_data *next;
} bgl_data;

bgl_data *bgl_create_data(enum bgl_data_type type)
{
  bgl_data *data = malloc(sizeof(bgl_data));

  data->type = type;
  data->next = NULL;

  return data;
}

bgl_data *bgl_evaluate(bgl_token_list *tokens)
{
}

/** end bgl data types */

bgl_token_list *bgl_eval(bgl_token_list *tokens)
{
  return tokens;
}

void bgl_print(bgl_token_list *tokens)
{
  free(tokens);
}

bool _assert(bool boolean, char *test_name)
{
  printf("[Test %s]: ", test_name);

  if (boolean == false)
  {
    printf("result: FAILURE\n");
    return false;
  }

  printf("- result: SUCCESS\n");
  return true;
}

void bgl_tests()
{
  bool failed = false;

  printf("beginning tests...\n");

  // can create bagel token.
  bgl_token *token_0 = bgl_create_token("bagel");

  if (!_assert(token_0 != NULL, "Token_WhenCreated_ShouldNotBeNull"))
    failed = true;
  if (!_assert(strcmp(token_0->text, "bagel") == 0, "Token_WhenCreated_TextValueShouldBeCorrect"))
    failed = true;

  bgl_token *token_1 = bgl_create_token("cream cheese");
  bgl_token *token_2 = bgl_create_token("lox");

  // can create bagel lists.
  bgl_token_list *list_0 = bgl_create_token_list(token_0);

  if (!_assert(list_0 != NULL, "List_WhenCreated_ShouldNotBeNull"))
    failed = true;

  bgl_token_list *list_1 = bgl_create_token_list(token_1);
  bgl_token_list *list_2 = bgl_create_token_list(token_2);

  list_0->next = list_1;
  list_1->next = list_2;

  if (!_assert(bgl_token_list_at(list_0, 1) == list_1, "List_WhenCreated_ShouldHaveNext"))
    failed = true;

  if (!_assert(bgl_token_list_at(list_0, 69) == NULL, "List_OverIndex_ShouldBeNullAndNotCrash"))
    failed = true;

  // can tokenize bagel text.
  bgl_token_list *list_3 = bgl_tokenize("bagel cream cheese lox");

  if (!_assert(list_3 != NULL, "Tokenize_WhenCalled_ShouldNotBeNull"))
    failed = true;

  if (!_assert(strcmp(list_3->current->text, "bagel") == 0, "Tokenize_WhenCalled_ShouldHaveCorrectCurrent"))
    failed = true;

  if (failed)
  {
    printf("TESTS FAILED: bagel sad :(\n");
    exit(1);
  }
}

int main()
{
  bgl_tests();

  for (;;)
  {
    bgl_print(bgl_eval(bgl_read()));
  }
}

#define bool int
#define false 0
#define true 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bagel.h"

void _strip_buffer(char *buffer)
{
  buffer[strcspn(buffer, "\n\r")] = 0;
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

void bgl_error(char *message)
{
  printf("%s", message);
  exit(1);
}

/** bgl token handling */

bgl_token *bgl_create_token(char *text)
{
  bgl_token *token = malloc(sizeof(bgl_token));

  token->text = text;

  return token;
}

bgl_token_list *bgl_create_token_list(bgl_token *current)
{
  bgl_token_list *list = malloc(sizeof(bgl_token_list));

  list->previous = NULL;
  list->current = current;
  list->next = NULL;

  return list;
}

bgl_token_list *bgl_token_list_at(bgl_token_list *list, int idx)
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

bgl_token_list *bgl_token_list_get_head(bgl_token_list *list)
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
      if (*c == ')')
        token->type = TOKEN_TYPE_LIST_END;
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

    else if (*c == '+' || *c == '-' || *c == '*' || *c == '/' || *c == '=')
    {
      token->type = TOKEN_TYPE_SYMBOL;
      end = c + 1;
    }

    else
    {
      bgl_error("Unknown token type.");
      exit(1);
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

  return list;
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

void bgl_tokens_advance(bgl_token_list **tokens)
{
  *tokens = (*tokens)->next;
}

/** end bgl token handling */

/** bgl dynamic array */

bgl_dynamic_array *bgl_create_dynamic_array(size_t type_size)
{
  bgl_dynamic_array *array = malloc(sizeof(bgl_dynamic_array));

  array->capacity = 32;
  array->length = 0;
  array->type_size = type_size;

  array->data = malloc(array->capacity * array->type_size);

  return array;
}

bool bgl_dynamic_array_get(bgl_dynamic_array *array, int idx, void *output)
{
  if (idx >= array->length)
  {
    bgl_error("SPANK! SPANK! NAUGHTY PROGRAMMER.");
    return false;
  }

  memcpy(output, array->data + idx, array->type_size);

  return true;
}

void bgl_dynamic_array_insert(bgl_dynamic_array *array, void *value)
{
  if (array->length >= array->capacity)
  {
    array->capacity *= 2;
    array->data = realloc(array, array->capacity * array->type_size);
  }

  array->data[array->length++] = value;
}

/** end dynamic array */

/** bgl data types */

bgl_data *bgl_create_data(enum bgl_data_type type)
{
  bgl_data *data = malloc(sizeof(bgl_data));

  data->type = type;

  return data;
}

bgl_data *bgl_free_data(bgl_data *data)
{
  free(data);
}

bgl_data *bgl_read_form(bgl_token_list **tokens)
{
  bgl_token *token = (*tokens)->current;

  bgl_data *ast;

  switch (token->type)
  {
  case TOKEN_TYPE_LIST:
    ast = bgl_read_list(tokens);
    break;
  default:
    ast = bgl_read_atomic(tokens);
  }

  return ast;
}

bgl_data *bgl_read_atomic(bgl_token_list **tokens)
{
  bgl_token *token = (*tokens)->current;
  bgl_data *ast;

  switch (token->type)
  {
  case TOKEN_TYPE_NUMBER:
    ast = bgl_read_number(*tokens);
    break;
  case TOKEN_TYPE_STRING:
    ast = bgl_read_string(*tokens);
    break;
  case TOKEN_TYPE_SYMBOL:
    ast = bgl_read_symbol(*tokens);
    break;
  }

  bgl_tokens_advance(tokens);

  return ast;
}

bgl_data *bgl_read_list(bgl_token_list **tokens)
{
  bgl_tokens_advance(tokens);

  bgl_data *data = bgl_create_data(BGL_DATA_TYPE_LIST);

  bgl_dynamic_array *list = bgl_create_dynamic_array(sizeof(bgl_data));

  while ((*tokens)->current->type != TOKEN_TYPE_LIST_END)
  {
    bgl_data *form = bgl_read_form(tokens);
    bgl_dynamic_array_insert(list, (void *)form);
  }

  bgl_tokens_advance(tokens);

  data->value.array = list;

  return data;
}

bgl_data *bgl_read_symbol(bgl_token_list *tokens)
{
  bgl_data *data = bgl_create_data(BGL_DATA_TYPE_SYMBOL);
  data->value.symbol = tokens->current->text;

  return data;
}

bgl_data *bgl_read_string(bgl_token_list *tokens)
{
  bgl_data *data = bgl_create_data(BGL_DATA_TYPE_STRING);
  data->value.string = tokens->current->text;

  return data;
}

bgl_data *bgl_read_number(bgl_token_list *tokens)
{
  bgl_data *data = bgl_create_data(BGL_DATA_TYPE_NUMBER);
  data->value.number = atoi(tokens->current->text);

  return data;
}

void bgl_data_print(bgl_data *data)
{
  switch (data->type)
  {
  case BGL_DATA_TYPE_NUMBER:
    printf("%d", data->value.number);
    break;
  case BGL_DATA_TYPE_STRING:
    printf("%s", data->value.string);
    break;
  case BGL_DATA_TYPE_SYMBOL:
    printf("%s", data->value.symbol);
    break;
  case BGL_DATA_TYPE_LIST:
    printf("(");
    for (int i = 0; i < data->value.array->length; i++)
    {
      bgl_data_print(data->value.array->data[i]);
      if (i != data->value.array->length - 1)
        printf(" ");
    }
    printf(")");
    break;
  }
}

/** end bgl data types */

bgl_token_list *bgl_eval(bgl_token_list *tokens)
{
  bgl_data *ast = bgl_read_form(&tokens);

  bgl_data_print(ast);

  return tokens;
}

void bgl_print(bgl_token_list *tokens)
{
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

  // testing advancing tokens

  bgl_token_list *list_4 = bgl_tokenize("bagel cream cheese lox");

  bgl_tokens_advance(&list_4);

  // testing dynamic array
  bgl_dynamic_array *array_0 = bgl_create_dynamic_array(sizeof(int));

  bgl_dynamic_array_insert(array_0, (void *)10);
  bgl_dynamic_array_insert(array_0, (void *)10);
  bgl_dynamic_array_insert(array_0, (void *)10);
  bgl_dynamic_array_insert(array_0, (void *)69);
  bgl_dynamic_array_insert(array_0, (void *)10);
  bgl_dynamic_array_insert(array_0, (void *)10);

  int out = 0;

  bgl_dynamic_array_get(array_0, 3, (void *)&out);

  if (!_assert(out == 69, "DynamicArray_WhenInserted_ShouldHaveCorrectValue"))
    failed = true;

  // testing bgl_dynamic_array with bgl_data
  bgl_dynamic_array *array_1 = bgl_create_dynamic_array(sizeof(bgl_data));

  bgl_data *data_0 = malloc(sizeof(bgl_data));

  data_0->type = BGL_DATA_TYPE_NUMBER;
  data_0->value.number = 69;

  bgl_dynamic_array_insert(array_1, (void *)data_0);

  // get the data.
  bgl_data *out_data;

  bgl_dynamic_array_get(array_1, 0, (void *)&out_data);

  if (!_assert(out_data->value.number == 69, "DynamicArray_WhenInserted_ShouldHaveCorrectValue"))
    failed = true;
}

int main()
{
  bgl_tests();

  for (;;)
  {
    bgl_print(bgl_eval(bgl_read()));
  }
}

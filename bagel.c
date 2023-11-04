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

typedef struct bgl_list
{
  struct bgl_list *previous;
  bgl_token *current;
  struct bgl_list *next;
} bgl_list;

bgl_list *bgl_create_list(bgl_token *current)
{
  bgl_list *list = malloc(sizeof(bgl_list));

  list->previous = NULL;
  list->current = current;
  list->next = NULL;

  return list;
}

bgl_list *bgl_list_at(bgl_list *list, int idx)
{
  bgl_list *current = list;
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

void *bgl_list_insert(bgl_list **list, bgl_token *token)
{
  bgl_list *current = *list;

  bgl_list *new_list = bgl_create_list(token);

  new_list->next = current;
  current->previous = new_list;

  *list = new_list;
}

void *bgl_list_append(bgl_list **list, bgl_token *token)
{
  bgl_list *current = *list;

  while (current->next != NULL)
  {
    current = current->next;
  }

  bgl_list *new_list = bgl_create_list(token);

  current->next = new_list;
  new_list->previous = current;
}

bgl_list *bgl_list_get_head(bgl_list *list)
{
  bgl_list *current = list;

  while (current->previous != NULL)
  {
    current = current->previous;
  }

  return current;
}

void *bgl_list_print(bgl_list *list)
{
  bgl_list *current = list;

  while (current != NULL)
  {
    printf("%s\n", current->current->text);
    current = current->next;
  }
}

bgl_list *bgl_tokenize(char *text)
{
  bgl_list *list = NULL;
  bgl_list *current = NULL;

  char *c = text;

  while (*c != '\0')
  {
    if (*c == ' ')
    {
      c++;
      continue;
    }

    if (*c == '(' || *c == ')')
    {
      char *start = c;
      char *end = c + 1;

      char *token = malloc(sizeof(char) * (end - start + 1));
      memcpy(token, start, end - start);
      token[end - start] = '\0';

      bgl_token *bgl_token = bgl_create_token(token);

      bgl_token->type = TOKEN_TYPE_LIST;

      if (list == NULL)
      {
        list = bgl_create_list(bgl_token);
        current = list;
      }
      else
      {
        bgl_list_append(&current, bgl_token);
        current = current->next;
      }

      ++c;
      continue;
    }

    if (*c >= '0' && *c <= '9')
    {
      char *start = c;

      while (*c >= '0' && *c <= '9')
        ++c;

      char *end = c;

      char *token = malloc(sizeof(char) * (end - start + 1));
      memcpy(token, start, end - start);
      token[end - start] = '\0';

      bgl_token *bgl_token = bgl_create_token(token);

      bgl_token->type = TOKEN_TYPE_NUMBER;

      if (list == NULL)
      {
        list = bgl_create_list(bgl_token);
        current = list;
      }
      else
      {
        bgl_list_append(&current, bgl_token);
        current = current->next;
      }

      continue;
    }

    if (*c >= 'a' && *c <= 'z' || *c >= 'A' && *c <= 'Z')
    {
      char *start = c;

      while (*c >= 'a' && *c <= 'z' || *c >= 'A' && *c <= 'Z' || *c == '-')
        ++c;

      char *end = c;

      char *token = malloc(sizeof(char) * (end - start + 1));
      memcpy(token, start, end - start);
      token[end - start] = '\0';

      bgl_token *bgl_token = bgl_create_token(token);

      bgl_token->type = TOKEN_TYPE_SYMBOL;

      if (list == NULL)
      {
        list = bgl_create_list(bgl_token);
        current = list;
      }
      else
      {
        bgl_list_append(&current, bgl_token);
        current = current->next;
      }

      continue;
    }

    ++c;
  }

  list = bgl_list_get_head(list);

  bgl_list_print(list);

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

bgl_list *bgl_read()
{
  int size = 1024;
  char *line = malloc(sizeof(char) * size);
  printf("> ");

  fgets(line, size, stdin);
  _strip_buffer(line);

  bgl_list *tokens = bgl_tokenize(line);

  free(line);

  return tokens;
}

bgl_list *bgl_eval(bgl_list *tokens)
{
  return tokens;
}

void bgl_print(bgl_list *tokens)
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
  bgl_list *list_0 = bgl_create_list(token_0);

  if (!_assert(list_0 != NULL, "List_WhenCreated_ShouldNotBeNull"))
    failed = true;

  bgl_list *list_1 = bgl_create_list(token_1);
  bgl_list *list_2 = bgl_create_list(token_2);

  list_0->next = list_1;
  list_1->next = list_2;

  if (!_assert(bgl_list_at(list_0, 1) == list_1, "List_WhenCreated_ShouldHaveNext"))
    failed = true;

  if (!_assert(bgl_list_at(list_0, 69) == NULL, "List_OverIndex_ShouldBeNullAndNotCrash"))
    failed = true;

  // can tokenize bagel text.
  bgl_list *list_3 = bgl_tokenize("bagel cream cheese lox");

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

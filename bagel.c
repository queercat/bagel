#define bool int
#define false 0
#define true 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** bgl token handling */

typedef struct bgl_token
{
  char *text;
} bgl_token;

bgl_token *bgl_create_token(char *text)
{
  bgl_token *token = malloc(sizeof(bgl_token));

  token->text = text;

  return token;
}

typedef struct bgl_list
{
  bgl_token *previous;
  bgl_token *current;
  bgl_token *next;
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

/** end bgl token handling */

void bgl_error(char *message)
{
  printf("%s", message);
  exit(1);
}

char *bgl_read()
{
  int size = 1024;
  char *line = malloc(sizeof(char) * size);
  printf("> ");

  fgets(line, size, stdin);

  return line;
}

char *bgl_eval(char *text)
{
  return text;
}

void bgl_print(char *text)
{
  printf("%s\n", text);
  free(text);
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

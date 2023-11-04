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
  bgl_token *token = bgl_create_token("bagel");

  if (!_assert(token != NULL, "Token_WhenCreated_ShouldNotBeNull"))
    failed = true;
  if (!_assert(strcmp(token->text, "bagel") == 0, "Token_WhenCreated_TextValueShouldBeCorrect"))
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

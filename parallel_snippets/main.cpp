#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include "snippets.h"

using namespace std;

enum snippet_id : uint8_t
{
  find_substring
};


constexpr snippet_id id = snippet_id::find_substring;

int main()
{
  if (id == snippet_id::find_substring)
  {
    cout << "find_pattern('text.txt', 'she');" << endl;
    find_substring("text.txt", "she");

    cout <<endl << "find_pattern('text.txt', 'ONCE');" << endl;
    find_substring("text.txt", "ONCE");
  }
  // else do nothig

  system("pause");
  return 0;
}

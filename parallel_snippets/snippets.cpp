/*
* List of snippets definitions.
*
* Author: subgravitation
*/

// file: snippets.cpp

#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <thread>
#include <cassert>

using namespace std;

void find_substring(const string& filename, const string& value)
{
  string buffer; // shared buffer is filled by 1st thread and is read by 2nd
  bool filled = false; // shared filled flag is true when buffer is filled
  bool done = false; // shared done flag is true, when file reading finished

  // read file in the cycle by line
  // fill buffer if it is empty
  // signalize that work  is done
  static auto read_thread = [](const string& filename, string& buffer, bool& filled, bool& done) {
    assert(buffer.empty());
    assert(filled == false);
    assert(done == false);

    // open file
    ifstream file(filename);
    // if file isn't open - nothing to process, all work is done
    if (!file.is_open())
    {
      cout << "Cannot open file" << endl;
      done = true;
      return;
    }

    while (true)
    {
      // if buffer was processed by 2nd thread
      if (!filled)
      {
        // read next string
        getline(file, buffer);
        // signalize that buffer is filled
        filled = true;
      }

      // if file reaches the end
      if (file.eof())
      {
        // signalize that work is done
        done = true;
        break;
      }
    }
  };

  // find substring in buffer if it is filled
  // clear buffer
  // signalize buffer was processed
  static auto processing_thread = [](string& buffer, const string& value, bool& filled, bool& done) {
    while (true)
    {
      // if buffer was filled by 1nd thread
      if (filled)
      {
        // find substring in buffer
        if (buffer.find(value) != string::npos)
          cout << buffer << endl;

        // clear buffer
        buffer.clear();
        // signalize buffer was processed
        filled = false;
      }

      // finish work if input file was read
      if (done)
        break;
    }
  };

  // create input and processing threads
  thread t1(read_thread, cref(filename), ref(buffer), ref(filled), ref(done));
  thread t2(processing_thread, ref(buffer), cref(value), ref(filled), ref(done));

  // wait for their completion
  t1.join();
  t2.join();
}
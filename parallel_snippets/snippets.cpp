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

namespace
{
  template <class PredicatT, class ActionT>
  void await(PredicatT predicat, ActionT action)
  {
    while (!predicat());
    action();
  }
}

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

string copy_string(const string& input)
{
  char buffer;
  bool empty = true;
  bool done = false;
  string output;

  static auto fill_empty = [](const string& input, char& buffer, bool& empty, bool& done) {
    assert(empty == true);

    auto index = size_t{ 0 };
    while (index < input.size())
    {
      static auto empty_pre = [&empty]() { return empty; };
      static auto fill_act = [&input, &buffer, &empty, &index]() {
        buffer = input[index++];
        empty = false;
      };

      await(empty_pre, fill_act);
    }

    done = true;
  };

  static auto copy_clear = [](const char& buffer, bool& empty, const bool& done, string& output) {
    assert(output.empty());

    while (!done)
    {
      static auto full_rpe = [&empty]() { return !empty; };
      static auto copy_act = [&buffer, &empty, &output]() {
        output.append(1, buffer);
        empty = true;
      };

      await(full_rpe, copy_act);
    }
  };

  thread producer_thread(fill_empty, cref(input), ref(buffer), ref(empty), ref(done));
  thread consumer_thread(copy_clear, cref(buffer), ref(empty), cref(done), ref(output));

  // wait for their completion
  producer_thread.join();
  consumer_thread.join();

  return output;
}

/*
 * List of snippets declarations.
 *
 * Author: subgravitation
 */

// file: snippets.h 

#ifndef SNIPPETS_H
#define SNIPPETS_H

#include <string>

/*
 * Finds substrings in file lines in parrel.
 * Creates 2 threads. Fisrt one for reading input from file, second one for substring finding.
 * Threads uses shared variables for communication.
 * @param filename - name of the input file.
 * @param value - substring to find in file lines.
 */
void find_substring(const std::string& filename, const std::string& value);

std::string copy_string(const std::string& input);

#endif

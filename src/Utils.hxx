#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> splitStringByDelimiter(const std::string &input,
                                                const std::string &delimiter) {
  size_t inputStartIndex = 0, delimiterIndex = 0;
  std::vector<std::string> result;

  for (size_t inputCurrIndex = 0; inputCurrIndex < input.size();
       inputCurrIndex++) {
    // check if match, else doesn't match contiguously
    if (input[inputCurrIndex] == delimiter[delimiterIndex])
      delimiterIndex++;
    else
      delimiterIndex = 0;

    if (delimiterIndex != delimiter.size())
      continue;

    // push back everything excluding delimiter
    auto sizeWithoutDelim =
        inputCurrIndex - inputStartIndex + 1 - delimiter.size();
    result.push_back(input.substr(inputStartIndex, sizeWithoutDelim));

    // restart delim and refresh start idx
    delimiterIndex = 0;
    inputStartIndex = inputCurrIndex + 1;
  }

  if (inputStartIndex < input.size()) {
    result.push_back(
        input.substr(inputStartIndex, input.size() - inputStartIndex));
  }

  return result;
}
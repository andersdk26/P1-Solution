# P1-Solution
P1 Solution to the problem: Can trains outrun airplanes?

# Optimal Trip Planner

A C program that calculates the best trip based on user-defined preferences for time, price, and environmental impact.

## Table of Contents
- [Description](#description)
- [Installation](#installation)
- [Usage](#usage)
- [Example](#example)
- [License](#license)

## Description
**Optimal Trip Planner** is a tool designed to help users plan the most suitable trip based on their chosen parameters:
- **Time:** Maximum time available for travel.
- **Price:** Budget constraints for the trip.
- **Environmental Impact:** Preference for eco-friendly travel options.

Using these inputs, the program evaluates various travel options and recommends the best one.

## Installation
Follow these steps to compile and run the program:

1. Clone the repository:
   ```bash
   git clone https://github.com/andersdk26/P1-Solution
   cd P1-Solution\cmake-build-debug\src\
   xcopy "..\..\data\" "..\data\"
   start P1_Solution.exe
   ```
## Usage
When you run the program, you will be prompted to rate your preferences for each parameter on a scale from 1 to 5:

1. **Time:** How much time you are willing to spend on the trip.
    - 1: Very short (few hours)
    - 5: Long (several days)

2. **Price:** How important budget considerations are to you.
    - 1: Budget is not important
    - 5: Very strict budget

3. **Environmental Impact:** How much you prioritize eco-friendly options.
    - 1: Low priority for environmental impact
    - 5: High priority for eco-friendliness

The program evaluates your ratings and suggests the best trip based on these preferences.

### Input and Output
**Input:**  
Users rate each parameter (time, price, and environmental impact) from 1 to 5.

**Output:**  
The program suggests an optimal trip, such as:
```text
The best trip for you is:
- Destination: Berlin
- Transportation: Train (Eco-friendly)
- Duration: 3 days
- Budget: 150 euro
```
## Example
Det skal Tobias sætte ind. 

## License

MIT License

Copyright (c) 2024 CS-24-SW-1-16 AAU

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



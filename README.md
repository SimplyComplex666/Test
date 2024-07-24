# Instructions
Welcome to the disguise embedded code test! Congratulations on getting this this phase of the interview process, we're excited to see your coding skills! This test has a time limit of 2 hours but we expect it not to take that long (We build in extra time for potential distractions!).

Good Luck!

## Problem Description
There is a small program (code_test.cpp) that allows for simple text communication over a command line interface.

The program imitates a video system that has a number of configurable settings.

The tasks detailed in the section below involve making some changes and improvements to the code to add extra settings, or to make the program easier to debug and work on.

Please use whichever development environment you are comfortable with! This is written in 'C' language style though you are welcome to use language features from C++

## Scoring Criteria
We are particularly interested in:

- Code style
- How you check for and handle errors
- How you comprehend, edit and adapt exising code
- How you organize your data structures
- How you handle memory

## Protocol Specification
This is a small interactive command line application that uses a simple text API to set
and get some values. Once the program is running, simply enter the commands as detailed below.

The API is of the format:
'(GET|SET) <parameter> [<param value 1>]\n'

This API currently only has one command:
'SET SYS_FRATE N'
Where N is a valid frame rate number that the API can accept

For example:
'SET SYS_FRATE 50'
will set the system frame rate to 50

'GET SYS_FRATE'
will print out the current frame rate

'SET SYS_FRATE 49'
will return an error number

## Tasks
Here is a copy of the tasks which already exist in the code_test.cpp file in case you delete them by accident, We recommend pushing a commit after each task at minimum (though you are welcome to make more commits):

1) There are some 'magic numbers' used in the array allocations and memset functions. Change this 
   to be safer and more programmer-friendly

2) Improve the error handling from when an invalid frame rate is entered

3) Add a valid frame rate of '48' to the system (so that 'SET SYS_FRATE 48' will work)

4) In the 'vfc_sys_if_cfg' struct there is a video_enabled flag. Add an API command to be able to 'GET' this value

5) Add a 'SET' command to set the 'video_enabled' flag to 0 or 1

# Explanation

# Running Tests

A very basic unit test suite is implemented at `tests/`. To compile the test function, run
```bash
g++ tests/test_controller.cpp \
    TrafficLight/TrafficLightController.cpp \
    -I TrafficLight \
    -o tests/test.exe
```
Then run the program with
```bash
./tests/test
```
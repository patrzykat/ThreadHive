# ThreadHive

ThreadHive is a C++ thread pool library designed to efficiently manage a pool of threads and execute tasks using them. It offers a high-level interface for managing thread pools, with more features on the way!

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [License](#license)

## Features

- Thread pool management: Manages a set of threads to perform tasks.
- Task queueing: Queue your tasks and let the threads execute them.
- Dynamic resizing: Ability to dynamically resize the number of worker threads.
- Wait Functionality: Offers a way to wait until all tasks are executed.
- Mutex and condition variable: Ensures safe access to shared resources with `pthread_mutex` and `pthread_cond`.

## Installation

### How to use in your project

ThreadHive is easy to install. You just need to clone the repository and compile the source code:

```bash
# Clone the repository
git clone https://github.com/yourusername/ThreadHive.git

# Navigate to the repository
cd ThreadHive

# Compile the source code
make
```

Then in your files you can

```cpp
#include "ThreadHive.h"
```

### How to run interactive tests

```bash
cd ThreadHive
make interactive_tests
./interactive_tests
```

## Documentation

### Creating a ThreadHive object

Instantiate a new ThreadHive object with the desired number of worker threads.

```cpp
ThreadHive myHive(5); // creates a ThreadHive with 5 worker threads
```

### Enqueueing tasks

Tasks are represented by std::function<void()> objects. You can enqueue tasks using the enqueue method.

```cpp
myHive.enqueue(function_name);

# OR

myHive.enqueue([]{
    // code for the task goes here
});
```

### Resizing the thread pool

Dynamically change the number of worker threads in the ThreadHive object using the resize method.

```cpp
myHive.resize(10); // resizes the pool to contain 10 worker threads
```

### Waiting for all tasks to complete

The wait_all method allows you to block the current thread until all tasks in the ThreadHive have completed.

```cpp
myHive.wait_all();
```

## Contributing

We welcome contributions! Feel free submit a PR!

## License

ThreadHive is licensed under the MIT License.
